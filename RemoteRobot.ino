//  Integration of motor driver and remote ir control
//  Authors: Paige Dungan, and Scott Dungan
//  Date: 24  May 2015
//  Version 0.1
//
//


#include <MotorDriver.h>
#include <IRLib.h>

IRrecv My_Receiver(7);//receiver on pin 7
const int pwPin = 2;
IRdecode My_Decoder;//Decoder object
long pulse, inches;
boolean movingForward, obstacleDetected = false; 

void setup()
{
        //Initialize serial communication 
        Serial.begin(9600);
        delay(1000);while(!Serial);
        
        //Initialize the motor system
	/*Configure the motor A to control the wheel at the left side.*/
	/*Configure the motor B to control the wheel at the right side.*/
	motordriver.init();
	motordriver.setSpeed(254,MOTORB);
	motordriver.setSpeed(254,MOTORA);

        //Initialize the ir system
        My_Receiver.enableIRIn(); // Start the receiver
        
        pinMode(pwPin, INPUT);
}
 
void loop()
{
  //Collision detector
  pulse = pulseIn(pwPin, HIGH);
  inches = pulse/147;
  
  if ((inches <= 7)& (movingForward == true)){   
    Serial.print("DANGER CLOSE\n");
    motordriver.stop();
    movingForward = false;
    obstacleDetected = true;
  }else{
     obstacleDetected = false;
  }

  
  //Command processor
  if (My_Receiver.GetResults(&My_Decoder)) {
    Serial.print("Something to decode\n");
    My_Decoder.decode();
    My_Decoder.DumpResults();
    if (My_Decoder.decode_type== JVC) {
      Serial.print(My_Decoder.value);
      Serial.print("\Something to decode\n");
      switch(My_Decoder.value) {
        case 0xf684:  //button 1 - forward half left
          if (obstacleDetected == true){
              break;
          }        
	  motordriver.setSpeed(180,MOTORB);
	  motordriver.setSpeed(254,MOTORA);
          motordriver.goForward();
          movingForward = true;
          break;
        case 0xf644:  //button 2 - forward
	  if (obstacleDetected == true){
            break;
          }
          motordriver.setSpeed(254,MOTORB);
	  motordriver.setSpeed(254,MOTORA);
          motordriver.goForward();
          movingForward = true;
          break;
        case 0xf6c4:  //button 3 - forward half right
          if (obstacleDetected == true){
              break;
          }
	  motordriver.setSpeed(254,MOTORB);
	  motordriver.setSpeed(180,MOTORA);
          motordriver.goForward();
          movingForward = true;
          break;
        case 0xf624:  //button 4 - left
	  motordriver.setSpeed(100,MOTORB);
	  motordriver.setSpeed(100,MOTORA);
          motordriver.goLeft();
          movingForward = false;
          break;
        case 0xf664:  //button 6 - right
	  motordriver.setSpeed(100,MOTORB);
	  motordriver.setSpeed(100,MOTORA);
          motordriver.goRight();
          movingForward = false;
          break;
        case 0xf614:  //button 8 - reverse
	  motordriver.setSpeed(254,MOTORB);
	  motordriver.setSpeed(254,MOTORA);          
          motordriver.goBackward();
          movingForward = false;
          break;
        case 0xf6e4:  //button 7 - reverse half left
	  motordriver.setSpeed(180,MOTORB);
	  motordriver.setSpeed(254,MOTORA);          
          motordriver.goBackward();
          movingForward = false;
          break;
        case 0xf694:  //button 9 - reverse half right
	  motordriver.setSpeed(254,MOTORB);
	  motordriver.setSpeed(180,MOTORA);          
          motordriver.goBackward();
          movingForward = false;
          break;          
        case 0xf6a4:  //button 5 - stop
          motordriver.stop();
          movingForward = false;
          break;          
      }
    }
    Serial.print("\Resuming to decode\n");  
    My_Receiver.resume(); //Restart the receiver  
  }
}

