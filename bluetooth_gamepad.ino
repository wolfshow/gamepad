/*
 * wolfshow's bluetooth gamepad v1.0
 */

#include <BleGamepad.h> 

BleGamepad bleGamepad("BlueBoy Gamepad", "wolfshow", 100);

//class CButton {
//  public:
//  int pin = NULL;
//  int lastState = 0;
//  
//  CButton(int p) {
//    pin = p;
//  }
//};

// 0, 2, 4, 5, 12,13,14,15,16,17,18,19,21,22,23,25,26,27,32,33,34,35
int Buttons[12] ={26,27,4,5,12,13,14,15,16,17,18,19};
int Dpad[4] = {21,22,23,25};

int button[12]= {1,2,4,8,16,32,64,128,256,512,1024,2048};

//int previousButton1State = HIGH;

int lastButtonState[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
int lastDpadState[4] = {0,0,0,0};

int JoyHat = 0;

void setup() {
  //pinMode(2, INPUT_PULLUP);
  //Serial.begin(115200);
  for(int i=0 ; i< 12 ;i++) {
    pinMode(Buttons[i], INPUT_PULLUP);
  }

  for(int i=0 ; i< 4 ;i++) {
    pinMode(Dpad[i], INPUT_PULLUP);
  }
  
  
  bleGamepad.begin();
  bleGamepad.setAxes(0, 0, 0, 0, 0, 0, DPAD_CENTERED);
  
}

void JButtonStates() {

  int dpad_status = DPAD_CENTERED;
  bool valueChanged = false;
  
  for (int i = 0; i < 4; i++)
  {
    int currentButtonState = !digitalRead(Dpad[i]);
    //Serial.println(currentButtonState);
    if (currentButtonState != lastDpadState[i])
    {
      valueChanged = true;
      lastDpadState[i] = currentButtonState;
      //Serial.println(lastDpadState[i]);
    }
  }

  //if(valueChanged)
  {
    JoyHat = 0;
    
    if ( lastDpadState[0] == HIGH ) JoyHat += 2;
    if ( lastDpadState[2] == HIGH ) JoyHat += 3;
    if ( lastDpadState[1] == HIGH ) JoyHat += 6;
    if ( lastDpadState[3] == HIGH ) JoyHat += 12;

    
    //Serial.println(JoyHat);
    
    switch(JoyHat) {
      case 2: dpad_status = DPAD_UP; break; // up
      case 3: dpad_status = DPAD_DOWN; break; // down
      case 6: dpad_status = DPAD_LEFT; break; // left
      case 8: dpad_status = DPAD_UP_LEFT; break; // up-left
      case 9: dpad_status = DPAD_DOWN_LEFT; break; // down-left
      case 12: dpad_status = DPAD_RIGHT; break; // right
      case 14: dpad_status = DPAD_UP_RIGHT; break; // up-right
      case 15: dpad_status = DPAD_DOWN_RIGHT; break; // down-right
      default: dpad_status = DPAD_CENTERED;
    }
  }

  //Serial.println(dpad_status);

  int lx = analogRead(32);
  int ly = analogRead(33);
  int rx = analogRead(34);
  int ry = analogRead(35);
  
  //Serial.println(rx);
  lx = map(lx, 0, 4095, -127, 127);
  ly = map(ly, 0, 4095, 127, -127);
  rx = map(rx, 0, 4095, 127, -127);
  ry = map(ry, 0, 4095, -127, 127);
  //Serial.println(rx);

  lx = lx * 5 /2;
  ly = ly * 5 /2;

  if(lx > 127)
    lx = 127;
  if(lx < -127)
    lx = -127;

  if(ly > 127)
    ly = 127;
  if(ly < -127)
    ly = -127;
    
  
  if(abs(lx) < 20)
    lx = 0;
  if(abs(ly) < 20)
    ly = 0;
  if(abs(rx) < 20)
    rx = 0;
  if(abs(ry) < 20)
    ry = 0;


  bleGamepad.setAxes(lx, ly, rx, ry, 0, 0, dpad_status);
  //bleGamepad.setAxes(0, 0, 0, 0, 0, 0, DPAD_DOWN_RIGHT);
    
  for (int i = 0; i < 12; i++) {
      int currentState = digitalRead(Buttons[i]);
      
      if (currentState != lastButtonState[i]) {
        //Joystick.setButton(i, currentState);
        if(currentState == LOW)
          bleGamepad.press(button[i]);
        else
          bleGamepad.release(button[i]);
          
        lastButtonState[i] = currentState;
      }   
  }  
  
}


void loop() {
  if(bleGamepad.isConnected()) {
    JoyHat = 0;
    JButtonStates();
    delay(50);
  }
}
