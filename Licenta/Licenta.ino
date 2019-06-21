    #include <LiquidCrystal.h> // includes the LiquidCrystal Library 
    #include <Keypad.h>
    #include <Adafruit_Fingerprint.h>
    #define buzzer 9
    #define trigPin 24
    #define echoPin 22
    #define redLed 13
    #define greenLed 12
    long duration;
    int distance, initialDistance, currentDistance, i, loop_cnt=0, j;
    int fingerID;
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
    SoftwareSerial mySerial(10, 11);
    Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

    void setup() { 
      lcd.begin(16,2);     
      pinMode(redLed, OUTPUT);
      pinMode(greenLed, OUTPUT);
      pinMode(buzzer, OUTPUT); // Set buzzer as an output
      pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
      pinMode(echoPin, INPUT);
      
      Serial.begin(9600);
  
      while (!Serial);  // For Yun/Leo/Micro/Zero/...
        delay(100);
        Serial.println("\n\nAdafruit finger detect test");

        // set the data rate for the sensor serial port
        finger.begin(57600);
  
         if (finger.verifyPassword()) {
            Serial.println("Found fingerprint sensor!");
            } else {
                    Serial.println("Did not find fingerprint sensor :(");
                    while (1) { delay(1); }
                   }

       finger.getTemplateCount();
       Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
       Serial.println("Waiting for valid finger...");// Sets the echoPin as an Input
            
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
              lcd.print("C-Parola");
              lcd.setCursor(0,1);
              lcd.print("D-Amprenta");
            }
            if(keypressed=='C'){
              enterPassword();
              if(valid){  
                loop_cnt=0;
                activareAlarma(9);
              }
            }else if(keypressed=='D'){    
              detectFingerPrint();
            }else if(keypressed=='#'){
              screenOffMsg=0;
            } else if (keypressed =='B') {
      lcd.clear();
      int i=1;
      tone(buzzer, 2000, 100);
      tempPassword = "";
      lcd.setCursor(0,0);
      lcd.print("Parola curenta");
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
          lcd.print("Parola noua");
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
              lcd.print("Parola noua");
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
   }
  
   
    

             
            
      
      else { 
        valid=false;
        digitalWrite(redLed,LOW);
        digitalWrite(greenLed,HIGH);
        if(screenOffMsg1==0){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Alarma activata");
          screenOffMsg1=1;
        } lcd.setCursor(0,1);
          lcd.print("A-Dezactivare");
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
      int k=7;
      tempPassword = "";
      activated = true;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Introduceti");
      lcd.setCursor(0,1);
      lcd.print("Parola>");
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
          if (k > 11 || keypressed == '#') {
            tempPassword = "";
            k=5;
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Introduceti");
            lcd.setCursor(0,1);
            lcd.print("Parola>");
          }
          if ( keypressed == '*') {
            if ( tempPassword == password ) {
              activated = false;
              valid=true;
            }
            else if (tempPassword != password) {
              lcd.setCursor(0,1);
              lcd.print("Parola gresita");
              delay(2000);
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("Introduceti");
              lcd.setCursor(0,1);
              lcd.print("Parola>");
              valid=false;
            }
          }    
        }
    }
   long getDistance(){
     
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      
      duration = pulseIn(echoPin, HIGH);
      
      distance = duration*0.034/2;
      
      return distance;
    }

  void activareAlarma(int j){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Alarma se");
      lcd.setCursor(0,1);
      lcd.print("activeaza in:");   
      
      while(j>=0){
       delay(50);
       loop_cnt++;
       lcd.setCursor(13,1);
       lcd.print(j);
        if (loop_cnt==20){
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



 uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence); 

  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID; 
}


void detectFingerPrint(){
   //finger.fingerID=0;
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("Detectare");
     lcd.setCursor(5,1);
     lcd.print("amprenta");
      while(!alarmActivated){
        delay(50);
        fingerID=getFingerprintIDez();
        if (fingerID == 1 || fingerID==2 || fingerID==3)
           activareAlarma(9);
    }
}
