 //Blink without delay

#define myledPin1 8
#define myledPin2 11
#define myledPin3 12
#define yourledPin1 5
#define yourledPin2 9
#define yourledPin3 10
#define herledPin1 4
#define herledPin2 3
#define herledPin3 2
#define Button1 13
#define Button2 7
#define Button3 6

const int numButs = 3;
const int numLights = 3;
int yourledPins[] = {yourledPin1, yourledPin2, yourledPin3};
int myledPins[] = {myledPin1, myledPin2, myledPin3};
int herledPins[] = {herledPin1, herledPin2, herledPin3};
int Buttons[] = {Button1, Button2, Button3};
int yourledStates[] = {HIGH, HIGH, HIGH};
int myledStates[] = {HIGH, HIGH, HIGH};
int herledStates[] = {HIGH, HIGH, HIGH};

long prev[] = {0, 0, 0};
long interval[] = {1000, 1000, 1000};
long minint = 1000;
long maxint = 5000;

boolean Play = true;

void checkButtons(unsigned long cur)
{
  for(int i=0; i<numButs; ++i){
    if(cur-prev[i]>interval[i]){
      interval[i] = random(minint, maxint);
      minint *= .95;
      maxint *= .95;
      if(!(myledStates[i])&&!(yourledStates[i])&&!(herledStates[i])){
        int x = random(0,3);
        if(x==0){
          myledStates[i] = HIGH;
        }else if(x==1){
          yourledStates[i]=HIGH;
        } else if(x==2){
          herledStates[i]=HIGH;
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
      herledStates[i] = LOW;
    }
  }
}

void writeLights()
{
  for(int i=0; i<numLights; ++i){
    digitalWrite(myledPins[i], myledStates[i]);
    digitalWrite(yourledPins[i], yourledStates[i]);
    digitalWrite(herledPins[i], herledStates[i]);
  }
}

void lose()
{
    for(int i = 0; i<numButs; ++i){
      myledStates[i] = HIGH;
      yourledStates[i] = HIGH;
      herledStates[i] = HIGH;
    }
    while(true){
      writeLights();
      for(int i = 0; i<numButs; ++i){
        myledStates[i] = !(myledStates[i]);
        yourledStates[i] = !(yourledStates[i]);
        herledStates[i] = !(herledStates[i]);      }
      delay(300);
    }
}
  
void setup()
{
  for(int i = 0; i<numButs; ++i){
    pinMode(myledPins[i], OUTPUT);
    pinMode(yourledPins[i], OUTPUT);
    pinMode(herledPins[i], OUTPUT);
    pinMode(Buttons[i], INPUT);
  } 
  writeLights();
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
