#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LCDS 2

struct things{
  const int pinNumber;
  const char type;
  int cur;
  int desired;
  const char message1[17];
  const char message2[17]; //the lcds have two rows of 16 characters (+'\0')
};

struct things FILLER = {0, '\0', LOW, LOW, "", ""};
/*struct things WhiteButton = {2, 'b', LOW, HIGH, "Push White", ""};
struct things Covered1 = {3, 's', LOW, HIGH, "Flip Covered1", ""};
struct things Covered2 = {4, 's', LOW, HIGH, "Flip Covered2", ""};
struct things RedSwitch = {5, 's', LOW, HIGH, "Flip Red", ""};
struct things BlueSwitch = {6, 's', LOW, HIGH, "Flip Blue", ""};
struct things BlueButton  = {7, 'b', LOW, HIGH, "Push Blue", ""};
struct things RedButton = {8, 'b', LOW, HIGH, "Push Red", ""};*/
struct things Pot = {11, 'p', 0, 2, "Slide to", ""}; //NOTE: not its actual pin
struct things South = {2, 'b', LOW, HIGH, "South", ""};
struct things West = {3, 'b', LOW, HIGH, "West", ""};
struct things North = {4, 'b', LOW, HIGH, "North", ""};
struct things East = {5, 'b', LOW, HIGH, "East", ""};
//The pin is just a filler. It's the same as its index in everything[]
struct things NE = {6, '2', LOW, 0, "Northeast", ""};
struct things NW = {7, '2', LOW, 1, "Northwest", ""};
struct things SouthEast = {8, '2', LOW, 2, "Southeast", ""};
struct things SW = {9, '2', LOW, 3, "Southwest", ""};
struct things WhiteButton = {10, 'b', LOW, HIGH, "White", ""};

//make sure array index matches pinNumber
const int length = 12;
struct things everything[] = {FILLER, FILLER, South, West, 
  North, East, NE, NW, SouthEast, SW, WhiteButton, Pot};
//struct things everything[] = {FILLER, FILLER, WhiteButton, Covered1, 
  //Covered2, RedSwitch, BlueSwitch, BlueButton, RedButton, Pot}; 

LiquidCrystal_I2C mylcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 
LiquidCrystal_I2C yourlcd(0x26, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
LiquidCrystal_I2C hislcd(0x25, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
LiquidCrystal_I2C herlcd(0x24, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

int lcdVals[] = {0, 0, 0, 0};
LiquidCrystal_I2C lcds[] = {mylcd, yourlcd, hislcd, herlcd};

long prev[] = {0, 0, 0, 0};
long interval[] = {2000, 2000, 2000, 2000};
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

void diag(int screen, int dir)
{
  Serial.print(dir);
  switch(dir){
    case 0://NE
      if(digitalRead(North.pinNumber) && digitalRead(East.pinNumber)){
        lcds[screen].clear();
        lcdVals[screen]=0;
      }
      break;
     case 1: //NW
       if(digitalRead(North.pinNumber) && digitalRead(West.pinNumber)){
         lcds[screen].clear();
         lcdVals[screen] = 0;
       }
       break;
     case 2://SE
       Serial.println("SE");
       if(digitalRead(South.pinNumber) && digitalRead(East.pinNumber)){
         lcds[screen].clear();
         lcdVals[screen] = 0;
         Serial.println("done");
       }
       break;
     case 3://SW
       if(digitalRead(South.pinNumber) && digitalRead(West.pinNumber)){
         lcds[screen] .clear();
         lcdVals[screen] = 0;
       }
       break;
  }
}
     
    

void pot(int screen)
{
  lcds[screen].clear();
  lcds[screen].setCursor(0,0);
  lcds[screen].write(Pot.message1);
  delay(10);
  int val = random(0,3);
  lcds[screen].setCursor(0,1);
  char t[1];
  itoa(val, t, 10);
  lcds[screen].write(t);
  delay(10);
  Pot.desired=val;
}

void checkPot(int screen)
{
  int pos;
  delay(10);
  Serial.println("wtf?");
  int val = analogRead(0);
  Serial.println("wtf?");
  delay(10);
  if(val < 20){
    pos = 0;
  } else if(815<val && val<920){ //arduino doesn't seem to like combined inequalities
    pos = 1;
  } else if(1000<val){
    pos = 2;
  } else{
    pos = 3;
  }
  Serial.println("here");
  if(pos == Pot.desired){
    Serial.println("here1");
    lcds[screen].clear();
    lcdVals[screen] = 0;
    delay(10);
    //I need to add the time change code
    //I should probably just put it into a function
  }
}
    

void checkButtons(unsigned long cur)
{
  for(int i=0; i<LCDS; ++i){
    if(cur-prev[i]>interval[i]){
      interval[i] = newint();
      if(!(lcdVals[i])){
        prev[i] = cur;
        int com = random(2, length);
        while(!everything[com].pinNumber){
          com = random(2, length); //There has to be a better way to do this
          Serial.println("random");
        }
        //refactor this into a function
        if(everything[com].type == 'p') {
          lcdVals[i] = com;
          Serial.println(com);
          pot(i);
        } else {
          lcdVals[i] = everything[com].pinNumber;
          Serial.println(com);
          lcds[i].clear();
          lcds[i].setCursor(0,0);
          lcds[i].write(everything[com].message1);
          delay(1);
          lcds[i].setCursor(0, 1);
          lcds[i].write(everything[com].message2);
          delay(1);
        }
      } else {
        Play = false;
        Serial.println(lcdVals[i]);
        Serial.println(interval[i]);
        Serial.println(cur);
        Serial.println(prev[i]);
      }
    }
    if(lcdVals[i]){
      //struct things *mything = &everything[lcdVals[i]]; //to save typing
      Serial.println(everything[lcdVals[i]].type);
      //Serial.println("help");
      if(everything[lcdVals[i]].type == 'p'){
        checkPot(i);
        Serial.println("Here2");
      } else if(everything[lcdVals[i]].type == '2'){
        Serial.println("2");
        diag(i, everything[lcdVals[i]].desired);
      }else if(digitalRead(lcdVals[i]) == everything[lcdVals[i]].desired){
        delay(1);
        lcds[i].clear();
        Serial.println(lcdVals[i]);
        lcdVals[i] = 0;
        int remaining = (prev[i]+interval[i]-cur);
        prev[i] = prev[i] - remaining*.5; //Give half the remaining time as break
        if(everything[lcdVals[i]].type == 's'){
          everything[lcdVals[i]].cur = everything[lcdVals[i]].desired;
          everything[lcdVals[i]].desired = !(everything[lcdVals[i]].desired);
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
  Serial.println("lost");
  delay(10000);
}
  
void setup()
{
  //for(int i = 0; i<length; ++i){
    //if(everything[i].pinNumber) pinMode(everything[i].pinNumber, INPUT);
  //}
  //pinMode(A0, INPUT); //just double checking
  for(int i = 0; i<LCDS; ++i){
    lcds[i].begin(16, 2);
  }
  Serial.begin(9600);
  Serial.println("Start");
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
