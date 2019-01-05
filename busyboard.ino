#define RED 0
#define GREEN 1
#define BLUE 4
#define BTN 2
#define LEVEL A3

#define RMODE 0
#define GMODE 1
#define BMODE 2
#define AMODE 3
#define NMODE 4

const int dur = 500;
const int diff = 20;

boolean lastButton = LOW;
boolean currentButton = LOW;
unsigned short currentMode = NMODE;

int rValue = 0;
int gValue = 0;
int bValue = 0;

int prevValue = 0;
int curValue = 0;

void setup() {
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(BTN, INPUT);
  blinkAll(dur, 3);
}

void loop() {
  currentButton = debounce(lastButton);
  if (lastButton == LOW && currentButton == HIGH) {
    if (currentMode == NMODE)
      currentMode = 0;
    else
      currentMode = currentMode + 1;
    switch (currentMode) {
      case RMODE:
        blinkLed(RED, dur, 2);
        break;
      case GMODE:
        blinkLed(GREEN, dur, 2);
        break;
      case BMODE:
        blinkLed(BLUE, dur, 2);
        break;
      case AMODE:
        blinkAll(dur, 2);
        break;
      case NMODE:
        blinkAll(dur, 4);
        break;
    }
    prevValue = analogRead(LEVEL);
  }
  lastButton = currentButton;
  handleValues();
}

void blinkLed(const int led, const int duration, const int times) {
  for (int i = 0; i < times; i++) {
    delay(duration);
    digitalWrite(led, HIGH);
    delay(duration);
    digitalWrite(led, LOW);
  }
  showCurrent();
}

void blinkAll(const int duration, const int times) {
  for (int i = 0; i < times; i++) {
    delay(duration);
    digitalWrite(RED, HIGH);
    digitalWrite(BLUE, HIGH);
    digitalWrite(GREEN, HIGH);
    delay(duration);
    digitalWrite(RED, LOW);
    digitalWrite(BLUE, LOW);
    digitalWrite(GREEN, LOW);
  }
  delay(duration);
  showCurrent();
}

boolean debounce(boolean last) {
  boolean current = digitalRead(BTN);
  if (last != current) {
    delay(10);
    current = digitalRead(BTN);
    return current;
  }
}

void handleValues() {
  if (currentMode == NMODE)
    prevValue = analogRead(LEVEL);
  else {
    curValue = analogRead(LEVEL);
    const int readValue = curValue;
    if (abs(curValue - prevValue) > diff) {
      if(curValue > 500)
         curValue += diff;
      else
         curValue -= diff;
      switch (currentMode) {
        case RMODE:
          rValue = curValue;
          break;
        case GMODE:
          gValue = curValue;
          break;
        case BMODE:
          bValue = curValue;
          break;
        case AMODE:
          int diff = curValue - prevValue;
          rValue = constrain(rValue + diff, 0, 1023);
          gValue = constrain(gValue + diff, 0, 1023);
          bValue = constrain(bValue + diff, 0, 1023);
          break;
      }
      prevValue = readValue;
      showCurrent();
    }
  }
}

void showCurrent() {
  analogWrite(RED, map(rValue, 0, 1023, 0, 255));
  analogWrite(GREEN, map(gValue, 0, 1023, 0, 255));
  analogWrite(BLUE, map(bValue, 0, 1023, 0, 255));
}
