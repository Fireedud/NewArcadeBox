 //Blink without delay

#define ledPin1 13
#define ledPin2 12
#define Button1 7
#define Button2 6

const int numButs = 2;
const int numLights = 2;
int ledPins[] = {ledPin1, ledPin2};
int Buttons[] = {Button1, Button2};
int ledStates[] = {HIGH, HIGH};

long prev[] = {0, 0};
long interval[] = {1000, 1000};

boolean Play = true;

void checkButtons(unsigned long cur)
{
  for(int i=0; i<numButs; ++i){
    if(cur-prev[i]>interval[i]){
      interval[i] = random(2000, 5000);
      if(!(ledStates[i])){
        ledStates[i] = !(ledStates[i]);
      }else{
        Play = false;
      }
      prev[i] = cur;
    }
    if(digitalRead(Buttons[i])){
      delay(1); //for stability
      ledStates[i] = LOW;
    }
  }
}

void writeLights()
{
  for(int i=0; i<numLights; ++i){
    digitalWrite(ledPins[i], ledStates[i]);
  }
}

void lose()
{
      for(int i = 0; i<numButs; ++i){
      ledStates[i] = HIGH;
    }
    while(true){
      for(int i = 0; i<numButs; ++i){
        digitalWrite(ledPins[i], ledStates[i]);
        ledStates[i] = !(ledStates[i]);
      }
      delay(100);
    }
}
  
void setup()
{
  for(int i = 0; i<numButs; ++i){
    pinMode(ledPins[i], OUTPUT);
    pinMode(Buttons[i], INPUT);
    digitalWrite(ledPins[i], ledStates[i]);
  } 
}

void loop()
{
  if(Play){
   unsigned long cur = millis();
   checkButtons(cur);
   delay(1); //for stability when reading buttons
   writeLights();
  } else {
    lose();
  }
}
