/*   
    L = Left 
    R = Right 
    F = forward 
    B = backward
*/
#include <Servo.h> 
int pinLB = 12;     // define pin 12 
int pinLF = 3;     // define pin 3 
int pinRB = 13;    // define pin 13 
int pinRF = 11;    // define pin 11 
int inputPin = 4;    // define pin for sensor echo
int outputPin =5;    // define pin for sensor trig
int Fspeedd = 0;      // forward speed
int Rspeedd = 0;      // right speed
int Lspeedd = 0;      // left speed
int directionn = 0;   // forward=8 backward=2 left=4 right=6 
Servo myservo;        // set myservo
int delay_time = 250; // settling time after steering servo motor moving B
int Fgo = 8;         // Move F
int Rgo = 6;         // move to the R
int Lgo = 4;         // move to the L
int Bgo = 2;         // move B

int NNNN = 11;     //Nort 
int NNNEE = 12;     //Nort East
int NNNWW = 13;     //Nort West
int EE = 14;     //East
int WW = 15;     //Weast
int SSS = 16;      //South
int SSSEE = 17;       //Sout East
int SSSWW = 18;       //Sout West
int mode = 0;     // mode 0 is AI mode 1 is RC
char serialInput = 0; //
void setup()
 {
  Serial.begin(9600);     // Define motor output pin 
  pinMode(pinLB,OUTPUT); // pin 12
  pinMode(pinLF,OUTPUT); // pin 3 (PWM)
  pinMode(pinRB,OUTPUT); // pin 13
  pinMode(pinRF,OUTPUT); // pin 11 (PWM) 
  pinMode(inputPin, INPUT);    // define input pin for sensor
  pinMode(outputPin, OUTPUT);  // define output pin for sensor   
  myservo.attach(9);    // Define servo motor output pin to D9 (PWM)
 Serial.println("just Finish setup");
 delay(1000);
 }
void advance()     // move forward
    { 
    digitalWrite(pinLB,LOW);    // right wheel moves forward
    digitalWrite(pinRB, LOW);  // left wheel moves forward
    analogWrite(pinLF,255 -51);   
    analogWrite(pinRF,255);
    Serial.println("just Finish advance");
    }
void NorthEast(){
      digitalWrite(12,LOW);
      digitalWrite(13,LOW);
      analogWrite(3,165 - 51);
      analogWrite(11,(255));  
      Serial.println("just Finish North East");  
    }

void NorthWest(){
      digitalWrite(12,LOW);
    digitalWrite(13,LOW);
    analogWrite(3,255 - 51);
    analogWrite(11,165);
    Serial.println("just Finish North West");
    }
void stopp()         // stop 
    {
     digitalWrite(pinLB,HIGH);
     digitalWrite(pinRB,HIGH);
     analogWrite(pinLF,0);
     analogWrite(pinRF,0); 
     Serial.println("just Finish stop");
    }
void right()        // turn right (single wheel)
    {
   digitalWrite(pinLB,HIGH);  // wheel on the left moves forward
   digitalWrite(pinRB,LOW); // wheel on the right moves backward
   analogWrite(pinLF, 255 -51);
   analogWrite(pinRF,255);   
   Serial.println("just Finish right");
    }
void left()         // turn left (single wheel)
    {
   digitalWrite(pinLB,LOW);  // wheel on the left moves backward
   digitalWrite(pinRB,HIGH); // wheel on the right moves forward
   analogWrite(pinLF, 255 -51);
   analogWrite(pinRF,255);  
   Serial.println("just Finish left");
    }
  
void back()          // move backward
    {
     digitalWrite(pinLB,HIGH);  // motor moves to left rear
     digitalWrite(pinRB,HIGH);  // motor moves to right rear
     analogWrite(pinLF,255 -51);  
     analogWrite(pinRF,255);  
     Serial.println("just Finish back");
    }
void SouthEast(){
        digitalWrite(12 , HIGH);//pinLB=12, LOW  right wheel moves forward
        digitalWrite(13,  HIGH);  // pinRB=13, LOW left wheel moves forward
        analogWrite (3,165 - 51);
        analogWrite (11,255);
        Serial.println("just Finish South East");
    }
void SouthWest(){
        digitalWrite(12 , HIGH);//pinLB=12, LOW  right wheel moves forward
        digitalWrite(13,  HIGH);  // pinRB=13, LOW left wheel moves forward
        analogWrite (3,255);
        analogWrite (11,(165-34));
        Serial.println("just Finish South West");
    }
void detection()        // measure 3 angles (0.90.179)
    {      
      Serial.println("just Enter Detectin");
      delay(10);
      int delay_time = 250;   // stabilizing time for servo motor after moving backward
      ask_pin_F();            // read the distance ahead
      Serial.println("Inside dectection after ask_pin_F()");
     if(Fspeedd < 25 || Fspeedd < 10 )     //default 10    // if distance ahead is <10cm stop and go back
      {
              Serial.println("Inside dectection after if(Fspeed < 10");
            stopp();               // clear data 
            delay(100);
            Serial.println("Inside dectection after Fspeed < 10 stopp()");
             delay(10);
            back();                // move backward for 0.2S
            delay(200);
      }
      if(Fspeedd < 35 || Fspeedd < 45)      //defualt 25   // if distance ahead is <25cm stop and loop left and right
      {
        Serial.println("Inside dectection after if(Fspeed < 25");
        delay(10);
        stopp();  
        Serial.println("Inside dectection after Fspeed < 25 stopp()");
       delay(10);
        delay(100);             // clear data 
        ask_pin_L();            // read distance on the left
        delay(delay_time);      // stabilizing time for servo motor
        ask_pin_R();            // read distance on the right  
        delay(delay_time);      // stabilizing time for servo motor  
        
        if(Lspeedd > Rspeedd)   // if distance on the left is >distance on the right
        {
          directionn = Lgo;      // move to the L
        }
         if(Lspeedd <= Rspeedd)   // if distance on the left is <= distance on the right
        {
         directionn = Rgo;      // move to the right
        } 
        if (Lspeedd < 10 && Rspeedd < 10)   // if distance on left and right are both <10cm
        {
         directionn = Bgo;      // move backward        
        } }
      else                      // if distance ahead is >25cm     
      {
        directionn = Fgo;        // move forward      
      }}    
void ask_pin_F()   // measure the distance ahead 
    {
      myservo.write(90); // default 90
      digitalWrite(outputPin, LOW);   // ultrasonic sensor transmit low level signal 2μs
      delayMicroseconds(2);
      digitalWrite(outputPin, HIGH);  // ultrasonic sensor transmit high level signal10μs, at least 10μs
      delayMicroseconds(10);
      digitalWrite(outputPin, LOW);    // keep transmitting low level signal
      float Fdistance = pulseIn(inputPin, HIGH);  // read the time in between
      Fdistance= Fdistance/5.8/10;       // convert time into distance (unit: cm) 
      Fspeedd = Fdistance;              // read the distance into Fspeedd
  Serial.print("Fspeedd = ");
  Serial.print(Fspeedd );
  Serial.println("  cm");   
    }  
 void ask_pin_L()   // measure distance on the left 
    {
      myservo.write(25); // default 5
      delay(325);  // default int delay_time = 250
      digitalWrite(outputPin, LOW);   // ultrasonic sensor transmit low level signal 2μs
      delayMicroseconds(2);
      digitalWrite(outputPin, HIGH);  // ultrasonic sensor transmit high level signal10μs, at least 10μs
      delayMicroseconds(10);
      digitalWrite(outputPin, LOW);    // keep transmitting low level signal
      float Ldistance = pulseIn(inputPin, HIGH);  // read the time in between
      Ldistance= Ldistance/5.8/10;       // convert time into distance (unit: cm)
      Lspeedd = Ldistance;              // read the distance into Lspeedd
  Serial.print("Lspeedd = ");
  Serial.print(Lspeedd );
      Serial.print("  cm  ");
    }  
void ask_pin_R()   //  measure distance on the right 
    {
      myservo.write(122); // default 177
      delay(325); // default int delay_time = 250
      digitalWrite(outputPin, LOW);   // ultrasonic sensor transmit low level signal 2μs
      delayMicroseconds(2);
      digitalWrite(outputPin, HIGH);  // ultrasonic sensor transmit high level signal10μs, at least 10μs
      delayMicroseconds(10);
      digitalWrite(outputPin, LOW);    // keep transmitting low level signal
      float Rdistance = pulseIn(inputPin, HIGH);  // read the time in between
      Rdistance= Rdistance/5.8/10;       // convert time into distance (unit: cm)
      Rspeedd = Rdistance;              // read the distance into Rspeedd
  Serial.print(" Rspeedd = ");
  Serial.print(Rspeedd );
      Serial.println("  cm");
    }  
void loop()
  {
    Serial.println("We are in the main loop");
    delay(10);

  if ( mode == 0) {
    
            myservo.write(90);  // home set the servo motor, ready for next measurement
            detection();        // measure the angle and determine which direction to move  
            Serial.println("We are in the main loop after calling detectin();");
            delay(100); 
           if(directionn == 2)  // if directionn= 2             
           {
            Serial.println("We are in the main loop were direction == 2 ");
            delay(10);
             back(); 
             delay(800);                    //  go backward
             left() ;      
             delay(110);        //Default 200      // Move slightly to the left (to prevent stuck in dead end)
           }
           if(directionn == 6)           // if directionn = 6    
           { back();
             delay(100);  
             right();  
             delay(110);                 // turn right Default 600
           }
           if(directionn == 4)          // if directionn = 4    
           { back(); 
             delay(600);      
             left(); 
             delay(110);                  // turn left Default 600
           }  
           if(directionn == 8)          // if directionn = 8      
           { advance();       // move forward  
            delay(50);  // default 100
           }
      }
      if ( mode != 0 ){ // if we are not in RC from BT
        delay(10);
            switch ( mode) {
              case 0:   // AI
              loop();
              break;
              case 1:   // advance
              advance();
              delay(300);
              mode = 9;
              break;
              case 2:// north east
              NorthEast();
              delay(300);
              mode = 9;;
              break ;
              case 3:     //north east
              NorthWest();
              delay(300);
              mode = 9;
              break;
              case 4:     //right
              right();
              delay(300);
              mode = 9;
              break;
              case 5:     // left
              left();
              delay(300);
              mode = 9;
              break;
              case 6:  // back
              back();
              delay(300);
              mode = 9;
              case 7:  // south east
              SouthEast();
              delay(300);
              mode = 9;
              break;
              case 8: // south west
              SouthWest();
              delay(300);
              mode = 9;
              break;
              case 9: 
              stopp();
              delay(50);
              break;
              
            }
      }
    
    }


   void serialEvent(){ // seriaEvent is like interup just have to wait for excuting functions to finish
    String serialInput01 = "";
    while (Serial.available() > 0){
      serialInput01 = Serial.readString();
      mode = serialInput01.toInt(); // 0 will be AI robot
    }
    
    //Serial.println("SERIAL EVENT:" + serialInput01);
    //Serial.println(mode);
    //1
    delay(1);
   }

