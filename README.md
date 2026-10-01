# Haptic Canvas – 4×4 Arduino Assistive Display & Reader

An assistive technology and Human–Computer Interaction (HCI) prototype converting visual input, typography, and documents into a physical 4×4 tactile actuator / LED representation, with Web Serial communication to an **Arduino UNO R4 WiFi**.

---

## 🚀 Key Improvements in this Upgrade

| Area | Original Version | Upgraded Version |
| :--- | :--- | :--- |
| **User Interface** | Basic dark layout, static buttons | Modern Cyber-Engineering glassmorphic UI with glowing LED optics & color themes (Emerald, Cyan, Amber, Red, Purple) |
| **Interactive Grid** | Read-only display of cells | **Direct Click-to-Toggle**: Click any of the 16 cells to manually toggle the LED state on the fly |
| **Drawing Canvas** | Disconnected dots when moving fast | Smooth quadratic bezier stroke interpolation, Pen & Eraser tools, Brush size slider, 4×4 cell fill percentage guides |
| **Character & Braille** | Only uppercase A–Z and digits | Full A–Z, 0–9, punctuation (`!`, `?`, `.`, `,`, etc.) + **Standard Grade 1 Braille Mode** (6-dot cell mapping) |
| **Document Reader** | Hardcoded 3-second delay, no audio | **Configurable speed slider (0.5s–5.0s)**, Step forward/back controls, interactive text scrubber, **Text-to-Speech (TTS) audio narration** |
| **Serial Communication** | Write-only, escaped `\\n` string bug | Full **bidirectional telemetry** (`port.readable` + `port.writable`), handshake ping/pong, echo verification, robust line buffering |
| **Hardware Testing** | None (trial and error) | **Diagnostic Suite**: Individual pin test buttons (D2..A3), sequential sweep, row/column scan, checkerboard |
| **Localhost Execution** | Manual command line required | **1-Click Launchers**: `start_server.bat` and `start_server.py` to auto-open `http://localhost:8000` |
| **Arduino Sketch** | Basic loop without timeout guard | **Power-on self-test sweep**, watchdog buffer reset, command handling (`PING`, `TEST`, `CLEAR`, `ALL`) |
| **Offline Testing** | Required physical hardware | **Virtual Simulation Mode** for testing all UI & reader features without hardware attached |

---

## 🔌 Hardware Circuit & Pinout

Each LED is connected in series with a **220 Ω resistor** to protect the Arduino pins and LEDs from overcurrent:

$$\text{Arduino Pin} \longrightarrow 220\,\Omega\text{ Resistor} \longrightarrow \text{LED Anode (Long Leg +)} \longrightarrow \text{LED Cathode (Short Leg -)} \longrightarrow \text{GND}$$

### 4×4 Matrix Pin Mapping Table

| Row / Column | LED Number | Arduino UNO R4 Pin | Bit Position in Serial Stream |
| :--- | :--- | :--- | :--- |
| **Row 1, Col 1** | LED 1 | `D2` | Bit 0 (1st character) |
| **Row 1, Col 2** | LED 2 | `D3` | Bit 1 |
| **Row 1, Col 3** | LED 3 | `D4` | Bit 2 |
| **Row 1, Col 4** | LED 4 | `D5` | Bit 3 |
| **Row 2, Col 1** | LED 5 | `D6` | Bit 4 |
| **Row 2, Col 2** | LED 6 | `D7` | Bit 5 |
| **Row 2, Col 3** | LED 7 | `D8` | Bit 6 |
| **Row 2, Col 4** | LED 8 | `D9` | Bit 7 |
| **Row 3, Col 1** | LED 9 | `D10` | Bit 8 |
| **Row 3, Col 2** | LED 10 | `D11` | Bit 9 |
| **Row 3, Col 3** | LED 11 | `D12` | Bit 10 |
| **Row 3, Col 4** | LED 12 | `D13` | Bit 11 |
| **Row 4, Col 1** | LED 13 | `A0` (configured as digital pin) | Bit 12 |
| **Row 4, Col 2** | LED 14 | `A1` (configured as digital pin) | Bit 13 |
| **Row 4, Col 3** | LED 15 | `A2` (configured as digital pin) | Bit 14 |
| **Row 4, Col 4** | LED 16 | `A3` (configured as digital pin) | Bit 15 (16th character) |

> [!IMPORTANT]
> Do NOT share one resistor among multiple LEDs. Each LED must have its own dedicated 220 Ω resistor.

---

## 🛠️ Step 1: Upload Arduino Code (Arduino IDE)

1. Open the **Arduino IDE** (v2.x or v1.8.x).
2. Connect your **Arduino UNO R4 WiFi** via USB-C cable.
3. In the Arduino IDE menu, go to **File → Open...** and select:
   ```
   arduino/haptic_canvas/haptic_canvas.ino
   ```
4. Select your board: **Tools → Board → Arduino UNO R4 Boards → Arduino UNO R4 WiFi**.
5. Select your COM Port: **Tools → Port → COM... (Arduino UNO R4 WiFi)**.
6. Click the **Upload (→)** button.
7. Upon successful upload, the Arduino will run an automatic power-on LED sweep from LED 1 to 16, followed by two flashes, confirming the circuit is properly wired.
8. **CRITICAL:** Close the Arduino IDE Serial Monitor / Serial Plotter before opening the website! Web Serial requires exclusive access to the COM port.

---

## 🌐 Step 2: Start the Web Application

The Web Serial API requires a secure origin (`http://localhost:8000` or `https://`).

### Windows 1-Click Launch:
Simply double-click:
```
start_server.bat
```
This will automatically launch the local Python server and open your default browser (Chrome or Edge recommended) to `http://localhost:8000`.

### Manual Command Line Launch:
```powershell
py start_server.py
```
Or with standard Python:
```powershell
python -m http.server 8000
```
Then navigate to: `http://localhost:8000`

---

## 📖 Feature Guide

### 1. 🔌 Connecting the Arduino
- Click **"🔌 Connect Arduino"** in the top bar.
- Choose your Arduino UNO R4 WiFi port from the browser prompt.
- The indicator will turn **Green** with status `Connected to Arduino`.

### 2. ✏️ Draw Studio
- Draw freehand on the canvas with your mouse, trackpad, or touchscreen/stylus.
- Toggle the **Show 4×4 Grid Guide** to see exactly how your stroke falls across the cells.
- Adjust **Sampling Threshold** (5% to 60%) to control how sensitive the cell activation is.
- Use preset buttons (Triangle, Circle, Heart, Cross, etc.) for quick shapes.

### 3. 🔤 Character & Braille Studio
- Type any key on your keyboard to instantly see its 4×4 pattern.
- Switch between **"4×4 Pixel Font"** and **"Tactile Braille (Grade 1)"** modes.
- Explore the interactive glyph gallery (A–Z, 0–9, and punctuation).

### 4. 📄 Automatic Document Reader
- Click **"Choose TXT / PDF File"** or paste text into the text area.
- Select from presets (`HELLO`, `HAPTIC 4X4`, `THE QUICK BROWN FOX`).
- Adjust the **Reading Pause Duration** slider (0.5s up to 5.0s, default 3.0s).
- Click any character in the **Document Stream Scrubber** to jump directly to it.
- Enable **🗣️ Speech Audio (TTS)** for multisensory voice readout while the actuators pulse!
- Use controls: `▶ Start`, `⏸ Pause`, `⏮ Step Back`, `⏭ Step Next`, `⏹ Stop`.

### 5. 🛠️ Hardware Diagnostic Suite
- Click **"⚡ Run LED Sequential Sweep"** to test pins D2 through A3 in order.
- Test individual pins with one click to locate loose wires or backwards LEDs.
- Run Row-by-Row and Column-by-Column test sweeps.

### 6. 📡 Serial Terminal
- Monitor live bidirectional data traffic (`TX` outgoing patterns and `RX` Arduino acknowledgments).
- Send test commands:
  - `PING` (tests connection round-trip latency)
  - `TEST` (runs hardware sweep)
  - `CLEAR` (turns off all LEDs)
  - `ALL` (turns on all LEDs)
  - `1000000000000001` (custom 16-bit pattern)

---

## ❓ Troubleshooting

1. **"No port selected" or "Port in use"**:
   - Make sure the **Arduino IDE Serial Monitor** is CLOSED. Only one program can use the COM port at a time.
2. **"Web Serial unavailable"**:
   - Web Serial is supported in **Google Chrome** and **Microsoft Edge**. Make sure you are using one of these browsers.
   - Ensure the site is running on `http://localhost:8000` (not `file:///`).
3. **LED does not light up**:
   - Check LED orientation: the long leg is Anode (+), connected to the 220 Ω resistor. The short leg is Cathode (-), connected to GND.
   - Use the **Hardware Diagnostic** tab to test that specific pin directly.
