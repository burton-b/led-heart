// ❤️ LED Heart Display
// Switch UP = song + magic LEDs then patterns
// Switch DOWN = everything off

// === PIN DEFINITIONS ===
const int LED_PINS[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, A1, A2};
const int NUM_LEDS = 14;

const int BRIGHT_PIN  = A0;
const int POWER_SW    = A3;
const int PATTERN_BTN = A4;
const int BUZZER_PIN  = A5;

// === STATE ===
int currentPattern = 0;
const int NUM_PATTERNS = 5;
int brightness = 255;
bool lastSwitchState = HIGH;
bool songDone = false;

unsigned long lastPatternPress = 0;
const int DEBOUNCE = 200;

// === NOTES ===
#define REST 0
#define C4   262
#define D4   294
#define E4   330
#define F4   349
#define G4   392
#define A4n  440
#define B4   494
#define C5   523

#define Q  400
#define H  800
#define E  200

// =====================
void setup() {
  for (int i = 0; i < NUM_LEDS; i++) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW);
  }
  pinMode(POWER_SW,    INPUT_PULLUP);
  pinMode(PATTERN_BTN, INPUT_PULLUP);
  pinMode(BUZZER_PIN,  OUTPUT);

  randomSeed(analogRead(A3));
}

// =====================
void loop() {
  brightness = map(analogRead(BRIGHT_PIN), 0, 1023, 10, 255);
  bool switchState = digitalRead(POWER_SW);

  // Switch DOWN = everything off
  if (switchState == HIGH) {
    allOff();
    noTone(BUZZER_PIN);
    songDone = false;
    lastSwitchState = HIGH;
    return;
  }

  // Switch just flipped UP
  if (switchState == LOW && lastSwitchState == HIGH) {
    delay(50);
    lastSwitchState = LOW;
    songDone = false;
    allOff();
    playTwinkleWithLEDs();
    songDone = true;
    allOn();
  }

  // After song — pattern mode available
  if (songDone) {
    if (digitalRead(PATTERN_BTN) == LOW && millis() - lastPatternPress > DEBOUNCE) {
      lastPatternPress = millis();
      currentPattern = (currentPattern + 1) % NUM_PATTERNS;
      playSwitch();
      allOff();
    }

    switch (currentPattern) {
      case 0: patternAllOn();     break;
      case 1: patternPulse();     break;
      case 2: patternSpiral();    break;
      case 3: patternAlternate(); break;
      case 4: patternHeartbeat(); break;
    }
  }
}

// =====================
// PLAY NOTE + LIGHT UP RANDOM LED
// =====================
void playNoteWithLED(int freq, int duration) {
  allOff();
  int led = random(0, NUM_LEDS);
  analogWrite(LED_PINS[led], brightness);

  if (freq != REST) {
    long period = 1000000L / freq;
    long cycles = (long)freq * duration / 1000;
    for (long i = 0; i < cycles; i++) {
      digitalWrite(BUZZER_PIN, HIGH);
      delayMicroseconds(period / 2);
      digitalWrite(BUZZER_PIN, LOW);
      delayMicroseconds(period / 2);
    }
  } else {
    delay(duration);
  }

  digitalWrite(LED_PINS[led], LOW);
  delay(30);
}

void playNoteWithMultiLED(int freq, int duration, int numLeds) {
  allOff();
  for (int n = 0; n < numLeds; n++) {
    int led = random(0, NUM_LEDS);
    analogWrite(LED_PINS[led], brightness);
  }

  if (freq != REST) {
    long period = 1000000L / freq;
    long cycles = (long)freq * duration / 1000;
    for (long i = 0; i < cycles; i++) {
      digitalWrite(BUZZER_PIN, HIGH);
      delayMicroseconds(period / 2);
      digitalWrite(BUZZER_PIN, LOW);
      delayMicroseconds(period / 2);
    }
  } else {
    delay(duration);
  }

  allOff();
  delay(30);
}

// =====================
// TWINKLE TWINKLE WITH LED MAGIC
// =====================
void playTwinkleWithLEDs() {
  // Twinkle twinkle little star
  playNoteWithLED(C4,  Q);
  playNoteWithLED(C4,  Q);
  playNoteWithLED(G4,  Q);
  playNoteWithLED(G4,  Q);
  playNoteWithLED(A4n, Q);
  playNoteWithLED(A4n, Q);
  playNoteWithMultiLED(G4, H, 4);

  // How I wonder what you are
  playNoteWithLED(F4,  Q);
  playNoteWithLED(F4,  Q);
  playNoteWithLED(E4,  Q);
  playNoteWithLED(E4,  Q);
  playNoteWithLED(D4,  Q);
  playNoteWithLED(D4,  Q);
  playNoteWithMultiLED(C4, H, 4);

  // Up above the world so high
  playNoteWithLED(G4,  Q);
  playNoteWithLED(G4,  Q);
  playNoteWithLED(F4,  Q);
  playNoteWithLED(F4,  Q);
  playNoteWithLED(E4,  Q);
  playNoteWithLED(E4,  Q);
  playNoteWithMultiLED(D4, H, 4);

  // Like a diamond in the sky
  playNoteWithLED(G4,  Q);
  playNoteWithLED(G4,  Q);
  playNoteWithLED(F4,  Q);
  playNoteWithLED(F4,  Q);
  playNoteWithLED(E4,  Q);
  playNoteWithLED(E4,  Q);
  playNoteWithMultiLED(D4, H, 4);

  // Twinkle twinkle little star
  playNoteWithLED(C4,  Q);
  playNoteWithLED(C4,  Q);
  playNoteWithLED(G4,  Q);
  playNoteWithLED(G4,  Q);
  playNoteWithLED(A4n, Q);
  playNoteWithLED(A4n, Q);
  playNoteWithMultiLED(G4, H, 4);

  // How I wonder what you are
  playNoteWithLED(F4,  Q);
  playNoteWithLED(F4,  Q);
  playNoteWithLED(E4,  Q);
  playNoteWithLED(E4,  Q);
  playNoteWithLED(D4,  Q);
  playNoteWithLED(D4,  Q);
  playNoteWithMultiLED(C4, H, 6);

  // Final sparkle
  for (int i = 0; i < 3; i++) {
    allOn();
    delay(150);
    allOff();
    delay(100);
  }
}

// =====================
// JINGLES
// =====================
void playNote(int freq, int duration) {
  if (freq == REST) { delay(duration); return; }
  long period = 1000000L / freq;
  long cycles = (long)freq * duration / 1000;
  for (long i = 0; i < cycles; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delayMicroseconds(period / 2);
    digitalWrite(BUZZER_PIN, LOW);
    delayMicroseconds(period / 2);
  }
  delay(30);
}

void playSwitch() {
  playNote(C5, 100);
  playNote(G4, 150);
}

void playHeartTick() {
  playNote(G4, 40);
  delay(20);
  playNote(G4, 40);
}

// =====================
// PATTERNS
// =====================
void patternAllOn() {
  for (int i = 0; i < NUM_LEDS; i++) analogWrite(LED_PINS[i], brightness);
  delay(50);
}

void patternPulse() {
  for (int b = 10; b <= 255; b += 3) {
    brightness = map(analogRead(BRIGHT_PIN), 0, 1023, 10, 255);
    int level = map(b, 10, 255, 10, brightness);
    for (int i = 0; i < NUM_LEDS; i++) analogWrite(LED_PINS[i], level);
    if (digitalRead(POWER_SW) == HIGH) return;
    delay(8);
  }
  for (int b = 255; b >= 10; b -= 3) {
    brightness = map(analogRead(BRIGHT_PIN), 0, 1023, 10, 255);
    int level = map(b, 255, 10, brightness, 10);
    for (int i = 0; i < NUM_LEDS; i++) analogWrite(LED_PINS[i], level);
    if (digitalRead(POWER_SW) == HIGH) return;
    delay(8);
  }
}

void patternSpiral() {
  for (int i = 0; i < NUM_LEDS; i++) {
    analogWrite(LED_PINS[i], brightness);
    if (digitalRead(POWER_SW) == HIGH) return;
    delay(60);
  }
  for (int i = NUM_LEDS - 1; i >= 0; i--) {
    digitalWrite(LED_PINS[i], LOW);
    if (digitalRead(POWER_SW) == HIGH) return;
    delay(60);
  }
}

void patternAlternate() {
  for (int i = 0; i < NUM_LEDS; i++) {
    if (i % 2 == 0) analogWrite(LED_PINS[i], brightness);
    else digitalWrite(LED_PINS[i], LOW);
  }
  if (digitalRead(POWER_SW) == HIGH) return;
  delay(300);

  for (int i = 0; i < NUM_LEDS; i++) {
    if (i % 2 != 0) analogWrite(LED_PINS[i], brightness);
    else digitalWrite(LED_PINS[i], LOW);
  }
  if (digitalRead(POWER_SW) == HIGH) return;
  delay(300);
}

void patternHeartbeat() {
  allOn(); delay(80);
  allOff(); delay(80);
  allOn(); delay(80);
  allOff();
  playHeartTick();
  if (digitalRead(POWER_SW) == HIGH) return;
  delay(600);
}

// =====================
// HELPERS
// =====================
void allOn() {
  for (int i = 0; i < NUM_LEDS; i++) analogWrite(LED_PINS[i], brightness);
}

void allOff() {
  for (int i = 0; i < NUM_LEDS; i++) digitalWrite(LED_PINS[i], LOW);
}
