#include <AccelStepper.h>


AccelStepper stepper(AccelStepper::DRIVER,7,6);// Using Extranla Driver Set to AccelStepper::DRIVER (2 pins) on 7,6


const int upSpeed = 30; // um/s
const int downSpeed = 1000; // um/s (unsigned)
const int upupSpeed = 1000;
const unsigned long runtime = 1400; // s

char incomingByte;   // for incoming serial data
unsigned long myTime; // for tracking current time
boolean isRun = false; // check if is running



void setup() {
  //Stepper motor setup
   stepper.setMaxSpeed(4000);
   stepper.setSpeed(0);  

  // put your setup code here, to run once:
  Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  //test something
  
}

void loop() {
  // put your main code here, to run repeatedly:
          // send data only when you receive data:
        if (Serial.available() > 0) {
                // read the incoming byte:
                incomingByte = Serial.read();

                // say what you got:
                Serial.print("I received: ");
                Serial.println(incomingByte);
                

                if (incomingByte == 'u' || incomingByte == 'U'){
                    digitalWrite(LED_BUILTIN, HIGH);
                    stepper.setSpeed(upSpeed);
                    myTime = millis(); 
                    isRun = true;
                    Serial.print("Start time: ");
                    Serial.println(myTime/1000);
                           
                }
        
                else if (incomingByte == 'd' || incomingByte == 'D'){
                    digitalWrite(LED_BUILTIN, LOW);
                    stepper.setSpeed(-downSpeed);
                    myTime = millis(); 
                    isRun = true;
                    Serial.print("Start time: ");
                    Serial.println(myTime/1000); 
                } 
        
                else if (incomingByte == 's' || incomingByte == 'S'){
                    digitalWrite(LED_BUILTIN, LOW);
                    stepper.setSpeed(0);
                    isRun = false;
                } 

                else if (incomingByte == 't' || incomingByte == 'T'){
                    digitalWrite(LED_BUILTIN, HIGH);
                    stepper.setSpeed(upupSpeed);
                    myTime = millis(); 
                    isRun = true;
                    Serial.print("Start time: ");
                    Serial.println(myTime/1000);
                } 
                
        
                else{
                    digitalWrite(LED_BUILTIN, LOW);
                    stepper.setSpeed(0); 
                    isRun = false;
                } 

                
        }
        
         


        if ((millis() - myTime > runtime*1000) && (isRun == true)) {
            stepper.setSpeed(0);
            myTime = millis();
            isRun = false;
            
            Serial.print("run_finish, time is:");
            Serial.println(myTime/1000); 
            
            digitalWrite(LED_BUILTIN, LOW);
        }
        stepper.runSpeed();  
}
