LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 1);
  // Print a message to the LCD.
  
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(4, 1);
  lcd.print("Silenzio!");
  delay(5000);
  lcd.clear(); 
  lcd.print("Abbassa la voce");
  delay(5000);
  lcd.clear(); 
  lcd.setCursor(1, 1);
  lcd.print("Troppo rumore!");
  delay(5000);
  lcd.clear(); 
  lcd.setCursor(3, 1);
  lcd.print("SILENZIO!");
  delay(5000);
  lcd.clear(); 
  lcd.setCursor(0, 1);
  lcd.print("ABBASSA LA VOCE");
  delay(5000);
  lcd.clear();
  lcd.setCursor(1, 1); 
  lcd.print("TROPPO RUMORE!");
  delay(5000);
  lcd.clear();
}
 
