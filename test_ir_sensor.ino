int irsensor1 = 2;
int irsensor2 = 3;

int valuesensor1 = 0;
int valuesensor2 = 0;

void setup() {
  Serial.begin(9600);
  pinMode(irsensor1, INPUT);
  pinMode(irsensor2, INPUT);

}

void loop() {
  valuesensor1 = digitalRead(irsensor1);
  valuesensor2 = digitalRead(irsensor2);

  Serial.print("sensor 1 = ");
  Serial.println(valuesensor1);
  Serial.print("sensor 2 = ");
  Serial.println(valuesensor2);
  delay(1000);

}
