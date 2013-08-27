int i1=3, i2=5, j1=6, j2=9, EA=12, EB=13;
char d[6], c;
int p; //assembled engine power
int i; 

/*
    IPW2013@Tirgu Mures, RO
(c) Alexandru Ioan Pop, 2013
    WIP
    
This receives commands over Bluetooth (ideally from the gyroscope
of a phone).
pins 3, 5, 6, 9 are Digital Outs with PWM
EA, EB enable the output of the engine

Used a L298 H bridge

The Arduino receives, over BT serial, a packet of 6 bytes
e.g.
"1+100 "

1st byte: 0 for X (front-back), 1 for Y (left-right)
2nd byte: + for front, right; - for back, left
next 3 bytes: a value from 0 to 127, how much current
              should go to the engines (this can be tweaked in the
              sender code, but that is the convention that we
              worked out)
last byte: one space - separator
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
  for(i=0; i<5; i++) d[i]='2'; //clean the input
  d[5]='\0';
  i=-1;
  p=0; 
}

void loop()
{
  if(Serial.available()>0 && i<5) //read a packet
  {
    i++;
    d[i]=Serial.read();
    /*c=Serial.read(); //PC keys controls
     if(c=='w') strcpy(d, "0+255 ");
     if(c=='s') strcpy(d, "0-255 ");
     if(c=='a') strcpy(d, "1-255 ");
     if(c=='d') strcpy(d, "1+255 ");*/
  }
  if(i==5)
  {
    d[5]='\0'; //cap it off
    i=-1; //reset the byte counter
  }
  digitalWrite(EA, HIGH); //enable the engines
  digitalWrite(EB, HIGH);
  p=(d[2]-'0')*100+(d[3]-'0')*10+d[4]-'0'; //assemble the power
  if(p>127) p=127; //PWN only goes up the 255, so we limit this
  if(d[0]=='0') //front-back
  {
    if(d[1]=='-') //back
    {
      analogWrite(i1, p*2);
      analogWrite(i2, 0);
      analogWrite(j1, p*2);
      analogWrite(j2, 0);
    }
    else //front
    {
      analogWrite(i1, 0);
      analogWrite(i2, p*2);
      analogWrite(j1, 0);
      analogWrite(j2, p*2);
    }
  }
  if(d[0]=='1') //left-right
  {
    if(d[1]=='+') //right
    {
      analogWrite(i1, p*2);
      analogWrite(i2, 0);
      analogWrite(j1, 0);
      analogWrite(j2, p*2);
    }
    else //left
    {
      analogWrite(i1, 0);
      analogWrite(i2, p*2);
      analogWrite(j1, p*2);
      analogWrite(j2, 0);
    }
  }
  delay(5); //stabilize everything
}


