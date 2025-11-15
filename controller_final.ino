#include <Keyboard.h>
#include <Mouse.h>

//--------------------------------------------BUTTON CONFIGURATION
const int buttonPins[] = {6, 5, 3, 2, 7, 4, 8};
const char buttonKeys[] = {'d', 'f', 'j', 'k', 'c', 'm', '1'};
const int numButtons = sizeof(buttonPins) / sizeof(buttonPins[0]);

int lastButtonState[numButtons];
unsigned long lastDebounceTime[numButtons];
const unsigned long debounceDelay = 10;  // 10 ms debounce

//--------------------------------------------ENCODER CONFIGURATION
#define CLK_1_pin 14
#define DT_1_pin 15
#define CLK_2_pin 16
#define DT_2_pin 10

int lastState1;
int lastState2;

void setup() {
  Serial.begin(9600);
  Keyboard.begin();
  Mouse.begin();

  // Setup button pins
  for (int i = 0; i < numButtons; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
    lastButtonState[i] = digitalRead(buttonPins[i]);
    lastDebounceTime[i] = 0;
  }

  // Setup encoders
  pinMode(CLK_1_pin, INPUT);
  pinMode(DT_1_pin, INPUT);
  pinMode(CLK_2_pin, INPUT);
  pinMode(DT_2_pin, INPUT);

  lastState1 = digitalRead(CLK_1_pin);
  lastState2 = digitalRead(CLK_2_pin);
}

void loop() {
  // -------------------- BUTTON HANDLING (Debounced)
  for (int i = 0; i < numButtons; i++) {
    int reading = digitalRead(buttonPins[i]);

    if (reading != lastButtonState[i]) {
      lastDebounceTime[i] = millis();  // reset debounce timer
    }

    if ((millis() - lastDebounceTime[i]) > debounceDelay) {
      if (reading == LOW) {
        Keyboard.press(buttonKeys[i]);
      } else {
        Keyboard.release(buttonKeys[i]);
      }
    }

    lastButtonState[i] = reading;
  }

  // -------------------- ENCODER 1 (Horizontal)
  int currentState1 = digitalRead(CLK_1_pin);
  if (currentState1 != lastState1) {
    if (digitalRead(DT_1_pin) != currentState1) {
      Mouse.move(90, 0);   // move right
      Serial.println("Encoder 1: Right");
    } else {
      Mouse.move(-90, 0);  // move left
      Serial.println("Encoder 1: Left");
    }
  }
  lastState1 = currentState1;

  // -------------------- ENCODER 2 (Vertical)
  int currentState2 = digitalRead(CLK_2_pin);
  if (currentState2 != lastState2) {
    if (digitalRead(DT_2_pin) != currentState2) {
      Mouse.move(0, -90);   // move up
      Serial.println("Encoder 2: Up");
    } else {
      Mouse.move(0, 90);    // move down
      Serial.println("Encoder 2: Down");
    }
  }
  lastState2 = currentState2;
}
