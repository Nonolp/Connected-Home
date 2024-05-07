#include <Adafruit_LiquidCrystal.h>

Adafruit_LiquidCrystal lcd_1(0);
int pir = 6;
int buzzer = 7;
int piretat = 0;
const int inter= 5;

void setup()
{
  lcd_1.begin(16, 2); 
  pinMode(pir, INPUT);
  pinMode(buzzer, OUTPUT);
  lcd_1.clear();
  pinMode(inter,INPUT);
  
}

void loop()
{ 
  int etat_inter= digitalRead(inter);
  if (etat_inter == HIGH){
  
  piretat = digitalRead(pir); // mouvement detecté
  
  if (piretat == HIGH){
  digitalWrite (buzzer, HIGH); // allume le buzzer
  lcd_1.print("Intrusion detecte");
  delay(100);
  lcd_1.clear();
  }
    else {
    digitalWrite (buzzer, LOW); // eteindre le buzzer
  lcd_1.setCursor(5,0);
  lcd_1.print("Aucune");
  lcd_1.setCursor(3,1);
  lcd_1.print("intrusion");
  delay(100);
  lcd_1.clear();
 } 
}


else {
   lcd_1.setCursor(0,0);
   digitalWrite (buzzer, LOW);
   lcd_1.print("ALARME");
   lcd_1.setCursor(0,1);
   lcd_1.print("DESACTIVE");
  delay(100);
  lcd_1.clear();
	

}}

