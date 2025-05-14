// Digispark Configurable Macro Pad with Modifier Support
#include <DigiKeyboard.h>
#include <avr/eeprom.h>

#define BTN_A 2
#define BTN_B 0
#define EEPROM_ADDR_A 0
#define EEPROM_ADDR_B (EEPROM_ADDR_A + 2)
#define SETUP_HOLD_TIME 3000
#define RESET_HOLD_TIME 3000

#define KEY_C 0x06
#define KEY_V 0x19
#define KEY_X 0x07
#define KEY_Z 0x1D
#define KEY_Y 0x1C
#define KEY_A 0x04
#define KEY_S 0x16
#define KEY_W 0x1A
#define KEY_ESC 0x29
#define KEY_TAB 0x2B
#define KEY_ENTER 0x28
#define KEY_SPACE 0x2C
#define KEY_BACKSPACE 0x2A
#define KEY_DELETE 0x4C
#define KEY_VOL_UP 0x80
#define KEY_VOL_DOWN 0x81
#define KEY_MUTE 0x7F
#define KEY_F13 0x68
#define KEY_F14 0x69

uint8_t eeprom_read(uint16_t addr) {
  return eeprom_read_byte((uint8_t*)addr);
}

void eeprom_write(uint16_t addr, uint8_t val) {
  eeprom_write_byte((uint8_t*)addr, val);
}

bool waitHold(int pin, unsigned long ms) {
  unsigned long start = millis();
  while (digitalRead(pin) == LOW) {
    DigiKeyboard.update();
    if ((millis() - start) >= ms) return true;
  }
  return false;
}

bool waitHoldBoth(int pin1, int pin2, unsigned long ms) {
  unsigned long start = millis();
  while (digitalRead(pin1) == LOW && digitalRead(pin2) == LOW) {
    DigiKeyboard.update();
    if ((millis() - start) >= ms) return true;
  }
  return false;
}

void blinkLedOnce() {
  pinMode(1, OUTPUT);
  digitalWrite(1, HIGH);
  delay(200);
  digitalWrite(1, LOW);
}

void resetBtns() {
  eeprom_write(EEPROM_ADDR_A, MOD_GUI_LEFT);
  eeprom_write(EEPROM_ADDR_A + 1, KEY_V);
  eeprom_write(EEPROM_ADDR_B, MOD_GUI_LEFT);
  eeprom_write(EEPROM_ADDR_B + 1, KEY_C);

  for (int i = 0; i < 3; i++) {
    blinkLedOnce();
    delay(150);
  }
}

void setup() {
  pinMode(BTN_A, INPUT_PULLUP);
  pinMode(BTN_B, INPUT_PULLUP);
  DigiKeyboard.update();
  DigiKeyboard.delay(300);

  if (eeprom_read(EEPROM_ADDR_A + 1) == 0xFF) {
    resetBtns();
  }
}

void sendKey(uint16_t eepromAddr) {
  DigiKeyboard.update();
  DigiKeyboard.delay(300);
  uint8_t mod = eeprom_read(eepromAddr);
  uint8_t key = eeprom_read(eepromAddr + 1);
  if (key != 0x00 && key <= 0xE7) {
    DigiKeyboard.sendKeyStroke(key, mod);
  }
}

void interactiveSetup(uint8_t configPin, uint8_t confirmPin, uint16_t eepromAddr) {
  const char* options[] = {
    "Copy", "Paste", "Cut", "Save", "Undo", "Redo", "Select All",
    "Enter", "Tab", "Esc", "Space", "Backspace", "Delete",
    "Vol Up", "Vol Down", "Mute", "F13", "F14"
  };
  const uint8_t codes[] = {
    KEY_C, KEY_V, KEY_X, KEY_S, KEY_Z, KEY_Y, KEY_A,
    KEY_ENTER, KEY_TAB, KEY_ESC, KEY_SPACE, KEY_BACKSPACE, KEY_DELETE,
    KEY_VOL_UP, KEY_VOL_DOWN, KEY_MUTE, KEY_F13, KEY_F14
  };
  const int count = sizeof(codes) / sizeof(codes[0]);
  int index = 0;
  int lastIndex = -1;

  blinkLedOnce();
  
  DigiKeyboard.print("# SETUP MODE: ");
  if (configPin == BTN_A) {
    DigiKeyboard.println("A: navigate / B: confirm");
  } else {
    DigiKeyboard.println("B: navigate / A: confirm");
  }

  uint8_t currentKey = eeprom_read(eepromAddr + 1);
  for (int i = 0; i < count; i++) {
    if (codes[i] == currentKey) {
      index = i;
      break;
    }
  }

  while (true) {
    if (index != lastIndex) {
      DigiKeyboard.print("> ");
      DigiKeyboard.println(options[index]);
      delay(100);
      lastIndex = index;
    }

    while (digitalRead(configPin) == HIGH && digitalRead(confirmPin) == HIGH);

    if (digitalRead(configPin) == LOW) {
      while (digitalRead(configPin) == LOW);
      delay(150);
      index = (index + 1) % count;
    }

    if (digitalRead(confirmPin) == LOW) {
      while (digitalRead(confirmPin) == LOW);
      delay(150);
      uint8_t mod = (index < 6) ? MOD_GUI_LEFT : 0;
      if (eeprom_read(eepromAddr) != mod) eeprom_write(eepromAddr, mod);
      if (eeprom_read(eepromAddr + 1) != codes[index]) eeprom_write(eepromAddr + 1, codes[index]);
      blinkLedOnce();
      DigiKeyboard.print("# SETUP SAVED: ");
      DigiKeyboard.println(options[index]);
      delay(300);
      return;
    }
  }
}

void loop() {
  if (waitHoldBoth(BTN_A, BTN_B, RESET_HOLD_TIME)) {
    resetBtns();
    return;
  }

  if (digitalRead(BTN_A) == LOW) {
    if (waitHold(BTN_A, SETUP_HOLD_TIME)) {
      interactiveSetup(BTN_A, BTN_B, EEPROM_ADDR_A);
    } else {
      sendKey(EEPROM_ADDR_A);
    }
    delay(300);
    return;
  }

  if (digitalRead(BTN_B) == LOW) {
    if (waitHold(BTN_B, SETUP_HOLD_TIME)) {
      interactiveSetup(BTN_B, BTN_A, EEPROM_ADDR_B);
    } else {
      sendKey(EEPROM_ADDR_B);
    }
    delay(300);
    return;
  }
}
