void setup() {
  Serial.begin(9600); // sets up the serial port, 9600 is the baud rate. The speed at which a signal can travel
  Serial.println("Hello from the Serial monitor!");
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);

}

void loop() {
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);
  digitalWrite(10, HIGH);
  digitalWrite(11, HIGH);
  digitalWrite(12, HIGH);
  digitalWrite(13, HIGH);

  delay(1000);

  digitalWrite(8,  LOW);
  digitalWrite(9,  LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);

  delay(1000);
}
