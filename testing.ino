#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LCDS 3

struct things{
  const int pinNumber;
  const char type;
  int cur;
  int desired;
  const char message1[17];
  const char message2[17]; //the lcds have two rows of 16 characters (+'\0')
};

struct things FILLER = {0, '\0', LOW, LOW, "", ""};
struct things WhiteButton = {2, 'b', LOW, HIGH, "Push White", ""};
struct things Covered1 = {3, 's', LOW, HIGH, "Flip Covered1", ""};
struct things Covered2 = {4, 's', LOW, HIGH, "Flip Covered2", ""};
struct things RedSwitch = {5, 's', LOW, HIGH, "Flip Red", ""};
struct things BlueSwitch = {6, 's', LOW, HIGH, "Flip Blue", ""};
struct things BlueButton  = {7, 'b', LOW, HIGH, "Push Blue", ""};
struct things RedButton = {8, 'b', LOW, HIGH, "Push Red", ""};

const int NumThings = 7;

//make sure array index matches pinNumber
const int length = 9;
struct things everything[] = {FILLER, FILLER, WhiteButton, Covered1, 
  Covered2, RedSwitch, BlueSwitch, BlueButton, RedButton}; 

LiquidCrystal_I2C mylcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 
LiquidCrystal_I2C yourlcd(0x26, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
LiquidCrystal_I2C hislcd(0x25, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
LiquidCrystal_I2C herlcd(0x24, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

int lcdVals[] = {0, 0, 0, 0};
LiquidCrystal_I2C lcds[] = {mylcd, yourlcd, hislcd, herlcd};

long prev[] = {0, 0, 0, 0};
long interval[] = {1000, 1000, 1000, 1000};
long minint = 2000;
long maxint = 5000;

boolean Play = true;

int newint()
{
  int interval = random(minint, maxint);
  minint *= .99;
  maxint *= .99;
  return interval;
}

void checkButtons(unsigned long cur)
{
  for(int i=0; i<LCDS; ++i){
    if(cur-prev[i]>interval[i]){
      interval[i] = newint();
      if(!(lcdVals[i])){
        int com = random(2, length);
        while(!everything[com].pinNumber){
          com = random(2, length); //There has to be a better way to do this
        }
        lcdVals[i] = everything[com].pinNumber;
        lcds[i].clear();
        lcds[i].setCursor(0,0);
        lcds[i].write(everything[com].message1);
        lcds[i].setCursor(0, 1);
        lcds[i].write(everything[com].message2);
        prev[i] = cur;
        Serial.println(com);
      } else {
        Play = false;
        Serial.println(prev[i]);
        Serial.println(cur);
      }
    }
    if(lcdVals[i]){
      struct things *mything = &everything[lcdVals[i]];
      if(digitalRead(lcdVals[i]) == mything->desired){
        lcds[i].clear();
        lcdVals[i] = 0;
        int remaining = (prev[i]+interval[i]-cur);
        prev[i] = prev[i] - remaining*.5; //Give half the remaining time as break
        if(mything->type == 's'){
          mything->cur = mything->desired;
          mything->desired = !(mything->desired);
        }
      }
    }
  }
}

/*void writeLCD()
{
  for(int i=0; i<LCDS; ++i){
    if(has_changed[i]){
      lcds[i].clear();
      switch(lcdVals[i]){
        case -1:
          lcds[i].setCursor(0,0);
          lcds[i].write("Turn the Switch");
          lcds[i].setCursor(0,1);
          if(our_switch.desired){
            lcds[i].write("ON");
          }else{
            lcds[i].write("OFF");
          }
          break;
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
}*/

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
  for(int i = 0; i<length; ++i){
    if(everything[i].pinNumber) pinMode(everything[i].pinNumber, INPUT);
  }
  for(int i = 0; i<LCDS; ++i){
    lcds[i].begin(16, 2);
  }
  Serial.begin(9600);
}

void loop()
{
  if(Play){
   unsigned long cur = millis();
   checkButtons(cur);
   delay(1); //for stability when reading buttons
   //writeLCD();
   delay(15);
  } else {
    lose();
  }
}
