#include <Servo.h>
Servo monServo;
#include <Keypad.h>
#include <Adafruit_LiquidCrystal.h>
Adafruit_LiquidCrystal lcd(0);

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'X','0','X','X'}
};
byte rowPins[ROWS] = {A0, A1, 11, 10};
byte colPins[COLS] = {9, 8, 7, 6};
int LCDRow = 0;
int pinmonServo=4;
String saisie;
String combinaison;
int Tentatives = 0;
int PorteBALouverte = 0;
char key;
char ToucheAppuyee;
int ChangementDeCode = 0;
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
int buzzer = 2;

void setup(){
  monServo.attach(4);
  Serial.begin(9600);
  lcd.begin(16, 2);
  combinaison = String("1234");
  monServo.write(0);
  DemandeCode();
  pinMode(buzzer, OUTPUT);
}

void loop(){
  LireAppuieTouche();
}

void LireAppuieTouche(){
  char key = keypad.getKey();
  if (key){
    AppuieTouche(key);
  }
}

void AppuieTouche(char key){
  ToucheAppuyee = key;
  if (ToucheAppuyee == 'A'){
    AppuieToucheA();
  } 
  else if (ToucheAppuyee == 'B'){
    AppuieToucheB();
  }   
  if (ToucheAppuyee == 'C'){
    AppuieToucheC();
  }   
  if (ToucheAppuyee != 'A' && ToucheAppuyee != 'B' && ToucheAppuyee != 'C' && ToucheAppuyee != 'X'){
    EcrireChiffre();
  }
}

void AppuieToucheA(){
  if (ChangementDeCode != 2){
    VerifierCombinaison();
  }
  else {
    ChangementDuCode();   
  }
}

void AppuieToucheB(){
  if (PorteBALouverte == 0){
    SupprimerSaisie();
  }
  else {
    FermerPorteBAL();
  }
}

void AppuieToucheC(){
  if (ChangementDeCode != 1){
    DemandeAncienCode();
  }
  else {
    ChangementDeCode = 0;
    DemandeCode();
  }
}

void EcrireChiffre(){
  lcd.print(ToucheAppuyee);
  saisie = saisie + ToucheAppuyee;
} 

void VerifierCombinaison(){
   if (saisie == combinaison ){
     CodeBon();
   }
   else {
     CodeFaux();
   }
}

void CodeBon(){
   Tentatives = 0; 
  if (ChangementDeCode != 1){
    lcd.clear();
    lcd.setCursor (0,0);
    lcd.print("CODE BON   ");      
    OuvrirPorteBAL();
  }
  else{
  DemandeNouveauCode();            
  }
}

void CodeFaux(){
  ++Tentatives;          
  lcd.clear();
  lcd.setCursor (0,0);
  lcd.print("CODE FAUX     ");        
  lcd.setCursor (0,1);
  lcd.print(3 - Tentatives);
  lcd.print(" ESSAI RESTANT     ");
  delay (1000);
  if (Tentatives >= 3){
    Alerte();
  }
  else {
    DemandeCode();
  }
}

void OuvrirPorteBAL(){
  PorteBALouverte = 1;
  lcd.clear();
  lcd.setCursor (0,0);
  lcd.print("OUVERTURE...    ");
   monServo.write(90);
  digitalWrite (buzzer, HIGH);
  delay(100);
  digitalWrite (buzzer, LOW);
  delay(10000);
  lcd.clear();
  lcd.setCursor (0,0);
  lcd.print("FERMER AVEC B     ");
}

void FermerPorteBAL(){
  PorteBALouverte = 0;
  lcd.clear();
  lcd.setCursor (0,0);
  lcd.print("FERMETURE...    ");
  monServo.write(0);
  delay(10000);
  DemandeCode();
}

void SupprimerSaisie(){                 
  saisie = "";
  lcd.setCursor(0,1);
  lcd.print("             ");
  lcd.setCursor(0,1);
}
  
void Alerte(){
    digitalWrite (buzzer, HIGH);
    lcd.clear();
    lcd.print("    ALERTE !");
    delay(600000);
    Tentatives = 0;
    DemandeCode(); 
}

void DemandeCode(){
  saisie = "";
  lcd.clear();
  lcd.print("SAISIR CODE :");
  lcd.setCursor(0,1);
}

void DemandeAncienCode(){
  saisie = "";
  lcd.clear();
  lcd.print("ANCIEN CODE :");
  lcd.setCursor(0,1);
  ChangementDeCode = 1;
}

void DemandeNouveauCode(){
  ChangementDeCode = 2;
  saisie = "";
  lcd.clear();
  lcd.print("NOUVEAU CODE :");
  lcd.setCursor(0,1);
}

void ChangementDuCode(){
  combinaison = String(saisie);
  lcd.clear();
  lcd.print("MODIFICATION...");
  ChangementDeCode = 0;
  delay(1000);
  DemandeCode(); 
}
