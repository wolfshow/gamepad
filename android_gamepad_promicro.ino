  /*
 * wolfshow's usb_gamepad for sparkfun pro micro v1.0
 * 
 */

#include <Joystick.h>

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD, 14, 1, 
  true, true, true, false, false, true, false, false, false, false, false);

class CButton {
  public:
  int pin = NULL;
  int lastState = 0;
  
  CButton(int p) {
    pin = p;
  }
};

CButton Buttons[10] ={0,1,2,3,4,5,6,7,8,9};
CButton Dpad[4] = {10,14,15,16};

int lastDpadState[4] = {0,0,0,0};
int JoyHat = 0;

void setup() {
  for(int i=0 ; i< 10 ;i++) {
    pinMode(Buttons[i].pin, INPUT_PULLUP);
  }

  for(int i=0 ; i< 4 ;i++) {
    pinMode(Dpad[i].pin, INPUT_PULLUP);
  }

  Joystick.begin();
  
  Joystick.setXAxisRange(-512, 512);
  Joystick.setYAxisRange(-512, 512);
  

  Joystick.setZAxisRange(-512, 512);
  Joystick.setRzAxisRange(-512, 512);

  Joystick.setHatSwitch(0,-1);

}

void JButtonStates() {
  
  Joystick.setXAxis(analogRead(A0) - 512);
  Joystick.setYAxis(512 - analogRead(A1));
  
  
  Joystick.setZAxis(512 - analogRead(A2));
  Joystick.setRzAxis(analogRead(A3) - 512);
  
  
  for (int i = 0; i < 10; i++) {
      int currentState = !digitalRead(Buttons[i].pin);
      
      if (currentState != Buttons[i].lastState) {
        Joystick.setButton(i, currentState);
        Buttons[i].lastState = currentState;
      }   
  }  
  
  bool valueChanged = false;
  
  for (int i = 0; i < 4; i++)
  {
    int currentButtonState = !digitalRead(Dpad[i].pin);
    if (currentButtonState != lastDpadState[i])
    {
      valueChanged = true;
      lastDpadState[i] = currentButtonState;
    }
  }

  if(valueChanged)
  {
    int JoyHat = 0;
    if ( lastDpadState[0] == 1 ) JoyHat += 2;
    if ( lastDpadState[2] == 1 ) JoyHat += 3;
    if ( lastDpadState[1] == 1 ) JoyHat += 6;
    if ( lastDpadState[3] == 1 ) JoyHat += 12;
  
    switch(JoyHat) {
      case 2: Joystick.setHatSwitch(0,0); break; // up
      case 3: Joystick.setHatSwitch(0,180); break; // down
      case 6: Joystick.setHatSwitch(0,270); break; // left
      case 8: Joystick.setHatSwitch(0,315); break; // up-left
      case 9: Joystick.setHatSwitch(0,225); break; // down-left
      case 12: Joystick.setHatSwitch(0,90); break; // right
      case 14: Joystick.setHatSwitch(0,45); break; // up-right
      case 15: Joystick.setHatSwitch(0,135); break; // down-right
      default: Joystick.setHatSwitch(0,-1);
    }
  }

}

void loop() {
  JoyHat = 0;
  JButtonStates();
  delay(10);
}
