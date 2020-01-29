#define fthold 5     // Threshold value in front direction
#define rthold 5     // Threshold value in right direction
#define lthold 5     // Threshold vlaue in left direction
#define bthold 5     // Threshold vlaue in back direction

// Front US sensor.
const int trigPinf = 7;          
const int echoPinf = 3;   

// Right US sensor.
const int trigPinr = 8;           
const int echoPinr = 5;

 // Left US sensor.
const int trigPinl =  4;           
const int echoPinl =  9;   

 // Back US sensor.
const int trigPinb =  2;           
const int echoPinb =  6; 

void setup() {
  pinMode(trigPinf,OUTPUT);   
  pinMode(echoPinf,INPUT);
  pinMode(trigPinr,OUTPUT);   
  pinMode(echoPinr,INPUT);
  pinMode(trigPinl,OUTPUT);   
  pinMode(echoPinl,INPUT);
  pinMode(trigPinb,OUTPUT);   
  pinMode(echoPinb,INPUT);
  pinMode(10,OUTPUT);  
  pinMode(11,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT);

}

void loop() {
      digitalWrite(10,HIGH);
      digitalWrite(11,LOW);
      digitalWrite(12,HIGH);
      digitalWrite(13,LOW);
}
