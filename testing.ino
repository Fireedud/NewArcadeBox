#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>

#define LCDS 4

struct things{
  const int pinNumber;
  const char type;
  int cur;
  int desired;
  const char message1[17];
  const char message2[17]; //the lcds have two rows of 16 characters (+'\0')
};

//Note, I need to refactor the code to have the do_command
//It'll be easier to make switches say two things
//or have two buttons act as a switch

struct things FILLER = {0, '\0', LOW, LOW, "", ""};
struct things Lights = {38, 's', LOW, HIGH, "LIGHTS!!", ""};
struct things Camera = {39, 's', LOW, HIGH, "CAMERA!!!", ""};
struct things Action = {40, 's', LOW, HIGH, "ACTION!!!!", ""};
struct things Stop = {8, 'b', LOW, HIGH, "Stop", ""};
struct things Go = {6, 'b', LOW, HIGH, "Go", ""};
struct things Speed = {7, 'b', LOW, HIGH, "Speed Up", ""};
struct things Hammer = {9, 'h', LOW, HIGH, "", ""};
struct things DoorBell = {41, 'b', LOW, HIGH, "Ring the", "Door Bell"};
//struct things Key = { , 's', LOW, HIGH, "Unlock", "Next Level"};
//struct things Lever = { 2, 'l', LOW, 0, "", ""}; //the second value will be 0 or 1,
//this will be the value of the pin to be checked, because I need to check both.
//struct things PullKnob = { , 'b', LOW, HIGH, "Pull Yourself", "Together"};
//Fire is very special
//struct things Fire = { , 'f', LOW, HIGH, "", ""};
struct things Cannon = { 24, 'c', LOW, HIGH, "", ""};
struct things Piano = {29, 'b', LOW, HIGH, "Play the Piano", ""};
struct things LShield = { 34, 'b', LOW, HIGH, "Shield Port", ""};
struct things RShield = {31, 'b', LOW, HIGH, "Shield Starboard", ""};
struct things LCover = {35, 'b', LOW, HIGH, "Provide Cover", "Port Flank"};
struct things RCover = {33, 'b', LOW, HIGH, "Provide Cover", "Starboard Flank"};
struct things BCover = {30 , 'b', LOW, HIGH, "Protect the", "Aft"};
struct things Trans = {36, 's', LOW, HIGH, "Turn the", "Transmorgifier"};
//struct things Nitro = { , 'e', LOW, HIGH, "", ""};
struct things Missiles = {23, 'm', LOW, HIGH, "", ""};
struct things Reach = {28, 'b', LOW, HIGH, "Reach in", ""};
struct things Amp = {26, 'l', LOW, 0, "", ""};
struct things Five = {25, 'b', LOW, HIGH, "High-five!", ""};
struct things CowBell = {37, 'b', LOW, HIGH, "I have a fever", "...More Cowbell"};
struct things Table = {10, 'b', LOW, HIGH, "Flip the Table", ""};
struct things Engine = {32, 'b', LOW, HIGH, "Start the Engine", ""};
struct things Pot = {14, 'p', 0, 2, "Panic Level:", ""}; //NOTE: not its actual pin
//Joystick
struct things South = {11, 'b', LOW, HIGH, "South", ""};
struct things West = {12, 'b', LOW, HIGH, "West", ""};
struct things North = {13, 'b', LOW, HIGH, "North", ""};
struct things East = {22, 'b', LOW, HIGH, "East", ""};
//The pin is just a filler. It's the same as its index in everything[]
struct things NE = {0, '2', LOW, 0, "Northeast", ""};
struct things NW = {1, '2', LOW, 1, "Northwest", ""};
struct things SouthEast = {2, '2', LOW, 2, "Southeast", ""};
struct things SW = {3, '2', LOW, 3, "Southwest", ""};

//make sure array index matches pinNumber
const int length = 42;
//struct things everything[] = {FILLER, FILLER, Lights, Camera, 
//              South, North, West, East, SouthEast, SW, NE};

struct things everything[] = {NE, NW, SouthEast, SW, FILLER, FILLER, Go, Speed, Stop, Hammer, 
        FILLER, South, West, North, FILLER, FILLER, FILLER, FILLER, FILLER, FILLER, FILLER, 
        FILLER, East, Missiles, Cannon, Five, FILLER, FILLER, Reach, Piano, BCover, RShield,
        Engine, RCover, LShield, LCover, Trans, CowBell, Lights, Camera, Action, DoorBell};

//Needs to be PWM
Adafruit_NeoPixel mystrip = Adafruit_NeoPixel(15, 2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel yourstrip = Adafruit_NeoPixel(15, 3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel hisstrip = Adafruit_NeoPixel(15, 4, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel herstrip = Adafruit_NeoPixel(15, 5, NEO_GRB + NEO_KHZ800);

Adafruit_NeoPixel strips[] = {yourstrip, hisstrip, mystrip, herstrip};

LiquidCrystal_I2C mylcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 
LiquidCrystal_I2C yourlcd(0x26, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
LiquidCrystal_I2C hislcd(0x25, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
LiquidCrystal_I2C herlcd(0x24, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

int lcdVals[] = {0, 0, 0, 0};
LiquidCrystal_I2C lcds[] = {mylcd, herlcd, hislcd, yourlcd};

long prev[] = {0, 0, 0, 0};
long interval[] = {4000, 4000, 4000, 4000};
long minint = 10000;
long maxint = 12000;

boolean Play = true;

void colors(long hunter, int i); //this function is soooo long  

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
        //reduceTime(screen);
      }
      break;
     case 1: //NW
       if(digitalRead(North.pinNumber) && digitalRead(West.pinNumber)){
         lcds[screen].clear();
         lcdVals[screen] = 0;
         //reduceTime(screen);
       }
       break;
     case 2://SE
       Serial.println("SE");
       Serial.println(digitalRead(South.pinNumber));
       Serial.println(digitalRead(East.pinNumber));
       Serial.println(digitalRead(North.pinNumber));
       Serial.println(digitalRead(West.pinNumber));
       if(digitalRead(South.pinNumber) && digitalRead(East.pinNumber)){
         lcds[screen].clear();
         lcdVals[screen] = 0;
         //reduceTime(screen);
       }
       break;
     case 3://SW
       if(digitalRead(South.pinNumber) && digitalRead(West.pinNumber)){
         lcds[screen] .clear();
         lcdVals[screen] = 0;
         //reduceTime(screen);
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
  switch(val){
    case 0:
      lcds[screen].write("Zen");
      break;
    case 1:
      lcds[screen].write("Unsettled");
      break;
    case 2:
      lcds[screen].write("Finals");
      break;
  }
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
  if(val < 200){
    pos = 0;
  } else if(868<val && val<942){ //arduino doesn't seem to like combined inequalities
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
    //reduceTime(screen);
  }
}
    
void setCommand(int screen, int com)
{
  lcdVals[screen] = com;
  lcds[screen].clear();
  lcds[screen].setCursor(0,0);
  switch(everything[com].type){
    case 'p':
      pot(screen);
      break;
    case 'h':
      if(digitalRead(com)){
            lcds[screen].write("Stop Hammer Time");
            everything[com].desired = LOW;
      } else {
            lcds[screen].write("Enter");
            lcds[screen].setCursor(0,1);
            lcds[screen].write("Hammer Space");
            everything[screen].desired = HIGH;
      }
      break;
    case 'm':
          if(digitalRead(everything[com].pinNumber)){
            lcds[screen].write("...You can stop");
            lcds[screen].setCursor(0,1);
            lcds[screen].write("firing missiles");
            everything[com].desired = LOW;
          } else {
            lcds[screen].write("Fire missiles");
            everything[com].desired = HIGH;
          }
          break;
     case 'c':
          if(digitalRead(everything[com].pinNumber)){
            lcds[screen].write("...You can stop");
            lcds[screen].setCursor(0,1);
            lcds[screen].write("firing cannons");
            everything[com].desired = LOW;
          } else {
            lcds[screen].write("Fire cannons");
            everything[com].desired = HIGH;
          }
          break;
     case 'b':
     case 's':
     case '2':
            lcds[screen].write(everything[com].message1);
            lcds[screen].setCursor(0, 1);
            lcds[screen].write(everything[com].message2);
            break;
     case 'l':
            if(everything[com].desired){
              everything[com].desired = 0;
              lcds[screen].write("Go to 0");
            } else {
              everything[com].desired = 1;
              lcds[screen].write("Turn it up to 11");
            }
            break;
  }
}
        
void reduceTime(int i, unsigned long cur)
{
        int remaining = (prev[i]+interval[i]-cur);
        prev[i] = prev[i] - remaining*.5; //Give half the remaining time as break
}

void checkButtons(unsigned long cur)
{
  for(int i=0; i<LCDS; ++i){
    if(cur-prev[i]>interval[i]){
      interval[i] = newint();
      if(!(lcdVals[i])){
        prev[i] = cur;
        int com = random(0, length);
        while(!everything[com].pinNumber){
          com = random(0, length); //There has to be a better way to do this
          Serial.println("random");
        }
          setCommand(i, com);
      } else {
        Play = false;
        Serial.println(lcdVals[i]);
        Serial.println(interval[i]);
        Serial.println(cur);
        Serial.println(prev[i]);
      }
    }
    //refactor this into a function
    if(lcdVals[i]){
      //struct things *mything = &everything[lcdVals[i]]; //to save typing
      Serial.println(everything[lcdVals[i]].type);
      Serial.println(digitalRead(lcdVals[i]));
      //Serial.println("help");
      if(everything[lcdVals[i]].type == 'p'){
        checkPot(i);
        Serial.println("Here2");
      } else if(everything[lcdVals[i]].type == '2'){
        Serial.println("2");
        diag(i, everything[lcdVals[i]].desired);
      } else if(everything[lcdVals[i]].type == 'l'){
        if(digitalRead(lcdVals[i]+everything[lcdVals[i]].desired)){
          lcds[i].clear();
          lcdVals[i] = 0;
          reduceTime(i, cur);
        }
      } else if(digitalRead(lcdVals[i]) == everything[lcdVals[i]].desired){
        delay(1);
        lcds[i].clear();
        Serial.println(lcdVals[i]);
        lcdVals[i] = 0;
        reduceTime(i, cur);
        if(everything[lcdVals[i]].type == 's'){
          everything[lcdVals[i]].cur = everything[lcdVals[i]].desired;
          everything[lcdVals[i]].desired = !(everything[lcdVals[i]].desired);
        }
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
  while(1){
    for(int i =0; i< LCDS; ++i){
      strips[i].setPixelColor(0, 255, 0, 0);
      strips[i].setPixelColor(1, 255, 0, 0);
      strips[i].setPixelColor(2, 255, 0, 0);
      strips[i].setPixelColor(3,  255, 0, 0);
      strips[i].setPixelColor(4,  255, 0, 0);
      strips[i].setPixelColor(5, 255, 0, 0);
      strips[i].setPixelColor(6,  255, 0, 0);
      strips[i].setPixelColor(7,  255, 0, 0);
      strips[i].setPixelColor(8,  255, 0, 0);
      strips[i].setPixelColor(9,  255, 0, 0);
      strips[i].setPixelColor(10,  255, 0, 0);
      strips[i].setPixelColor(11,  255, 0, 0);
      strips[i].setPixelColor(12,  255, 0, 0);
      strips[i].setPixelColor(13,  255, 0, 0);
      strips[i].setPixelColor(14,  255, 0, 0);
      strips[i].show();
    }
    delay(500);
    for(int i =0; i<LCDS; ++i){
      strips[i].setPixelColor(0, 0, 0, 0);
      strips[i].setPixelColor(1, 0, 0, 0);
      strips[i].setPixelColor(2, 0, 0, 0);
      strips[i].setPixelColor(3,  0, 0, 0);
      strips[i].setPixelColor(4,  0, 0, 0);
      strips[i].setPixelColor(5, 0, 0, 0);
      strips[i].setPixelColor(6,  0, 0, 0);
      strips[i].setPixelColor(7,  0, 0, 0);
      strips[i].setPixelColor(8,  0, 0, 0);
      strips[i].setPixelColor(9,  0, 0, 0);
      strips[i].setPixelColor(10,  0, 0, 0);
      strips[i].setPixelColor(11,  0, 0, 0);
      strips[i].setPixelColor(12,  0, 0, 0);
      strips[i].setPixelColor(13,  0, 0, 0);
      strips[i].setPixelColor(14,  0, 0, 0);
      strips[i].show();
    }
    delay(250);
  }
      
  Serial.println("lost");
  delay(10000);
}
  
void setup()
{
  for(int i = 0; i<length; ++i){
    if(everything[i].pinNumber) pinMode(everything[i].pinNumber, INPUT);
  }
  pinMode(A0, INPUT); //just double checking
  for(int i = 0; i<LCDS; ++i){
    lcds[i].begin(16, 2);
  }
  Serial.begin(9600);
  Serial.println("Start");
  for(int i =0; i< LCDS; ++i){
    strips[i].begin();
    strips[i].show();
  }
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
}

void health(long cur)
{
  for(int i = 0; i < LCDS; ++i){
    long val = (cur-prev[i])* 15000 / interval[i]; //normalize to hunter's 15 seconds
    colors(val, i);
  }
}

void loop()
{
  if(Play){
   unsigned long cur = millis();
   checkButtons(cur);
   delay(1); //for stability when reading buttons
   //writeLCD();
   health(cur);
   delay(15);
  } else {
    lose();
  }
}

void colors(long hunter, int i)  
{
  if(hunter > 0){ 
      strips[i].setPixelColor(0, 0, 255, 0);
      strips[i].setPixelColor(1, 0, 255, 0);
      strips[i].setPixelColor(2, 0, 255, 0);
      strips[i].setPixelColor(3,  0, 255, 0);
      strips[i].setPixelColor(4,  0, 255, 0);
      strips[i].setPixelColor(5, 0, 255, 0);
      strips[i].setPixelColor(6,  0, 255, 0);
      strips[i].setPixelColor(7,  0, 255, 0);
      strips[i].setPixelColor(8,  0, 255, 0);
      strips[i].setPixelColor(9,  0, 255, 0);
      strips[i].setPixelColor(10,  0, 255, 0);
      strips[i].setPixelColor(11,  0, 255, 0);
      strips[i].setPixelColor(12,  0, 255, 0);
      strips[i].setPixelColor(13,  0, 255, 0);
      strips[i].setPixelColor(14,  0, 255, 0);
  }
  
  if(hunter > 1000){
      strips[i].setPixelColor(0, 17, 238, 0);
      strips[i].setPixelColor(1, 17, 238, 0);
      strips[i].setPixelColor(2, 17, 238, 0);
      strips[i].setPixelColor(3, 17, 238, 0);
      strips[i].setPixelColor(4, 17, 238, 0);
      strips[i].setPixelColor(5, 17, 238, 0);
      strips[i].setPixelColor(6, 17, 238, 0);
      strips[i].setPixelColor(7, 17, 238, 0);
      strips[i].setPixelColor(8, 17, 238, 0);
      strips[i].setPixelColor(9, 17, 238, 0);
      strips[i].setPixelColor(10, 17, 238, 0);
      strips[i].setPixelColor(11, 17, 238, 0);
      strips[i].setPixelColor(12, 17, 238, 0);
      strips[i].setPixelColor(13, 17, 238, 0);
      strips[i].setPixelColor(14, 0, 0, 0);
  }
  
  if(hunter > 2000){
      strips[i].setPixelColor(0, 34, 221, 0);
      strips[i].setPixelColor(1, 34, 221, 0);
      strips[i].setPixelColor(2, 34, 221, 0);
      strips[i].setPixelColor(3, 34, 221, 0);
      strips[i].setPixelColor(4, 34, 221, 0);
      strips[i].setPixelColor(5, 34, 221, 0);
      strips[i].setPixelColor(6, 34, 221, 0);
      strips[i].setPixelColor(7, 34, 221, 0);
      strips[i].setPixelColor(8, 34, 221, 0);
      strips[i].setPixelColor(9, 34, 221, 0);
      strips[i].setPixelColor(10, 34, 221, 0);
      strips[i].setPixelColor(11, 34, 221, 0);
      strips[i].setPixelColor(12, 34, 221, 0);
      strips[i].setPixelColor(13, 0, 0, 0);
      strips[i].setPixelColor(14, 0, 0, 0);
  }
  if(hunter > 3000){
      strips[i].setPixelColor(0, 51, 204, 0);
      strips[i].setPixelColor(1, 51, 204, 0); 
      strips[i].setPixelColor(2, 51, 204, 0);
      strips[i].setPixelColor(3, 51, 204, 0);
      strips[i].setPixelColor(4, 51, 204, 0);
      strips[i].setPixelColor(5, 51, 204, 0);
      strips[i].setPixelColor(6, 51, 204, 0);
      strips[i].setPixelColor(7, 51, 204, 0);
      strips[i].setPixelColor(8, 51, 204, 0);
      strips[i].setPixelColor(9, 51, 204, 0);
      strips[i].setPixelColor(10, 51, 204, 0);
      strips[i].setPixelColor(11, 51, 204, 0);
      strips[i].setPixelColor(12, 0, 0, 0);
      strips[i].setPixelColor(13, 0, 0, 0);
      strips[i].setPixelColor(14, 0, 0, 0);
  }
  if(hunter > 4000){
      strips[i].setPixelColor(0, 68, 187, 0);
      strips[i].setPixelColor(1, 68, 187, 0);
      strips[i].setPixelColor(2, 68, 187, 0);
      strips[i].setPixelColor(3, 68, 187, 0);
      strips[i].setPixelColor(4, 68, 187, 0);
      strips[i].setPixelColor(5, 68, 187, 0);
      strips[i].setPixelColor(6, 68, 187, 0);
      strips[i].setPixelColor(7, 68, 187, 0);
      strips[i].setPixelColor(8, 68, 187, 0);
      strips[i].setPixelColor(9, 68, 187, 0);
      strips[i].setPixelColor(10, 68, 187, 0);
      strips[i].setPixelColor(11, 0, 0, 0);
      strips[i].setPixelColor(12, 0, 0, 0);
      strips[i].setPixelColor(13, 0, 0, 0);
      strips[i].setPixelColor(14, 0, 0, 0);
  }
  if (hunter > 5000)
  {
      strips[i].setPixelColor(0, 85, 170, 0);
      strips[i].setPixelColor(1, 85, 170, 0);
      strips[i].setPixelColor(2, 85, 170, 0);
      strips[i].setPixelColor(3, 85, 170, 0);
      strips[i].setPixelColor(4, 85, 170, 0);
      strips[i].setPixelColor(5, 85, 170, 0);
      strips[i].setPixelColor(6, 85, 170, 0);
      strips[i].setPixelColor(7, 85, 170, 0);
      strips[i].setPixelColor(8, 85, 170, 0);
      strips[i].setPixelColor(9, 85, 170, 0);
      strips[i].setPixelColor(10, 0, 0, 0);
      strips[i].setPixelColor(11, 0, 0, 0);
      strips[i].setPixelColor(12, 0, 0, 0);
      strips[i].setPixelColor(13, 0, 0, 0);
      strips[i].setPixelColor(14, 0, 0, 0);
  }
  if (hunter > 6000)
  {
      strips[i].setPixelColor(0, 102, 153, 0);
      strips[i].setPixelColor(1, 102, 153, 0);
      strips[i].setPixelColor(2, 102, 153, 0);
      strips[i].setPixelColor(3, 102, 153, 0);
      strips[i].setPixelColor(4, 102, 153, 0);
      strips[i].setPixelColor(5, 102, 153, 0);
      strips[i].setPixelColor(6, 102, 153, 0);
      strips[i].setPixelColor(7, 102, 153, 0);
      strips[i].setPixelColor(8, 102, 153, 0);      
      strips[i].setPixelColor(9, 0, 0, 0);
      strips[i].setPixelColor(10, 0, 0, 0);
      strips[i].setPixelColor(11, 0, 0, 0);
      strips[i].setPixelColor(12, 0, 0, 0);
      strips[i].setPixelColor(13, 0, 0, 0);
      strips[i].setPixelColor(14, 0, 0, 0);
  }
  if (hunter > 7000)
  {
      strips[i].setPixelColor(0, 119, 136, 0);
      strips[i].setPixelColor(1, 119, 136, 0);
      strips[i].setPixelColor(2, 119, 136, 0);
      strips[i].setPixelColor(3, 119, 136, 0);
      strips[i].setPixelColor(4, 119, 136, 0);
      strips[i].setPixelColor(5, 119, 136, 0);
      strips[i].setPixelColor(6, 119, 136, 0);
      strips[i].setPixelColor(7, 119, 136, 0);
      strips[i].setPixelColor(8, 0, 0, 0);      
      strips[i].setPixelColor(9, 0, 0, 0);
      strips[i].setPixelColor(10, 0, 0, 0);
      strips[i].setPixelColor(11, 0, 0, 0);
      strips[i].setPixelColor(12, 0, 0, 0);
      strips[i].setPixelColor(13, 0, 0, 0);
      strips[i].setPixelColor(14, 0, 0, 0);
  }
 if (hunter > 8000)
  {
      strips[i].setPixelColor(0, 136, 119, 0);
      strips[i].setPixelColor(1, 136, 119, 0);
      strips[i].setPixelColor(2, 136, 119, 0);
      strips[i].setPixelColor(3, 136, 119, 0);
      strips[i].setPixelColor(4, 136, 119, 0);
      strips[i].setPixelColor(5, 136, 119, 0);
      strips[i].setPixelColor(6, 136, 119, 0);
      strips[i].setPixelColor(7, 0, 0, 0);
      strips[i].setPixelColor(8, 0, 0, 0);      
      strips[i].setPixelColor(9, 0, 0, 0);
      strips[i].setPixelColor(10, 0, 0, 0);
      strips[i].setPixelColor(11, 0, 0, 0);
      strips[i].setPixelColor(12, 0, 0, 0);
      strips[i].setPixelColor(13, 0, 0, 0);
      strips[i].setPixelColor(14, 0, 0, 0);
}
 if (hunter > 9000)
  {
      strips[i].setPixelColor(0, 153, 102, 0);
      strips[i].setPixelColor(1, 153, 102, 0);
      strips[i].setPixelColor(2, 153, 102, 0);
      strips[i].setPixelColor(3, 153, 102, 0);
      strips[i].setPixelColor(4, 153, 102, 0);
      strips[i].setPixelColor(5, 153, 102, 0);
      strips[i].setPixelColor(6, 0, 0, 0);
      strips[i].setPixelColor(7, 0, 0, 0);
      strips[i].setPixelColor(8, 0, 0, 0);      
      strips[i].setPixelColor(9, 0, 0, 0);
      strips[i].setPixelColor(10, 0, 0, 0);
      strips[i].setPixelColor(11, 0, 0, 0);
      strips[i].setPixelColor(12, 0, 0, 0);
      strips[i].setPixelColor(13, 0, 0, 0);
      strips[i].setPixelColor(14, 0, 0, 0);
  }
 if (hunter > 10000)
  {
      strips[i].setPixelColor(0, 170, 85, 0);
      strips[i].setPixelColor(1, 170, 85, 0);
      strips[i].setPixelColor(2, 170, 85, 0);
      strips[i].setPixelColor(3, 170, 85, 0);
      strips[i].setPixelColor(4, 170, 85, 0);
      strips[i].setPixelColor(5, 0, 0, 0);
      strips[i].setPixelColor(6, 0, 0, 0);
      strips[i].setPixelColor(7, 0, 0, 0);
      strips[i].setPixelColor(8, 0, 0, 0);      
      strips[i].setPixelColor(9, 0, 0, 0);
      strips[i].setPixelColor(10, 0, 0, 0);
      strips[i].setPixelColor(11, 0, 0, 0);
      strips[i].setPixelColor(12, 0, 0, 0);
      strips[i].setPixelColor(13, 0, 0, 0);
      strips[i].setPixelColor(14, 0, 0, 0);
  }
if (hunter > 11000)
  {
      strips[i].setPixelColor(0, 187, 68, 0);
      strips[i].setPixelColor(1, 187, 68, 0);
      strips[i].setPixelColor(2, 187, 68, 0);
      strips[i].setPixelColor(3, 187, 68, 0);
      strips[i].setPixelColor(4, 0, 0, 0);
      strips[i].setPixelColor(5, 0, 0, 0);
      strips[i].setPixelColor(6, 0, 0, 0);
      strips[i].setPixelColor(7, 0, 0, 0);
      strips[i].setPixelColor(8, 0, 0, 0);      
      strips[i].setPixelColor(9, 0, 0, 0);
      strips[i].setPixelColor(10, 0, 0, 0);
      strips[i].setPixelColor(11, 0, 0, 0);
      strips[i].setPixelColor(12, 0, 0, 0);
      strips[i].setPixelColor(13, 0, 0, 0);
      strips[i].setPixelColor(14, 0, 0, 0);
  }
if (hunter > 12000)
  {
      strips[i].setPixelColor(0, 204, 51, 0);
      strips[i].setPixelColor(1, 204, 51, 0);
      strips[i].setPixelColor(2, 204, 51, 0);
      strips[i].setPixelColor(3, 0, 0, 0);
      strips[i].setPixelColor(4, 0, 0, 0);
      strips[i].setPixelColor(5, 0, 0, 0);
      strips[i].setPixelColor(6, 0, 0, 0);
      strips[i].setPixelColor(7, 0, 0, 0);
      strips[i].setPixelColor(8, 0, 0, 0);      
      strips[i].setPixelColor(9, 0, 0, 0);
      strips[i].setPixelColor(10, 0, 0, 0);
      strips[i].setPixelColor(11, 0, 0, 0);
      strips[i].setPixelColor(12, 0, 0, 0);
      strips[i].setPixelColor(13, 0, 0, 0);
      strips[i].setPixelColor(14, 0, 0, 0);
  }
 if (hunter > 13000)
  {
      strips[i].setPixelColor(0, 221, 34, 0);
      strips[i].setPixelColor(1, 221, 34, 0);
      strips[i].setPixelColor(2, 0, 0, 0);
      strips[i].setPixelColor(3, 0, 0, 0);
      strips[i].setPixelColor(4, 0, 0, 0);
      strips[i].setPixelColor(5, 0, 0, 0);
      strips[i].setPixelColor(6, 0, 0, 0);
      strips[i].setPixelColor(7, 0, 0, 0);
      strips[i].setPixelColor(8, 0, 0, 0);      
      strips[i].setPixelColor(9, 0, 0, 0);
      strips[i].setPixelColor(10, 0, 0, 0);
      strips[i].setPixelColor(11, 0, 0, 0);
      strips[i].setPixelColor(12, 0, 0, 0);
      strips[i].setPixelColor(13, 0, 0, 0);
      strips[i].setPixelColor(14, 0, 0, 0);
  }
 if (hunter > 14000)
  {
      strips[i].setPixelColor(0, 255, 0, 0);
      strips[i].setPixelColor(1, 0, 0, 0);
      strips[i].setPixelColor(2, 0, 0, 0);
      strips[i].setPixelColor(3, 0, 0, 0);
      strips[i].setPixelColor(4, 0, 0, 0);
      strips[i].setPixelColor(5, 0, 0, 0);
      strips[i].setPixelColor(6, 0, 0, 0);
      strips[i].setPixelColor(7, 0, 0, 0);
      strips[i].setPixelColor(8, 0, 0, 0);      
      strips[i].setPixelColor(9, 0, 0, 0);
      strips[i].setPixelColor(10, 0, 0, 0);
      strips[i].setPixelColor(11, 0, 0, 0);
      strips[i].setPixelColor(12, 0, 0, 0);
      strips[i].setPixelColor(13, 0, 0, 0);
      strips[i].setPixelColor(14, 0, 0, 0);
  }
  if(lcdVals[i] == 0){
      strips[i].setPixelColor(0, 0, 255, 0);
      strips[i].setPixelColor(1, 0, 255, 0);
      strips[i].setPixelColor(2, 0, 255, 0);
      strips[i].setPixelColor(3,  0, 255, 0);
      strips[i].setPixelColor(4,  0, 255, 0);
      strips[i].setPixelColor(5, 0, 255, 0);
      strips[i].setPixelColor(6,  0, 255, 0);
      strips[i].setPixelColor(7,  0, 255, 0);
      strips[i].setPixelColor(8,  0, 255, 0);
      strips[i].setPixelColor(9,  0, 255, 0);
      strips[i].setPixelColor(10,  0, 255, 0);
      strips[i].setPixelColor(11,  0, 255, 0);
      strips[i].setPixelColor(12,  0, 255, 0);
      strips[i].setPixelColor(13,  0, 255, 0);
      strips[i].setPixelColor(14,  0, 255, 0);
  }
  strips[i].show();
}
