// setting PWM properties
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 10;

int number = 102;
String inString = "";

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(100);
  }

  pinMode(23, OUTPUT);
  pinMode(19, OUTPUT);
  digitalWrite(23, HIGH);
//  digitalWrite(19, HIGH);
  
  // configure LED PWM functionalitites
  ledcSetup(ledChannel, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(25, ledChannel);
  ledcAttachPin(2, ledChannel);
}

void loop() {
  while (Serial.available() > 0) {
    int inChar = Serial.read();  
    
    if (isDigit(inChar)) {
      inString += (char)inChar;
    }
    
    if (inChar == '\n') {
      int temp = inString.toInt();
      
      if ((temp >= 102) && (temp <= 922)) {
        number = inString.toInt();
      }
      else if (temp > 922) {
        number = 922;
      }
      else if (temp < 102) {
        number = 102;
      }
      inString = "";
    }
  }
  Serial.println(number);
  ledcWrite(ledChannel, number);
  delay(100);
}
