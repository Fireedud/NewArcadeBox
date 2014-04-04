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
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address


/*-----( Declare Variables )-----*/
//none

void setup()   /*----( SETUP: RUNS ONCE )----*/
{
  pinMode(2, INPUT);
  lcd.begin(16,2);         // initialize the lcd for 16 chars 2 lines and turn on backlight

// ------- Quick 3 blinks of backlight  -------------
  for(int i = 0; i< 3; i++)
  {
    lcd.backlight();
    delay(250);
    lcd.noBacklight();
    delay(250);
  }
  lcd.backlight(); // finish with backlight on  
  
}/*--(end setup )---*/

boolean prev = false;

void loop()   /*----( LOOP: RUNS CONSTANTLY )----*/
{
  if(digitalRead(2)){
    if(prev){
      ;
    } else {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("You are pressing");
      lcd.setCursor(0,1);
      lcd.print("the button.");
      prev = true;
    }
  }else{
    if(prev){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("You suck");
      prev = false;
    } else {
      ;
    }
  }
  delay(50);


}

