#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LCDS 4
#define RedButton 2
#define WhiteButton 3
#define GreenButton 4
#define BlueButton 5

LiquidCrystal_I2C mylcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 
LiquidCrystal_I2C yourlcd(0x26, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
LiquidCrystal_I2C hislcd(0x25, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
LiquidCrystal_I2C herlcd(0x24, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

int lcdVals[] = {0, 0, 0, 0};
boolean has_changed[] = {false, false, false, false};
LiquidCrystal_I2C lcds[] = {mylcd, yourlcd, hislcd, herlcd};

const int NumButs = 4;
int Buts[] = {RedButton, WhiteButton, GreenButton, BlueButton};

long prev[] = {0, 0, 0, 0};
long interval[] = {1000, 1000, 1000, 1000};
long minint = 1000;
long maxint = 5000;

boolean Play = true;

void checkButtons(unsigned long cur)
{
  for(int i=0; i<LCDS; ++i){
    if(cur-prev[i]>interval[i]){
      interval[i] = random(minint, maxint);
      minint *= .98;
      maxint *= .98;
      if(!(lcdVals[i])){
        lcdVals[i] = random(2,6);
        has_changed[i]=true;
      }else{
        Play = false;
      }
      prev[i] = cur;
    } else {
      has_changed[i]=false;
    }
  }
  for(int i = 0; i<NumButs; ++i){
    if(digitalRead(Buts[i])){
      delay(1); //for stability
      for(int j =0; j<LCDS; ++j){
        if(lcdVals[j] == (i+2)) { 
          lcds[j].clear();
          lcdVals[j] = 0;
        }
      }
    }
  }
}

void writeLCD()
{
  for(int i=0; i<LCDS; ++i){
    if(has_changed[i]){
      lcds[i].clear();
      switch(lcdVals[i]){
        case 2:
          lcds[i].setCursor(2, 0);
          lcds[i].write("Push the Red");
          lcds[i].setCursor(5, 1);
          lcds[i].write("Button");
          break;
        case 3:
          lcds[i].setCursor(1, 0);
          lcds[i].write("Push the White");
          lcds[i].setCursor(5, 1);
          lcds[i].write("Button");
          break;
        case 4:
          lcds[i].setCursor(1, 0);
          lcds[i].write("Push the Green");
          lcds[i].setCursor(5, 1);
          lcds[i].write("Button");
          break;
        case 5:
          lcds[i].setCursor(1, 0);
          lcds[i].write("Push the Blue");
          lcds[i].setCursor(5, 1);
          lcds[i].write("Button");
          break;
      }
    }
  }
}

void lose()
{
  for(int i = 0; i<LCDS; ++i){
    lcds[i].clear();
    lcds[i].setCursor(0,0);
    lcds[i].write("You Lose!");
  }
  delay(10000);
}
  
void setup()
{
  for(int i = 0; i<NumButs; ++i){
    pinMode(Buts[i], INPUT);
  }
  for(int i = 0; i<LCDS; ++i){
    lcds[i].begin(16, 2);
  }
  
}

void loop()
{
  if(Play){
   unsigned long cur = millis();
   checkButtons(cur);
   delay(1); //for stability when reading buttons
   writeLCD();
   delay(15);
  } else {
    lose();
  }
}
