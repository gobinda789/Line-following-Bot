int P, D, I, previousError, PIDvalue, error;
int lsp, rsp;
int lfspeed = 160;

float Kp = 0.21;//0.21
float Kd = 0.5;//1
float Ki = 0 ;


int minValues[5], maxValues[5], threshold[5];

void setup()
{
  Serial.begin(9600);
  pinMode(11, INPUT_PULLUP);
  pinMode(12,OUTPUT);
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
}


void loop()
{
  while (digitalRead(11)==0) {}
  delay(1000);
  calibrate();
  while (digitalRead(11)==0) {}
  delay(1000);

  while (1)
  {
    if (analogRead(0) > threshold[0] && analogRead(4) < threshold[4] )
    {
      lsp = 0; rsp = lfspeed;
  analogWrite(3,rsp);
  analogWrite(5,rsp+6);
  digitalWrite(2,LOW);
  digitalWrite(4,HIGH);
  digitalWrite(6,HIGH);
  digitalWrite(7,LOW);
    }

    else if (analogRead(4) > threshold[4] && analogRead(0) < threshold[0])
    { lsp = lfspeed; rsp = 0;
   analogWrite(3,lsp);
  analogWrite(5,lsp+6);
  digitalWrite(2,HIGH);
  digitalWrite(4,LOW);
  digitalWrite(6,LOW);
  digitalWrite(7,HIGH);
    }
    else if (analogRead(2) > threshold[2])
    {
      Kp = 0.005 * (1000 - analogRead(2));//0.003
      Kd = 0 *Kp;
      //Ki = 0.0001;
      linefollow();
    }
  }
}

void linefollow()
{
  int error = (analogRead(1) - analogRead(3));

  P = error;
  I = I + error;
  D = error - previousError;

  PIDvalue = (Kp * P) + (Ki * I) + (Kd * D);
  previousError = error;

  lsp = lfspeed - PIDvalue;
  rsp = lfspeed + PIDvalue;

  if (lsp > 255) {
    lsp = 255;
  }
  if (lsp < 0) {
    lsp = 0;
  }
  if (rsp > 255) {
    rsp = 255;
  }
  if (rsp < 0) {
    rsp = 0;
  }
  analogWrite(3,rsp);
  analogWrite(5,lsp);
  digitalWrite(2,HIGH);
  digitalWrite(4,LOW);
  digitalWrite(6,HIGH);
  digitalWrite(7,LOW);

}

void calibrate()
{
  for ( int i = 0; i < 5; i++)
  {
    minValues[i] = analogRead(i);
    maxValues[i] = analogRead(i);
  }
  
  for (int i = 0; i < 3000; i++)
  {
    analogWrite(3,50);
    analogWrite(5,50);
    digitalWrite(2,HIGH);
    digitalWrite(4,LOW);
    digitalWrite(6,LOW);
    digitalWrite(7,HIGH);
    for ( int i = 0; i < 5; i++)
    {
      if (analogRead(i) < minValues[i])
      {
        minValues[i] = analogRead(i);
      }
      if (analogRead(i) > maxValues[i])
      {
        maxValues[i] = analogRead(i);
      }
    }
  }

  for ( int i = 0; i < 5; i++)
  {
    threshold[i] = (minValues[i] + maxValues[i]) / 2;
    Serial.print(threshold[i]);
    Serial.print("   ");
  }
  Serial.println();
  
   analogWrite(3,0);
  analogWrite(5,0);
  digitalWrite(2,HIGH);
  digitalWrite(4,LOW);
  digitalWrite(6,LOW);
  digitalWrite(7,HIGH);
}
