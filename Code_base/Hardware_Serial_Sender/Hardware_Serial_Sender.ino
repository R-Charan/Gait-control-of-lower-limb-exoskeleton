int state = 0;
#define RXD2 16
#define TXD2 17

void setup () {
  pinMode(0, INPUT);
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, 16, 17);
//             (baudrate, protocol, RX_pin, TX_pin)
}

void loop () {
  Serial.println("USB-Serial");

  if (digitalRead(0) == LOW) {
    if (state <= 1) {
      state = state + 1;
    }
    else {
      state = 0;
    }
  }
  
  switch (state) {
    case 0:
      Serial.println(0);
      Serial2.write("123456\n");
      break;
    case 1:
      Serial.println(1);
      Serial2.write("100101\n");
      break;
    case 2:
      Serial.println(2);
      Serial2.write("654321\n");
      break;
  }
  
  delay(1000);
}
