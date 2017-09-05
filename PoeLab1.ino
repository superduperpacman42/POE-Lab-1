int red = 11;
int yellow = 12;
int green = 13;
int button = 8;
int state = 0;
bool pressed = false;
long t0 = 0;
int pos = 0;
// Messages = SOS and HELLO WORLD
// 0 = space, 1 = dot, 2 = dash
const byte MESSAGE1[] = {1,1,1,0,2,2,2,0,1,1,1};
const byte MESSAGE2[] = {1,1,1,1,0,1,0,1,2,1,1,0,1,2,1,1,0,2,2,2,0,0,0,1,2,2,0,2,2,2,0,1,2,1,0,1,2,1,1,0,2,1,1};
const int DOT = 500;
const int SPACE = 750;
const int DASH = 1500;
const int GAP = 100;
const int END = 3000;
const int CYCLE_TIME = 200;

void setup() {
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(button, INPUT);
}

void loop() {
  // Check for button press
  if(!digitalRead(button)) {
    pressed = false;
  } else if(!pressed) {
    pressed = true;
    state++;
    state = state%8;
    t0 = millis();
    pos = 0;
    setLEDs(LOW,LOW,LOW);
  }
  // State 0: All off
  if(state==0) {
    setLEDs(LOW,LOW,LOW);
  }
  // State 1: Green
  if(state==1) {
    setLEDs(LOW,LOW,HIGH);
  }
  // State 2: Yellow
  if(state==2) {
    setLEDs(LOW,HIGH,LOW);
  }
  // State 3: Red
  if(state==3) {
    setLEDs(HIGH,LOW,LOW);
  }
  // State 4: Cycle
  if(state==4) {
    switch(int((millis()-t0)/CYCLE_TIME)%3) {
      case 0: setLEDs(LOW,LOW,HIGH); break;
      case 1: setLEDs(LOW,HIGH,LOW); break;
      case 2: setLEDs(HIGH,LOW,LOW); break;
    }
  }
  // State 5: Binary counter
  if(state==5) {
    int secs = int((millis()-t0)/1000)%8;
    setLEDs((secs/4)%2*HIGH,(secs/2)%2*HIGH,secs%2*HIGH);
  }
  // State 6: Morse code message
  if(state>=6) {
    byte *message;
    int len = 0;
    switch(state){
      case 6: message = MESSAGE1; len = sizeof(MESSAGE1); break;
      case 7: message = MESSAGE2; len = sizeof(MESSAGE2); break;
    }
    if(int((millis()-t0)/(DASH+GAP))>0) {
      if(pos>len) { // End message
        t0 = millis()+END;
        setLEDs(LOW,LOW,LOW);
        pos = -1;
      }
      else if(message[pos]==1) { // Dot
        t0 = millis()-(DASH-DOT);
        setLEDs(HIGH,HIGH,HIGH);
      }
      else if(message[pos]==2) { // Dash
        t0 = millis();
        setLEDs(HIGH,HIGH,HIGH);
      } else { // Space
        t0 = millis()-(DASH-SPACE);
        setLEDs(LOW,LOW,LOW);
      }
      pos++;
    }
    if(int((millis()-t0)/DASH)) { // Short spaces
      setLEDs(LOW,LOW,LOW);
    }
  }
  delay(5);
}

void setLEDs(int r, int y, int g) {
  digitalWrite(red, r);
  digitalWrite(yellow, y);
  digitalWrite(green, g);
}
