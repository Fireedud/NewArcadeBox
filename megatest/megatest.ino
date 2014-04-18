/* YourDuino.com Example Software Sketch
 20 character 4 line I2C Display
 Backpack Interface labelled "LCM1602 IIC  A0 A1 A2"
 terry@yourduino.com */

/*-----( Import needed libraries )-----*/
#include <Wire.h>  // Comes with Arduino IDE
// Get the LCD I2C Library here: 
// https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads
// Move any other LCD libraries to another folder or delete them
// See Library "Docs" folder for possible commands etc.
#include <LiquidCrystal_I2C.h>

/*-----( Declare Constants )-----*/
//none
/*-----( Declare objects )-----*/
// set the LCD address to 0x27 for a 20 chars 4 line display
// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C mylcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
//LiquidCrystal_I2C yourlcd(0x26, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
//LiquidCrystal_I2C hislcd(0x25, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
//LiquidCrystal_I2C herlcd(0x24, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);


/*-----( Declare Variables )-----*/
//none

struct switches{
  int pinNumber;
  int cur;
  int desired;
};

struct switches myswitch = {6, LOW, HIGH};

void setup()   /*----( SETUP: RUNS ONCE )----*/
{
  mylcd.begin(16,2);         // initialize the lcd for 16 chars 2 lines and turn on backlight
  pinMode(6, INPUT);
  //yourlcd.begin(16,2);
  //hislcd.begin(16,2);
  //herlcd.begin(16,2);

}/*--(end setup )---*/

void loop()   /*----( LOOP: RUNS CONSTANTLY )----*/
{
  mylcd.clear();
  myswitch.cur = digitalRead(myswitch.pinNumber);
  mylcd.setCursor(0,0);
  if(myswitch.cur){
    mylcd.write("ON");
  } else{
    mylcd.write("OFF!");
  }
  mylcd.setCursor(0,1);
  if(myswitch.cur == myswitch.desired){
    mylcd.write("MATCHES");
    myswitch.desired = !myswitch.desired;
  }else{
    mylcd.write("DOESN'T");
  }
  delay(500);
}

