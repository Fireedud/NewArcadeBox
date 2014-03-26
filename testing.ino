 //Blink without delay

#define ledPin1 9
#define ledPin2 10
#define ledPin3 11
#define ledPin4 12
#define Button1 7
#define Button2 6

const int numButs = 2;
const int numLights = 2;
int yourledPins[] = {ledPin1, ledPin2};
int myledPins[] = {ledPin3, ledPin4};
int Buttons[] = {Button1, Button2};
int yourledStates[] = {HIGH, HIGH};
int myledStates[] = {HIGH, HIGH};

long prev[] = {0, 0};
long interval[] = {1000, 1000};

boolean Play = true;

void checkButtons(unsigned long cur)
{
  for(int i=0; i<numButs; ++i){
    if(cur-prev[i]>interval[i]){
      interval[i] = random(2000, 5000);
      if(!(myledStates[i])&&!(yourledStates[i])){
        int x = random(0,2);
        if(x){
          myledStates[i] = HIGH;
        }else{
          yourledStates[i]=HIGH;
        }
      }else{
        Play = false;
      }
      prev[i] = cur;
    }
    if(digitalRead(Buttons[i])){
      delay(1); //for stability
      myledStates[i] = LOW;
      yourledStates[i] = LOW;
    }
  }
}

void writeLights()
{
  for(int i=0; i<numLights; ++i){
    digitalWrite(myledPins[i], myledStates[i]);
    digitalWrite(yourledPins[i], yourledStates[i]);
  }
}

void lose()
{
    for(int i = 0; i<numButs; ++i){
      myledStates[i] = HIGH;
      yourledStates[i] = HIGH;
    }
    while(true){
      writeLights();
      for(int i = 0; i<numButs; ++i){
        myledStates[i] = !(myledStates[i]);
        yourledStates[i] = !(yourledStates[i]);
      }
      delay(300);
    }
}
  
void setup()
{
  for(int i = 0; i<numButs; ++i){
    pinMode(myledPins[i], OUTPUT);
    pinMode(yourledPins[i], OUTPUT);
    pinMode(Buttons[i], INPUT);
    writeLights();
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
