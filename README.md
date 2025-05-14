# Digispark Dual Button Macro Pad

A tiny and configurable USB device that executes keyboard shortcuts when two physical buttons are pressed.

- Supports 2 external buttons via 3.5mm headphone jack (TRS)
- Remembers configuration in EEPROM
- Enter setup mode without reprogramming
- Built with a Digispark (ATtiny85)

---

## 🎮 Inspiration

This project was inspired by the [8BitDo Dual Super Buttons](https://shop.8bitdo.com/products/8bitdo-dual-super-buttons). The goal was to create a fully standalone version that works as a USB keyboard without needing the original 8BitDo controller.

## 🔧 Hardware

### Required components:
- 1x Digispark (ATtiny85)
- 1x TRS 3.5mm female headphone jack (3 vias)
- 2x normally open push-buttons
- Wires, solder, optional 3D-printed enclosure

### Pin connections:

```
       [ 3.5mm TRS Jack - Female ]
        _________________________
       | Tip   (BTN_A) -> P2    |
       | Ring  (BTN_B) -> P0    |
       | Sleeve (GND) -> GND    |
       |________________________|
```

### On the Digispark:
- BTN_A is connected to **P2** (digital pin 2)
- BTN_B is connected to **P0** (digital pin 0)
- GND goes to sleeve of jack

> No pull-up resistors needed — they are enabled internally via `INPUT_PULLUP`

---

## 🔌 Usage

1. Plug the device into your USB port
2. Press either button to execute the configured macro
3. Hold a button for **3 seconds** to enter setup mode
4. Use that button to navigate through options, and the other to confirm
5. Hold both buttons for **3 seconds** to reset to default (Copy & Paste)

---

## ⌨️ Default Commands
- **A** → Paste (`Cmd + V`)
- **B** → Copy (`Cmd + C`)

You can change them to other commands like Undo, Save, Delete, Volume Up, etc.

---

## 🧠 Configuration Options
- Copy, Paste, Cut, Save, Undo, Redo, Select All
- Enter, Tab, Esc, Space, Backspace, Delete
- Volume Up, Volume Down, Mute
- F13, F14 (useful for custom bindings)

---

## 🛠️ Programming the Digispark

1. Install the [Digistump Arduino package](http://digistump.com/wiki/digispark/tutorials/connecting)
2. Use **Arduino IDE** and select "Digispark (Default - 16.5mhz)"
3. Upload the `digispark-macro-pad.ino` file
4. When prompted, plug in your Digispark

---

## ✅ License
MIT — free to use, modify, and distribute.

---

Made with ❤️ using ATTiny85, buttons, ChatGPT, and good intentions.
