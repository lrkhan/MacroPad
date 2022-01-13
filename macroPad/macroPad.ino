//Libraries
#include <HID-Project.h>
#include <Encoder.h>

//Control Mode
char mode = 'm';

String key = "";

//Encoder Setup
Encoder knobLeft(A0, A1);
Encoder knobRight(A2, A3);
long positionLeft  = -999;
long positionRight = -999; \
byte lButton = 14;
byte rButton = 16;

//MacroPad Setup
int ROWS = 5;
int COLS = 4;
int row[] = {6, 7, 8, 9, 10};
int col[] = {5, 4, 3, 2};

bool isMac(char key) {
  if (key == 'm') {
    return true;
  } else {
    return false;
  }
}

String getKey() {
  for (int i = 0; i < COLS; i++) {
    digitalWrite(col[i], HIGH);

    for (int j = 0; j < ROWS; j++) {
      if (digitalRead(row[j])) {
        Serial.print(i);
        Serial.print(" ");
        Serial.print(j);
        Serial.print(": ");
        Serial.print((String(i) + String(j)).toInt());
        Serial.print("\n");
        delay(250);
        return (String(i) + String(j));
      }
    }

    digitalWrite(col[i], LOW);
  }
  return "";
}

//Actions for Keys
void keyAction(String key) {
  switch (key.toInt()) {
    case 0:
      // launch FireFox - open youtube and gmail
      if (isMac(mode)) {
        Keyboard.press(KEY_LEFT_GUI);
        Keyboard.press(32); //space
        Keyboard.releaseAll();
        Keyboard.println("fire");
        Keyboard.press(KEY_RETURN);
        delay(30);
        Keyboard.release(KEY_RETURN);
        delay(200);

        Keyboard.press(KEY_LEFT_GUI);
        Keyboard.press('l'); //spacefi
        Keyboard.releaseAll();
        Keyboard.println("youtube.com");
        Keyboard.press(KEY_RETURN);
        delay(30);
        Keyboard.release(KEY_RETURN);
        delay(200);

        Keyboard.press(KEY_LEFT_GUI);
        Keyboard.press('t'); //space
        Keyboard.releaseAll();
        Keyboard.println("gmail.com");
        Keyboard.press(KEY_RETURN);
        delay(30);
        Keyboard.release(KEY_RETURN);
        delay(200);

        break;
      }

      Keyboard.write(KEY_LEFT_GUI);
      delay(100);
      Keyboard.releaseAll();
      Keyboard.println("fire");
      Keyboard.press(KEY_ENTER);
      delay(30);
      Keyboard.release(KEY_ENTER);
      delay(200);

      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('l'); //spacefi
      Keyboard.releaseAll();
      Keyboard.println("youtube.com");
      Keyboard.press(KEY_RETURN);
      delay(30);
      Keyboard.release(KEY_RETURN);
      delay(200);

      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('t'); //space
      Keyboard.releaseAll();
      Keyboard.println("gmail.com");
      Keyboard.press(KEY_RETURN);
      delay(30);
      Keyboard.release(KEY_RETURN);
      delay(200);

      break;
    case 1:
      // statements
      break;
    case 2:
      // Zoom Mute Mic
      if (isMac(mode)) {
         Keyboard.press(KEY_LEFT_GUI);
         delay(50);
         Keyboard.press(KEY_LEFT_SHIFT);
         delay(50);
         Keyboard.press('a');
         delay(50);
         Keyboard.releaseAll();

         break;
        }
      break;
    case 3:
      // F18
      Keyboard.write(245);
      break;
    case 4:
      mode = 'w';
      Serial.println(mode);
      break;
    case 10:
      // statements
      break;
    case 11:
      // statements
      break;
    case 12:
      // F19
      Keyboard.write(246);
      break;
    case 13:
      // F20
      Keyboard.write(247);
      break;
    case 14:
      mode = 'm';
      Serial.println(mode);
      break;
    case 20:
      // statements
      break;
    case 21:
      // statements
      break;
    case 22:
      // F21
      Keyboard.write(248);
      break;
    case 23:
      // F22
      Keyboard.write(249);
      break;
    case 30:
      // Zoom Mute Video
      if (isMac(mode)) {
         Keyboard.press(KEY_LEFT_GUI);
         delay(50);
         Keyboard.press(KEY_LEFT_SHIFT);
         delay(50);
         Keyboard.press('v');
         delay(50);
         Keyboard.releaseAll();

         break;
        }
      break;
    case 31:
      // statements
      break;
    case 32:
      // F23
      Keyboard.write(250);
      break;
    case 33:
      // F24
      Keyboard.write(251);
      break;
    default:
      //shouldn't reach here but this will go to a return;
      break;
  }
  return;
}

void encoderCommands() {
  long newLeft, newRight;
  newLeft = knobLeft.read();
  newRight = knobRight.read();
  if (newRight > positionRight + 3) {
    Consumer.write(MEDIA_VOL_DOWN);
    positionRight = newRight;
  } else if (newRight < positionRight - 3) {
    Consumer.write(MEDIA_VOL_UP);
    positionRight = newRight;
  }
  if (newLeft > positionLeft + 3) {
    if (isMac(mode)) {
      Keyboard.write('j'); // J - youtube 10 rewind
    } else {
      Keyboard.write(241); //Mic Gain down
    }
    positionLeft = newLeft;
  } else if (newLeft < positionLeft - 3) {
    if (isMac(mode)) {
      Keyboard.write('l'); // L - youtube 10 forward
    } else {
      Keyboard.write(242); // Mic Gain Up
    }
    positionLeft = newLeft;
  }

  // if a character is sent from the serial monitor,
  // reset both back to zero.
  if (digitalRead(rButton) == LOW) {
    knobRight.write(0);
    Consumer.write(MEDIA_VOL_MUTE);
    delay(500);
  }
  if (digitalRead(lButton) == LOW) {
    if (isMac(mode)) {
      Keyboard.write(107); // K - youtube pause
    } else {
      Keyboard.write(243); // Cut Mic
    }
    delay(500);
  }

  return;
}

void setup() {
  // put your setup code here, to run once:

  key = "";

  // setting up cols as output
  for (byte i = 0; i < COLS; i++) {
    pinMode(col[i], OUTPUT);
  }
  // setting up rows as input
  for (byte i = 0; i < ROWS; i++) {
    pinMode(row[i], INPUT);
  }

  //Encoder In/Outputs
  Consumer.begin();
  Keyboard.begin();
  pinMode(14, OUTPUT); // Encoder button pin
  pinMode(lButton, INPUT_PULLUP); // Engoder button pin
  pinMode(rButton, INPUT_PULLUP); // Engoder button pin

  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  String key = getKey();

  encoderCommands();

  if (key != "") {
    Serial.println(key);
    keyAction(key);
    key = "";
  }
}
/*
 * 
 */
