#define fthold 5     // Threshold value in front direction
#define rthold 2     // Threshold value in right direction
#define lthold 2     // Threshold vlaue in left direction
#define bthold 5     // Threshold vlaue in back direction

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


int kar=0;int kar1=0;int kar2 = 0;int x=0;int final;int count=0;int present_count=0; int current_row; int current_column; int current_dir;      
int **arr;  
int link[30]; 
float fdist;    float rdist;    float ldist;  float bdist;      // front, right, and left distances.
float fdur[5];  float rdur[5];  float ldur[5];  float bdur[5];    // Arrays which store the values of five durations... We will take only the median value(afer sorting) with error bearing capacity of 40%.                      


int fblocks;int rblocks;int lblocks;int bblocks;

 
float middleval(float mid[])
{
   for(kar2=0;kar2<4;kar2++)
    {
        for(kar1=0;kar1<4;kar1++)
        {
            if(mid[kar1]>mid[kar1+1])
            {
                int temp = mid[kar1];
                mid[kar1] = mid[kar1+1];
                mid[kar1+1] = temp;
            }
        }
    }
  return mid[2]; // Median value.
}




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

void check(int row, int column, int dir){
  int remainder;
  int y = x;
  for(kar2 = 0; kar2<dir; kar2++){
    remainder = y%10;
    y = y/10;
    y = 1000*remainder + y;
  }

  if(y == arr[row][column]){
    link[present_count] = row*1000+column*10+dir;
    present_count++;
  }

return; 
  
}

void find_blocks(){

  for(kar2=0;kar2<=4;kar2++)
  {
    fdur[kar2] = frontdist();
    rdur[kar2] = rightdist();
    bdur[kar2] = backdist();
    ldur[kar2] = leftdist();
  }
  fdist = middleval(fdur);
  rdist = middleval(rdur);
  bdist = middleval(bdur);
  ldist = middleval(ldur);

  fblocks = round((fdist - fthold)/25.0);
  rblocks = round((rdist - rthold)/25.0);
  bblocks = round((bdist - bthold)/25.0);
  lblocks = round((ldist - lthold)/25.0);
  
  Serial.print("frontblocks: ");
  Serial.println(fblocks);
  Serial.print("rightblocks: ");
  Serial.println(rblocks);
  Serial.print("backblocks: ");
  Serial.println(bblocks);
  Serial.print("leftblocks: ");
  Serial.println(lblocks);

  x = 1000*fblocks+10*bblocks;     
    
  return;
}

void fill(int row, int column, int a, int b, int c, int d){
  arr[row][column] = 1000*a+100*b+10*c+1*d;
  
  if(1000*a+100*b+10*c+1*d == x){
    link[count] = row*1000+column*10+1;
    count++;
  }

  else if(1000*b+100*c+10*d+1*a == x){
    link[count] = row*1000+column*10+2;
    count++;
  }

  else if(1000*c+100*d+10*a+1*b == x){
    link[count] = row*1000+column*10+3;  
    count++;
  }

  else if(1000*d+100*a+10*b+1*c == x){
    link[count] = row*1000+column*10+4;
    count++;
  }

  return;
}


void setup(){
  
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

  Serial.begin(9600);
  Serial.print("a\n");
  
  arr = (int **)malloc(15 * sizeof(int * )); 
  for (kar=0; kar<15; kar++) {
      arr[kar] = (int *)malloc(15 * sizeof(int ));
  }
  
  Serial.print("b\n");
  find_blocks();
  fill(0,0,0,0,1,0);fill(0,1,0,1,0,0);fill(0,2,0,5,0,1);fill(0,3,0,4,0,2);fill(0,4,0,3,0,3);fill(0,5,0,2,0,4);fill(0,6,0,1,1,5);fill(0,7,0,0,3,6);fill(0,8,0,0,2,0);fill(0,9,0,4,4,0);fill(0,10,0,3,1,1);fill(0,11,0,2,0,2);fill(0,12,0,1,0,3);fill(0,13,0,0,1,4);fill(0,14,0,0,5,0);fill(1,0,1,1,0,0); fill(1,1,0,0,2,1); fill (1,2,0,4,1,0); fill(1,3,0,3,0,1); fill(1,4,0,2,0,2); fill(1,5,0,1,0,3); fill(1,6,1,0,0,4); fill(1,7,1,1,2,0) ;fill(1,8,1,0,1,1); fill(1,9,1,0,3,0); fill(1,10,1,1,0,0); fill(1,11,0,0,1,1); fill(1,12,0,1,1,0); fill(1,13,1,0,0,1); fill(1,14,1,0,4,0); fill(2,0,0,0,5,0); fill(2,1,1,0,1,0); fill(2,2,1,1,0,0); fill(2,3,0,0,2,1); fill(2,4,0,2,0,0); fill(2,5,0,1,0,1); fill(2,6,0,0,3,2); fill(2,7,2,0,1,0); fill(2,8,2,1,0,0); fill(2,9,2,0,2,1); fill(2,10,0,1,2,0); fill(2,11,1,0,0,1); fill(2,12,1,2,0,0); fill(2,13,0,1,0,1); fill(2,14,2,0,3,2); fill(3,0,1,0,4,0); fill(3,1,2,4,0,0); fill(3,2,0,3,2,1); fill(3,3,1,2,1,2); fill(3,4,0,1,0,3); fill(3,5,0,0,2,4); fill(3,6,1,0,2,0); fill(3,7,3,1,0,0); fill(3,8,0,0,1,1); fill(3,9,3,4,1,0); fill(3,10,1,3,1,1); fill(3,11,0,2,0,2); fill(3,12,0,1,1,3); fill(3,13,0,0,0,4); fill(3,14,3,8,2,0); fill(4,0,2,1,3,0); fill(4,1,0,0,1,1); fill(4,2,1,0,1,0); fill(4,3,2,1,0,0); fill(4,4,0,0,0,1); fill(4,5,1,0,1,0); fill(4,6,2,2,1,0); fill(4,7,0,1,0,1); fill(4,8,1,0,0,2); fill(4,9,4,0,0,0); fill(4,10,2,1,0,0); fill(4,11,0,0,1,1); fill(4,12,1,2,0,0); fill(4,13,0,1,0,1); fill(4,14,4,0,1,2); fill(5,0,3,0,2,0); fill(5,1,1,0,0,0); fill(5,2,2,1,0,0); fill(5,3,0,0,1,1); fill(5,4,0,4,1,0); fill(5,5,2,3,0,1); fill(5,6,3,2,0,2); fill(5,7,0,1,2,3); fill(5,8,0,0,1,4); fill(5,9,0,1,1,0); fill(5,10,0,0,2,1); fill(5,11,1,2,0,0); fill(5,12,0,1,0,1); fill(5,13,0,0,1,2); fill(5,14,5,0,0,0); fill(6,0,4,2,1,0); fill(6,1,0,1,0,1); fill(6,2,0,0,1,2); fill(6,3,1,2,0,0); fill(6,4,1,1,0,1); fill(6,5,0,0,1,2); fill(6,6,0,1,2,0); fill(6,7,1,0,1,1); fill(6,8,1,1,0,0); fill(6,9,1,0,0,1); fill(6,10,1,0,1,0); fill(6,11,0,1,2,0); fill(6,12,0,0,1,1); fill(6,13,1,1,0,0); fill(6,14,0,0,4,1); fill(7,0,5,0,0,0); fill(7,1,0,4,1,0); fill(7,2,1,3,0,1); fill(7,3,0,2,5,2); fill(7,4,0,1,0,3); fill(7,5,1,0,0,4); fill(7,6,1,0,1,0); fill(7,7,2,3,0,0); fill(7,8,0,2,0,1); fill(7,9,0,1,1,2); fill(7,10,2,0,0,3); fill(7,11,1,0,1,0); fill(7,12,1,1,0,0); fill(7,13,0,0,1,1); fill(7,14,1,0,3,0); fill(8,0,0,1,6,0); fill(8,1,1,0,0,1); fill(8,2,0,0,3,0); fill(8,3,1,0,4,0); fill(8,4,0,2,5,0); fill(8,5,0,1,4,1); fill(8,6,2,0,0,2); fill(8,7,0,0,1,0); fill(8,8,0,3,1,0); fill(8,9,1,2,0,1); fill(8,10,0,1,1,2); fill(8,11,2,0,0,3); fill(8,12,0,0,4,0); fill(8,13,1,1,0,0); fill(8,14,2,0,2,1); fill(9,0,1,0,5,0); fill(9,1,0,2,0,0); fill(9,2,1,1,2,1); fill(9,3,2,0,3,2); fill(9,4,1,0,4,0); fill(9,5,1,4,3,0); fill(9,6,0,3,0,1); fill(9,7,1,2,0,2); fill(9,8,1,1,0,3); fill(9,9,0,0,1,4); fill(9,10,1,1,0,0); fill(9,11,0,0,3,1); fill(9,12,1,1,3,0); fill(9,13,0,0,0,1); fill(9,14,3,8,1,0); fill(10,0,2,2,4,0); fill(10,1,0,1,0,1); fill(10,2,2,0,1,2); fill(10,3,3,0,2,0); fill(10,4,2,1,3,0); fill(10,5,2,0,2,1); fill(10,6,0,2,3,0); fill(10,7,0,1,0,1); fill(10,8,0,0,1,2); fill(10,9,1,1,0,0); fill(10,10,0,0,1,1); fill(10,11,1,0,2,0); fill(10,12,2,0,2,0); fill(10,13,0,1,1,0); fill(10,14,4,0,0,1); fill(11,0,3,0,3,0); fill(11,1,0,1,3,0); fill(11,2,3,0,0,1); fill(11,3,4,1,1,0); fill(11,4,3,0,2,1); fill(11,5,3,1,1,0); fill(11,6,1,0,2,1); fill(11,7,0,3,3,0); fill(11,8,1,2,0,1); fill(11,9,0,1,0,2); fill(11,10,1,0,0,3); fill(11,11,2,0,1,0); fill(11,12,3,1,1,0); fill(11,13,1,0,0,1); fill(11,14,0,0,3,0); fill(12,0,4,1,2,0); fill(12,1,1,0,2,1); fill(12,2,0,1,2,0); fill(12,3,5,0,0,1); fill(12,4,4,0,1,0); fill(12,5,4,0,0,0); fill(12,6,2,0,1,0); fill(12,7,1,0,2,0); fill(12,8,0,1,2,0); fill(12,9,0,0,8,1); fill(12,10,0,1,2,0); fill(12,11,3,0,0,1); fill(12,12,4,1,0,0); fill(12,13,0,0,2,1); fill(12,14,1,0,2,0); fill(13,0,5,0,1,0); fill(13,1,2,0,1,0); fill(13,2,1,1,1,0); fill(13,3,0,0,8,1); fill(13,4,5,2,0,0); fill(13,5,0,1,0,1); fill(13,6,3,0,0,2); fill(13,7,2,0,1,0); fill(13,8,1,0,1,0); fill(13,9,1,0,8,0); fill(13,10,1,0,1,0); fill(13,11,0,1,1,0); fill(13,12,0,0,1,1); fill(13,13,1,0,1,0); fill(13,14,2,0,1,0); fill(14,0,6,0,0,0); fill(14,1,3,1,0,0); fill(14,2,2,0,0,1); fill(14,3,1,5,8,0); fill(14,4,0,4,0,1); fill(14,5,0,3,0,2); fill(14,6,0,2,0,3); fill(14,7,3,1,0,4); fill(14,8,2,0,0,5); fill(14,9,2,1,8,0); fill(14,10,2,0,0,1); fill(14,11,1,0,0,0); fill(14,12,1,2,0,0); fill(14,13,2,1,0,1); fill(14,14,3,0,0,2);  

}




void loop(){
  if(count!=1){
    Serial.print("count is ");
    Serial.println(count);   
    if(fblocks>0 && fblocks<8){
      digitalWrite(10,HIGH);
      digitalWrite(11,LOW);
      digitalWrite(12,HIGH);
      digitalWrite(13,LOW);
      delay(1000);
      current_dir = 0;
    }
    if(rblocks>0 && rblocks<8){
      
      digitalWrite(10,LOW); 
      digitalWrite(11,LOW);
      digitalWrite(12,HIGH);
      digitalWrite(13,LOW);
      delay(1000);
      
      digitalWrite(10,HIGH);
      digitalWrite(11,LOW);
      digitalWrite(12,HIGH);
      digitalWrite(13,LOW);
      delay(1000);
      current_dir = 1;
    }
    if(bblocks>0 && bblocks<8){
      
      digitalWrite(10,LOW); 
      digitalWrite(11,LOW);
      digitalWrite(12,HIGH);
      digitalWrite(13,LOW);
      delay(2000);
      
      digitalWrite(10,HIGH);
      digitalWrite(11,LOW);
      digitalWrite(12,HIGH);
      digitalWrite(13,LOW);
      delay(1000);
      current_dir = 2;
    }
    if(lblocks>0 && lblocks<8){
      
      digitalWrite(10,HIGH); 
      digitalWrite(11,LOW);
      digitalWrite(12,LOW);
      digitalWrite(13,LOW);
      delay(1000);
      
      digitalWrite(10,HIGH);
      digitalWrite(11,LOW);
      digitalWrite(12,HIGH);
      digitalWrite(13,LOW);
      delay(1000);
      current_dir = 3;
    }
  
    find_blocks();
       
    for(kar =0 ; kar<count;kar++){
      current_dir = (current_dir+link[kar]%10)% 4;
      current_column = (link[kar]/10)%100 - ((current_dir)%2)*(current_dir-2);
      current_row  = (link[kar]/1000)%100 + ((current_dir+1))%2*(current_dir-1);
      check(current_row,current_column,current_dir);
    }
  
    count = present_count;
    present_count =0;  
  
  }

  else{
    delay(30000);        
  }
  
}
