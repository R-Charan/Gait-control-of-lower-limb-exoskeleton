String inString = "";
#define RXD2 16
#define TXD2 17

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, 16, 17);
  while (!Serial) {
  }
}

void loop() {
  while (Serial2.available() > 0) {
    Serial.println("entered");
    int inChar = Serial2.read();
    Serial.println(inChar);

    if (isDigit(inChar)) {
      inString += (char)inChar;
    }

    if (inChar == '\n') {
      Serial.println(inString.toInt());
      int tempt = inString.toInt() / 1000;
      double thigh = (double)tempt / 10;
      int tempk = inString.toInt() - (1000 * tempt);
      double knee = (double)tempk / 10;

      Serial.print("Thigh: ");
      Serial.println(thigh);
      Serial.print("Knee: ");
      Serial.println(knee);
      inString = "";
    }
  }
}
