int i1=3, i2=5, j1=6, j2=9, EA=12, EB=13;
char c, d[5];
int p; //assembled engine power
int i, packetSize=5;
boolean PC=true;
/*
    IPW2013@Tirgu Mures, RO
(c) Alexandru Ioan Pop, 2013
    WIP
    
This receives commands over serial (I used Bluetooth) (ideally from the gyroscope
of a phone).
pins 3, 5, 6, 9 are Digital Outs with PWM
EA, EB enable the output of the engine

Used a L298 H bridge

The Arduino receives, over BT serial, a packet of 5 bytes
e.g.
"1+100"

1st byte: 0 for X (front-back), 1 for Y (left-right)
2nd byte: + for back, right; - for front, left
next 3 bytes: a value from 0 to 255, how much current
              should go to the engines (this can be tweaked in the
              sender code, but that is the convention that we
              worked out)
*/

void setup()
{
  pinMode(i1, OUTPUT);
  pinMode(i2, OUTPUT);
  pinMode(j1, OUTPUT);
  pinMode(j2, OUTPUT);
  pinMode(EA, OUTPUT);
  pinMode(EB, OUTPUT);
  Serial.begin(9600);
  for(i=0; i<packetSize; i++) d[i]='2'; //clean the input
  d[packetSize-1]='\0';
  i=0;
  p=0;
}

void loop()
{ 
  
  if(Serial.available()>0 && i<packetSize) //read a packet
  {
   // d[i]=Serial.read();
    c=Serial.read(); //PC keys controls
     if(c=='w') strcpy(d, "0-255");
     if(c=='s') strcpy(d, "0+255");
     if(c=='a') strcpy(d, "1+255");
     if(c=='d') strcpy(d, "1-255");
     if(c=='q') strcpy(d, "0+000");
     //i++;
  }
  if(valid()==0) //check to see if input is good
  {
    writeToEngine();
  }
  if(i==5) i=0;
  digitalWrite(EA, HIGH); //enable the engines
  digitalWrite(EB, HIGH);
  
  delay(5); //stabilize everything
}

int valid()
{
  if(d[0]<'0' || d[0]>'1') return 1;
  if(!(d[1]=='+' || d[1]=='-')) return 2;
  if(i!=packetSize-1 && !PC) return 3;
  return 0;
}

void writeToEngine()
{
  p=(d[2]-'0')*100+(d[3]-'0')*10+d[4]-'0'; //assemble the power
  if(d[0]=='0') //front-back
  {
    if(d[1]=='-') //back
    {
      analogWrite(i1, p);
      analogWrite(i2, 0);
      analogWrite(j1, p);
      analogWrite(j2, 0);
    }
    else //front
    {
      analogWrite(i1, 0);
      analogWrite(i2, p);
      analogWrite(j1, 0);
      analogWrite(j2, p);
    }
  }
  if(d[0]=='1') //left-right
  {
    if(d[1]=='+') //right
    {
      analogWrite(i1, p);
      analogWrite(i2, 0);
      analogWrite(j1, 0);
      analogWrite(j2, p);
    }
    else //left
    {
      analogWrite(i1, 0);
      analogWrite(i2, p);
      analogWrite(j1, p);
      analogWrite(j2, 0);
    }
  }
}

