#include <LiquidCrystal.h> // includes the LiquidCrystal Library 
    #include <Keypad.h>
    #define buzzer 9
    #define trigPin 24
    #define echoPin 22
    #define redLed 13
    #define greenLed 12
    long duration;
    int distance, initialDistance, currentDistance, i, loop_cnt=0, j;

    int screenOffMsg =0; int screenOffMsg1 =0;
    String password="1234";
    String tempPassword;
    boolean activated = false; // State of the alarm
    boolean isActivated;
    boolean valid=false;
    boolean activateAlarm = false;
    boolean alarmActivated = false;
    boolean enteredPassword; // State of the entered password to stop the alarm
    boolean passChangeMode = false;
    boolean passChanged = false;
    const byte ROWS = 4; //four rows
    const byte COLS = 4; //four columns
    char keypressed;
    //void  enterPassword();
    //define the cymbols on the buttons of the keypads
    char keyMap[ROWS][COLS] = {
      {'1','2','3','A'},
      {'4','5','6','B'},
      {'7','8','9','C'},
      {'*','0','#','D'}
    };
    byte rowPins[ROWS] = {14, 15, 16, 17}; //Row pinouts of the keypad
    byte colPins[COLS] = {18, 19, 20, 21}; //Column pinouts of the keypad
    Keypad myKeypad = Keypad( makeKeymap(keyMap), rowPins, colPins, ROWS, COLS); 
    LiquidCrystal lcd(8, 7 , 6, 5, 4, 3); // Creates an LC object. Parameters: (rs, enable, d4, d5, d6, d7) 
    void setup() { 
      lcd.begin(16,2); 
      pinMode(redLed, OUTPUT);
      pinMode(greenLed, OUTPUT);
      pinMode(buzzer, OUTPUT); // Set buzzer as an output
      pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
      pinMode(echoPin, INPUT); // Sets the echoPin as an Input
      
    }

    void loop()
    {
      delay(100);
          if (alarmActivated == true){
          currentDistance = getDistance(); 
          if ( currentDistance < initialDistance) {
            tone(buzzer, 1000); // Send 1KHz sound signal 
            lcd.clear();
            enterPassword();
            alarmActivated=false;
            screenOffMsg=0;
          }
        }
      if(!alarmActivated){
        noTone(buzzer);
        valid=false;
        digitalWrite(redLed,HIGH);
        digitalWrite(greenLed,LOW);
        if(screenOffMsg==0){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("A-Activare");
          lcd.setCursor(0, 1);
          lcd.print("B-Parola");
          screenOffMsg=1;
        } 
          keypressed= myKeypad.getKey();
            if(keypressed== 'A'){
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("C-PAROLA");
              lcd.setCursor(0,1);
              lcd.print("D-AMPRENTA");
            }
            if(keypressed=='C'){
              enterPassword();
              if(valid){  
                loop_cnt=0;
                activareAlarma(10);
                //lcd.clear();
                //lcd.setCursor(0,0);//activare alarma
                //lcd.print("Se va activa alarma");
               
                
                //led rosu aprins
                //functie pentru activare
                //
              }
              //lcd.clear();
              //lcd.setCursor(0,0);
              //lcd.print("Introduceti Parola");
            }else if(keypressed=='D'){
              //functie pentru amprenta
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("Introduceti Amprenta");
            }else if(keypressed=='#'){
              screenOffMsg=0;
            } else if (keypressed =='B') {
      lcd.clear();
      int i=1;
      tone(buzzer, 2000, 100);
      tempPassword = "";
      lcd.setCursor(0,0);
      lcd.print("Current Password");
      lcd.setCursor(0,1);
      lcd.print(">");
      passChangeMode = true;
      passChanged = true;   
      while(passChanged) {      
      keypressed = myKeypad.getKey();
      if (keypressed != NO_KEY){
        if (keypressed == '0' || keypressed == '1' || keypressed == '2' || keypressed == '3' ||
            keypressed == '4' || keypressed == '5' || keypressed == '6' || keypressed == '7' ||
            keypressed == '8' || keypressed == '9' ) {
         tempPassword += keypressed;
         lcd.setCursor(i,1);
         lcd.print("*");
         i++;
         tone(buzzer, 2000, 100);
        }
      }
      if (i > 5 || keypressed == '#') {
        tempPassword = "";
        i=1;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Current Password");
        lcd.setCursor(0,1);
        lcd.print(">"); 
      }
      if ( keypressed == '*') {
        i=1;
        tone(buzzer, 2000, 100);
        if (password == tempPassword) {
          tempPassword="";
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Set New Password");
          lcd.setCursor(0,1);
          lcd.print(">");
          while(passChangeMode) {
            keypressed = myKeypad.getKey();
            if (keypressed != NO_KEY){
              if (keypressed == '0' || keypressed == '1' || keypressed == '2' || keypressed == '3' ||
                  keypressed == '4' || keypressed == '5' || keypressed == '6' || keypressed == '7' ||
                  keypressed == '8' || keypressed == '9' ) {
                tempPassword += keypressed;
                lcd.setCursor(i,1);
                lcd.print("*");
                i++;
                tone(buzzer, 2000, 100);
              }
            }
            if (i > 5 || keypressed == '#') {
              tempPassword = "";
              i=1;
              tone(buzzer, 2000, 100);
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("Set New Password");
              lcd.setCursor(0,1);
              lcd.print(">");
            }
            if ( keypressed == '*') {
              i=1;
              tone(buzzer, 2000, 100);
              password = tempPassword;
              passChangeMode = false;
              passChanged = false;
              screenOffMsg = 0;
            }            
          }
        }
      }
    }
   }
 

             
            
      
      }else { 
        valid=false;
        digitalWrite(redLed,LOW);
        digitalWrite(greenLed,HIGH);
        if(screenOffMsg1==0){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Alarma activata");
          screenOffMsg1=1;
        }
          keypressed= myKeypad.getKey();
          if(keypressed=='A'){
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Dezactivare");
            enterPassword();
            if(valid){
              alarmActivated=false;
              screenOffMsg=0;
            }
            
          }
        
    }
    
}
void enterPassword() {
      int k=5;
      tempPassword = "";
      activated = true;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(" *** ALARM *** ");
      lcd.setCursor(0,1);
      lcd.print("Pass>");
          while(activated) {
          keypressed = myKeypad.getKey();
          if (keypressed != NO_KEY){
            if (keypressed == '0' || keypressed == '1' || keypressed == '2' || keypressed == '3' ||
                keypressed == '4' || keypressed == '5' || keypressed == '6' || keypressed == '7' ||
                keypressed == '8' || keypressed == '9' ) {
              tempPassword += keypressed;
              lcd.setCursor(k,1);
              lcd.print("*");
              k++;
            }
          }
          if (k > 9 || keypressed == '#') {
            tempPassword = "";
            k=5;
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print(" *** ALARM *** ");
            lcd.setCursor(0,1);
            lcd.print("Pass>");
          }
          if ( keypressed == '*') {
            if ( tempPassword == password ) {
              activated = false;
              valid=true;
            }
            else if (tempPassword != password) {
              lcd.setCursor(0,1);
              lcd.print("Wrong! Try Again");
              delay(2000);
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print(" *** ALARM *** ");
              lcd.setCursor(0,1);
              lcd.print("Pass>");
              valid=false;
            }
          }    
        }
    }
   long getDistance(){
      //int i=10;
      
      //while( i<=10 ) {
      // Clears the trigPin
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      // Sets the trigPin on HIGH state for 10 micro seconds
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      // Reads the echoPin, returns the sound wave travel time in microseconds
      duration = pulseIn(echoPin, HIGH);
      // Calculating the distance
      distance = duration*0.034/2;
      //sumDistance += distance;
      //}
      //int averageDistance= sumDistance/10;
      return distance;
    }
/* long getDistance(){
    digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
// Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
// Calculating the distance
  distance= duration*0.034/2;
// Prints the distance on the Serial Monitor
    return distance;
 }*/

  void activareAlarma(int j){
    while(j>=0){
      delay(200);
      loop_cnt++;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Alarma se");
      lcd.setCursor(0,1);
      lcd.print("activeaza in:");   
      lcd.print(j);

        
        
        if (loop_cnt==5){
          loop_cnt=0;
          j--;
          tone(buzzer,2000);
        }  else 
          {
            noTone(buzzer);
          }
    }
  
        initialDistance = 10;
        noTone(buzzer);
        alarmActivated=true;
        screenOffMsg1=0;
        
      
  }
 
