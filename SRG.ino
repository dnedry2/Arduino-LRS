int latchPin = 8;
int clockPin = 12;
int dataPin = 11;

byte initialFill = 0;
bool buttonStates[] = { false, false, false, false };
unsigned short LRS = 0;

void setup() {
  pinMode(latchPin, OUTPUT);
  Serial.begin(9600);

  pinMode(2, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
}

void loop() {
  getFill();
  displayFill();
  getLRS();

  transmit(((byte*)&LRS));
}

void getLRS() {
  byte fill = initialFill;
  LRS = 0;

  Serial.println("start");
  
  for (int i = 0; i < 16; i++) {
    byte next = ((fill >> 1) & 1) ^ ((fill >> 3) & 1); //get next bit from taps

    if ((fill >> 1) & 1 == 1)
      LRS |= 1 << i;

    fill <<= 1;
    
    if (next == 1)
      fill |= 1 << 0;
  }
  
}

void getFill() {
  bool change = false;
  
  if (digitalRead(2) == HIGH) {
    buttonStates[0] = !buttonStates[0];
    change = true;
    delay(150);
  }
  if (digitalRead(4) == HIGH) {
    buttonStates[1] = !buttonStates[1];
    change = true;
    delay(150);
  }
  if (digitalRead(7) == HIGH) {
    buttonStates[2] = !buttonStates[2];
    change = true;
    delay(150);
  }
  if (digitalRead(10) == HIGH) {
    buttonStates[3] = !buttonStates[3];
    change = true;
    delay(150);
  }

  if (change) {
    initialFill = 0;
    initialFill |= (buttonStates[0]? (byte)1 : (byte)0) << 0;
    initialFill |= (buttonStates[1]? (byte)1 : (byte)0) << 1;
    initialFill |= (buttonStates[2]? (byte)1 : (byte)0) << 2;
    initialFill |= (buttonStates[3]? (byte)1 : (byte)0) << 3;
  }
}
void displayFill() {
  if ((initialFill >> 0) & 1 == 1) pinMode(3, OUTPUT);
  else pinMode(3, INPUT);

  if ((initialFill >> 1) & 1 == 1) pinMode(5, OUTPUT);
  else pinMode(5, INPUT);

  if ((initialFill >> 2) & 1 == 1)pinMode(6, OUTPUT);
  else pinMode(6, INPUT);
  
  if ((initialFill >> 3) & 1 == 1) pinMode(9, OUTPUT);
  else pinMode(9, INPUT);
}





void transmit(byte* bytes) {
    digitalWrite(latchPin, 0);
    
    shiftOut(dataPin, clockPin, bytes[1]); 
    shiftOut(dataPin, clockPin, bytes[0]);
    
    digitalWrite(latchPin, 1);
}


void shiftOut(int myDataPin, int myClockPin, byte myDataOut) {
  int i=0;
  int pinState;

  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, OUTPUT);

  digitalWrite(myDataPin, 0);
  digitalWrite(myClockPin, 0);

  for (i=7; i>=0; i--)  {
    digitalWrite(myClockPin, 0);

    if ( myDataOut & (1<<i) ) {
      pinState= 1;
    }
    else {  
      pinState= 0;
    }

    digitalWrite(myDataPin, pinState);
    digitalWrite(myClockPin, 1);
    digitalWrite(myDataPin, 0);
  }

  digitalWrite(myClockPin, 0);
}
