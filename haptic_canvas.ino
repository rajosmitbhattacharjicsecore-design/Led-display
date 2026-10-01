/*
 * ============================================================================
 * Project: Haptic Canvas – 4x4 Arduino LED / Actuator Display
 * Target: Arduino UNO R4 WiFi (also compatible with UNO R3, Leonardo, Mega, Nano)
 * Baud Rate: 9600 baud
 * ============================================================================
 * 
 * Hardware Pin Mapping:
 *   Row 1: D2,  D3,  D4,  D5   -> LEDs 1,  2,  3,  4
 *   Row 2: D6,  D7,  D8,  D9   -> LEDs 5,  6,  7,  8
 *   Row 3: D10, D11, D12, D13  -> LEDs 9,  10, 11, 12
 *   Row 4: A0,  A1,  A2,  A3   -> LEDs 13, 14, 15, 16
 * 
 * Circuit Wiring (Per LED):
 *   Arduino Pin -> 220 Ohm Resistor -> LED Anode (Long Leg +)
 *   LED Cathode (Short Leg -) -> Ground (GND)
 * 
 * Protocol:
 *   - Send 16 binary characters ('0' or '1') followed by newline '\n'.
 *   - Arduino updates all 16 LEDs and responds with: "PATTERN: 101001..."
 *   - Special commands (case-insensitive):
 *       "PING"  -> Responds with "PONG"
 *       "TEST"  -> Runs sequential LED sweep animation
 *       "CLEAR" -> Turns off all LEDs
 *       "ALL"   -> Turns on all LEDs
 * ============================================================================
 */

// 16 LED pins in row-major order (Row 1: D2-D5, Row 2: D6-D9, Row 3: D10-D13, Row 4: A0-A3)
const int ledPins[16] = {
   2,  3,  4,  5,   // Row 1 (LEDs 1..4)
   6,  7,  8,  9,   // Row 2 (LEDs 5..8)
  10, 11, 12, 13,   // Row 3 (LEDs 9..12)
  A0, A1, A2, A3    // Row 4 (LEDs 13..16)
};

String buffer = "";
unsigned long lastCharTime = 0;
const unsigned long BUFFER_TIMEOUT_MS = 500; // Reset buffer if transmission stalls

// Set all LEDs to a specific state
void setAll(int state) {
  for (int i = 0; i < 16; i++) {
    digitalWrite(ledPins[i], state);
  }
}

// Power-on self-test animation to verify every LED and resistor
void runSelfTestAnimation() {
  // 1. Sequential sweep: D2 -> A3
  for (int i = 0; i < 16; i++) {
    digitalWrite(ledPins[i], HIGH);
    delay(40);
    digitalWrite(ledPins[i], LOW);
  }
  // 2. Quick double flash of all LEDs
  for (int f = 0; f < 2; f++) {
    delay(70);
    setAll(HIGH);
    delay(70);
    setAll(LOW);
  }
}

void setup() {
  // Configure all 16 pins as digital outputs and turn off
  for (int i = 0; i < 16; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }

  // Initialize USB Serial communication at 9600 baud
  Serial.begin(9600);

  // Run initial LED sweep so the user visually sees hardware is ready
  runSelfTestAnimation();

  Serial.println("READY: Haptic Canvas 4x4 Controller Online (9600 baud)");
}

void loop() {
  // Buffer timeout protection: if half a packet arrived and stopped, reset buffer
  if (buffer.length() > 0 && (millis() - lastCharTime > BUFFER_TIMEOUT_MS)) {
    buffer = "";
  }

  while (Serial.available() > 0) {
    char c = Serial.read();
    lastCharTime = millis();

    // Check for delimiter (newline / carriage return)
    if (c == '\n' || c == '\r') {
      buffer.trim();
      if (buffer.length() == 0) {
        continue;
      }

      // Check special textual commands
      if (buffer.equalsIgnoreCase("PING")) {
        Serial.println("PONG");
        buffer = "";
        continue;
      }
      if (buffer.equalsIgnoreCase("CLEAR")) {
        setAll(LOW);
        Serial.println("STATUS: CLEARED");
        buffer = "";
        continue;
      }
      if (buffer.equalsIgnoreCase("ALL")) {
        setAll(HIGH);
        Serial.println("STATUS: ALL_ON");
        buffer = "";
        continue;
      }
      if (buffer.equalsIgnoreCase("TEST")) {
        runSelfTestAnimation();
        Serial.println("STATUS: TEST_COMPLETE");
        buffer = "";
        continue;
      }

      // If buffer is 16 bits, apply it
      if (buffer.length() == 16) {
        bool valid = true;
        for (int i = 0; i < 16; i++) {
          if (buffer.charAt(i) != '0' && buffer.charAt(i) != '1') {
            valid = false;
            break;
          }
        }
        if (valid) {
          for (int i = 0; i < 16; i++) {
            digitalWrite(ledPins[i], buffer.charAt(i) == '1' ? HIGH : LOW);
          }
          Serial.print("PATTERN: ");
          Serial.println(buffer);
        } else {
          Serial.print("ERROR: Invalid characters in pattern: ");
          Serial.println(buffer);
        }
      }
      buffer = "";
      continue;
    }

    // Accumulate characters
    if (c == '0' || c == '1' || isAlpha(c)) {
      buffer += c;
    }

    // Immediate 16-bit pattern triggering (backward compatibility with original sketch)
    if (buffer.length() == 16) {
      bool allBinary = true;
      for (int i = 0; i < 16; i++) {
        if (buffer.charAt(i) != '0' && buffer.charAt(i) != '1') {
          allBinary = false;
          break;
        }
      }

      if (allBinary) {
        for (int i = 0; i < 16; i++) {
          digitalWrite(ledPins[i], buffer.charAt(i) == '1' ? HIGH : LOW);
        }
        Serial.print("PATTERN: ");
        Serial.println(buffer);
        buffer = "";
      }
    }
  }
}
