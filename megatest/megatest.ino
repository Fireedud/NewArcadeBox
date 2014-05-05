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
LiquidCrystal_I2C yourlcd(0x26, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
//LiquidCrystal_I2C hislcd(0x25, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
//LiquidCrystal_I2C herlcd(0x24, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);


/*-----( Declare Variables )-----*/
//none

/*struct things{
  int pinNumber;
  const char type;
  int cur;
  int desired;
  const char message1[17];
  const char message2[17];
};

const int numinputs = 7;
struct things FILLER = {0, '\0', LOW, LOW, "", ""};
struct things WhiteButton = {2, 'b', LOW, HIGH, "Push White", ""};
struct things Covered1 = {3, 's', LOW, HIGH, "Flip Covered1", ""};
struct things Covered2 = {4, 's', LOW, HIGH, "Flip Covered2", ""};
struct things RedSwitch = {5, 's', LOW, HIGH, "Flip Red", ""};
struct things BlueSwitch = {6, 's', LOW, HIGH, "Flip Blue", ""};
struct things BlueButton  = {7, 'b', LOW, HIGH, "Push Blue", ""};
struct things RedButton = {8, 'b', LOW, HIGH, "Push Red", ""};

const int length = 9; //length of the array 
struct things everything[] = {FILLER, FILLER, WhiteButton, Covered1, 
  Covered2, RedSwitch, BlueSwitch, BlueButton, RedButton}; */


void setup()   /*----( SETUP: RUNS ONCE )----*/
{
  mylcd.begin(16,2);
  yourlcd.begin(16,2);
  pinMode(A0, INPUT);
  //pinMode(A0, INPUT);
  //yourlcd.begin(16,2);
  //hislcd.begin(16,2);
  //herlcd.begin(16,2);

}/*--(end setup )---*/

void loop()   /*----( LOOP: RUNS CONSTANTLY )----*/
{
  mylcd.setCursor(0,0);
  mylcd.write("here1");
  delay(10);
  yourlcd.setCursor(0,0);
  yourlcd.write("here1");
  delay(10);
  int val = analogRead(0);
  delay(10);
  mylcd.setCursor(0,1);
  char t[6];
  itoa(val, t, 10);
  mylcd.write(t);
  delay(10);
  yourlcd.setCursor(0,1);
  yourlcd.write("Here2");  
  delay(200);
  mylcd.clear();
  yourlcd.clear();

  
  //N.B.:
  //North is red
  //West is black
  //East is green
  //South is yellow
  /*if(a&&b){
    Serial.println("SW");
  }else if(a&&d){
    Serial.println("SE");
  }else if(a){
    Serial.println("S");
  }else if(c&&b){
    Serial.println("NW");
  }else if(c&&d){
    Serial.println("NE");
  }else if(c){
    Serial.println("N");
  }else if(b){
    Serial.println("W");
  }else if(d){
    Serial.println("E");
  } */
  //int val = analogRead(A0);
  //Serial.print("value: ");
  //Serial.print(val);
  //Serial.print("\n");
  //if(val<20){    
    //Serial.println(0);
  //} else if(815<val && val<920){
    //Serial.println(1);
  //} else if(1000<val){
    //Serial.println(2);
  //} else {
    //Serial.println(3);
  //}
  delay(300);
}
