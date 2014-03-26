//Blink without delay

#define ledPin1 13
#define ledPin2 12
#define Button1 7
#define Button2 6

int ledPins[] = {ledPin1, ledPin2};
int Buttons[] = {Button1, Button2};
int ledStates[] = {HIGH, HIGH};

long prev[] = {0, 0};
long interval[] = {1000, 1000};

boolean Play = true;

void setup()
{
  for(int i = 0; i<2; ++i){
    pinMode(ledPins[i], OUTPUT);
    pinMode(Buttons[i], INPUT);
    digitalWrite(ledPins[i], ledStates[i]);
  } 
}

void loop()
{
  if(Play){
   unsigned long cur = millis();
   for(int i = 0; i < 2; ++i){
     if(cur-prev[i]>interval[i]){
       interval[i] = random(2000,5000);
       if(!(ledStates[i])){
         ledStates[i] = !(ledStates[i]);
       } else {
         Play = false;
       }
       prev[i] = cur;
     }
     if(digitalRead(Buttons[i])){
       ledStates[i] = LOW;
     }
     digitalWrite(ledPins[i], ledStates[i]);
   }
   delay(1);
  } else {
    for(int i = 0; i<2; ++i){
      ledStates[i] = HIGH;
    }
    while(true){
      for(int i = 0; i<2; ++i){
        digitalWrite(ledPins[i], ledStates[i]);
        ledStates[i] = !(ledStates[i]);
      }
      delay(100);
    }
  }
}
