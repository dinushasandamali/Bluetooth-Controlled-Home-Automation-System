#include <LiquidCrystal.h>
LiquidCrystal lcd(2,3,4,5,6,7);
int tempPin = A0; // the output pin of LM35
int temp;
int tempMin = 25; // the temperature to start the fan 0%
int tempMax = 70; // the maximum temperature when fan is at 100%
int fanSpeed;
int fanLCD;
char check = 'Y';
String voice;

void setup() {
pinMode(13, OUTPUT);
pinMode(12, OUTPUT);
pinMode(tempPin, INPUT);
lcd.begin(16,2);
Serial.begin(9600);
}

void loop() {
while(Serial.available()){
  delay(3);
  char c = Serial.read();
  voice+=c;
  }
temp = readTemp(); 
Serial.print( temp );

if(voice.length() >0)
{
  Serial.println(voice);
  
  if(voice == "light on")
  {
    digitalWrite(13, HIGH);
    fanLCD = 0;
  }
  if(voice == "light off")
  {

    digitalWrite(13, LOW);
    fanLCD = 0;
  }
  temp = readTemp();
  if((voice == "fan on"))
  {
	  check = 'X';
  }
 
  if(voice == "fan off")
  {
    check = 'Y';
    digitalWrite(12, LOW);
    fanLCD = 0;
  }
  if(voice == "all on")
  {
    digitalWrite(12, HIGH);
    digitalWrite(13, HIGH);
  }
  if(voice == "all off")
  {
    digitalWrite(12, LOW);
    digitalWrite(13, LOW);
    fanLCD = 0;
  }
  voice = "";
  }

  temp = readTemp(); // get the temperature
  Serial.print( temp );
  if (temp == tempMin){
    fanSpeed = 0; // fan is not spinning
    //analogWrite(12, fanSpeed);
    fanLCD=0;
    digitalWrite(12, LOW);
  }

  if((check == 'X')&&(temp > tempMin) && (temp <= tempMax)) // if temperature is higher than minimum temp
  {
  fanSpeed = temp;//map(temp, tempMin, tempMax, 0, 100); // the actual speed of fan//map(temp, tempMin, tempMax, 32, 255);
  fanSpeed=1.5*fanSpeed;
  fanLCD = map(temp, tempMin, tempMax, 0, 100); // speed of fan to display on LCD100
  analogWrite(12, fanSpeed); // spin the fan at the fanSpeed speed
  digitalWrite(12, HIGH);
  }

  lcd.print("TEMP: ");
  lcd.print(temp); // display the temperature
  lcd.print("C ");
  lcd.setCursor(0,1); // move cursor to next line
  lcd.print("FANS: ");
  lcd.print(fanLCD); // display the fan speed
  lcd.print("%");
  delay(200);
  lcd.clear();

}

int readTemp() { // get the temperature and convert it to celsius
temp = analogRead(tempPin);
return temp * 0.48828125;
}