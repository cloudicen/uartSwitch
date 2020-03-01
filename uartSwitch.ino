//all input pin triggers while LOW!!!! and output HIGH!!!!

//#define SELF_RECOVERY  //uncomment this if you are using a self recovery switch

#define PIN_COUNT 8     //define the quantity of pins that used for IO
#define BAUD 115200 //baud rate
/*
 * now you need to define the pin that used for input and out put
 * make sure the quantity of input pins and output pins are same, and also qual to 'pinCount' definded above!
 * this firmware support up to 8-way switch (8 pins for input and 8 pins for output)
 * because arduino nano only have 19 pins for io, and uart port need 2 pins to work. Also, states of 8 pins can be easily presented using 8bits(char or uint8_t) and esay for transmitting.
 */
uint8_t inputPinList[] = {2, 3, 4, 5, 6, 7, 8, 9};    //maximum pin using in arduino uno/nano/pro mini/mini
uint8_t outputPinList[] = {10,11,12,13,14,15,16,17};  //maximum pin using in arduino uno/nano/pro mini/mini

//const uint8_t inputPinList[] = {0, 4, 5,10}; //maximum pin using in esp8266-12e nodemcu
//const uint8_t outputPinList[] = {15, 13, 12, 14};//maximum pin using in esp8266-12e nodemcu


//========code below no need to change======//

//commands defination
#define QUERY_COMMAND 0x18
#define SET_COMMAND 0x29
#define RESULT_COMMAND 0x33

uint8_t state = 0;
uint8_t preState = 0;

void initialize();
void readState();
void writeState();
void handleSerial();


void setup() {
  initialize();
}

void loop() {
  readState();
  handleSerial();
  writeState();
}

void initialize()
{
  Serial.begin(BAUD);
  for(int i = 0;i < PIN_COUNT;i++)
  {
    pinMode(inputPinList[i],INPUT_PULLUP);
    pinMode(outputPinList[i], OUTPUT);
    preState += 1;
    if(i != PIN_COUNT - 1)
    {
      preState = preState << 1;
    }
  }
}

void readState()
{
  uint8_t nowState = 0;
  for (int i = 0; i < PIN_COUNT; i++)
  {
    nowState += digitalRead(inputPinList[i]);
    if(i != PIN_COUNT-1)
    {
      nowState = nowState << 1; 
    }
  }
  #ifdef SELF_RECOVERY
  {
    if(preState != nowState)
    {
      state = state ^ (~nowState);
    }
  }
  #else
  {
    state = state ^ (preState ^ nowState);
  }
  #endif
  preState = nowState;
  delay(15);//消除抖动
}

void writeState()
{
  uint8_t stateToSet = state;
  for (int i = PIN_COUNT - 1; i >= 0; i--)
  {
    digitalWrite(outputPinList[i], stateToSet % 2);
    stateToSet = stateToSet >> 1;
  }
}

void handleSerial()
{
  uint8_t cmd;
  if (Serial.available())
  {
    cmd = Serial.read();
  }
  switch (cmd)
  {
    case QUERY_COMMAND:
      {
        Serial.write(RESULT_COMMAND);
        Serial.write(state);
        break;
      }
    case SET_COMMAND:
      {
        delay(10);//wait for message
        if(Serial.available())
        {
          state = Serial.read(); 
        }
        break;
      }
      Serial.flush();
  }
}
