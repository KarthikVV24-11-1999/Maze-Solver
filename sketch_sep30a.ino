#define fthold 5     // Threshold value in front direction
#define rthold 2     // Threshold value in right direction
#define lthold 2     // Threshold vlaue in left direction
#define bthold 5     // Threshold vlaue in back direction
const int trigPinf = 2;          
const int echoPinf = 6;   

// Right US sensor.
const int trigPinr = 8;           
const int echoPinr = 5;

 // Left US sensor.
const int trigPinl =  4;           
const int echoPinl =  9;   

 // Back US sensor.
const int trigPinb =  7;           
const int echoPinb =  3; 

struct cell{
  int index;
  char dir;
  int front;
  int left;
  int right;
  int back;
};

bool fsensor;       // For the front US sensor.
bool rsensor;       // For the right US sensor.
bool lsensor;       // For the left US sensor.
bool bsensor;       // For the back US sensor.

int fblocks;
int rblocks;
int lblocks;
int bblocks;

struct cell *current_cell = (struct cell*)malloc(sizeof(struct cell));
struct cell **block = (struct cell**)malloc(4*sizeof(struct cell*));

float frontdist()
{
 float gapf;float ticktockf;

 digitalWrite(trigPinf,LOW);
 delayMicroseconds(2);
 digitalWrite(trigPinf,HIGH);
 delayMicroseconds(10);
 digitalWrite(trigPinf,LOW);

ticktockf = pulseIn(echoPinf,HIGH);    // in one cm there are 29 microseconds. 
 gapf = ticktockf*0.0344/2;

 return gapf;  
}
// Returns the distance the wall to its right side
 float rightdist()
 { 
   float gapr;float ticktockr;
   digitalWrite(trigPinr,LOW);
   delayMicroseconds(2);
   digitalWrite(trigPinr,HIGH);
   delayMicroseconds(10);
   digitalWrite(trigPinr,LOW);

   ticktockr = pulseIn(echoPinr,HIGH);
   gapr = ticktockr*0.0344/2;

   return gapr;
 }

// Returns the distance of the wall to its left side
float leftdist()
 {  
   float gapl;float ticktockl;
   
   digitalWrite(trigPinl,LOW);
   delayMicroseconds(2);
   digitalWrite(trigPinl,HIGH);
   delayMicroseconds(10);
   digitalWrite(trigPinl,LOW);

   ticktockl = pulseIn(echoPinl,HIGH);
   gapl = ticktockl*0.0334/2;

   return gapl; 
 }
// Returns the distance the wall to its back side
 float backdist()
 { 
   float gapb;float ticktockb;
   digitalWrite(trigPinb,LOW);
   delayMicroseconds(2);
   digitalWrite(trigPinb,HIGH);
   delayMicroseconds(10);
   digitalWrite(trigPinb,LOW);

   ticktockb = pulseIn(echoPinb,HIGH);
   gapb = ticktockb*0.0344/2;

   return gapb;
 }
 
float middleval(float arr[])
{
   for(int p=0;p<4;p++)
    {
        for(int q=0;q<4;q++)
        {
            if(arr[q]>arr[q+1])
            {
                int temp = arr[q];
                arr[q] = arr[q+1];
                arr[q+1] = temp;
            }
        }
    }
  return arr[2]; // Median value.
}
void setup(){
  pinMode (trigPinf,OUTPUT);   
  pinMode (echoPinf,INPUT);
  pinMode (trigPinr,OUTPUT);   
  pinMode (echoPinr,INPUT);
  pinMode (trigPinl,OUTPUT);   
  pinMode (echoPinl,INPUT);
  pinMode (trigPinb,OUTPUT);   
  pinMode (echoPinb,INPUT);
  pinMode(10,OUTPUT);  
  pinMode(11,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT);
  Serial.begin(9600);
}

void loop(){
  float fdist;    float rdist;    float ldist;  float bdist;      // front, right, and left distances.
  float fduration;float rduration;float lduration;float bduration;  // front, right, and left travel time in echoPin.
  float fdur[5];  float rdur[5];  float ldur[5];  float bdur[5];    // Arrays which store the values of five durations... We will take only the median value(afer sorting) with error bearing capacity of 40%.


//  for(int i=0;i<=4;i++)
//  {
//   digitalWrite(trigPinf,LOW);         // Clearing the trigPin.
//   delayMicroseconds(5); 
//   digitalWrite(trigPinf,HIGH);        // Setting the trigPin HIGH for 10 microseconds..sends some 8cycle sonics.
//   delayMicroseconds(10); 
//   digitalWrite(trigPinf,LOW);
//  
//  fdur[i] = pulseIn(echoPinf,HIGH);    // Returns the time for which the wave travelled.
//  }
//
//  fduration = middleval(fdur);
//  fdist = fduration*0.0344/2;          // Distance of the wall in the forward direction
//                                    



  for(int i=0;i<=4;i++)
  {
    fdur[i] = frontdist();
    rdur[i] = rightdist();
    ldur[i] = leftdist();
    bdur[i] = backdist();
  }
  
  fdist = middleval(fdur);
  rdist = middleval(rdur);
  ldist = middleval(ldur);
  bdist = middleval(bdur);
  
  Serial.print("frontdistance: ");
  Serial.println(fdist);
  Serial.print("rightdistance: ");
  Serial.println(rdist);
  Serial.print("leftdistance: ");
  Serial.println(ldist);
  Serial.print("backdistance: ");
  Serial.println(bdist);
  Serial.println("");

  
  if((fdist>=125)||(rdist>=150)||(ldist>=400)) {return;}    // Cancelling out any error values...goes back to void loop().

  fsensor = false;rsensor = false;lsensor = false;bsensor = false;          // Setting up the booleans.

  if(rdist<=rthold) rsensor = true; 
  if(ldist<=lthold) lsensor = true;            
  if(fdist<=fthold) fsensor = true;
  if(bdist<=bthold) bsensor = true;

  fblocks = round((fdist - fthold)/25.0);
  rblocks = round((rdist - rthold)/25.0);
  lblocks = round((ldist - lthold)/25.0);
  bblocks = round((bdist - bthold)/25.0);
  
  Serial.print("frontblocks: ");
  Serial.println(fblocks);
  Serial.print("rightblocks: ");
  Serial.println(rblocks);
  Serial.print("leftblocks: ");
  Serial.println(lblocks);
  Serial.print("backblocks: ");
  Serial.println(bblocks);
  Serial.println("");
  Serial.println("");
  
  struct cell *temp_cell = (struct cell*)malloc(sizeof(struct cell));

  temp_cell->front = fblocks;
  temp_cell->left = lblocks;
  temp_cell->right = rblocks;
  temp_cell->back = bblocks;

  free(temp_cell);

  if(temp_cell->front>0){
    
  }
}
