
#define SERIAL_BAUD_RATE 9600

int pin_1 = A0;
int pin_2 = A1;
int pin_3 = A2;
int pin_4 = A3;

void setup() {
  pinMode(13, OUTPUT);
  pinMode(pin_1, INPUT);
  pinMode(pin_2, INPUT);
  pinMode(pin_3, INPUT);
  pinMode(pin_4, INPUT);
  
  Serial.begin(SERIAL_BAUD_RATE);
  while (!Serial);
}

void loop() {
  digitalWrite(13, HIGH);
  
  String msg;
  char c;
  while (true) {
    while (!Serial.available());
    
    c = Serial.read();
    
    if (c == '\n')
      break;
      
    msg += c;
  }
  
  if (msg.startsWith("get_one")) {
    unsigned long time = millis();
    int channel1 = analogRead(pin_1) >> 2;
    int channel2 = analogRead(pin_2) >> 2;
    int channel3 = analogRead(pin_3) >> 2;
    int channel4 = analogRead(pin_4) >> 2;
    
    Serial.print(time);
    Serial.print(" ");
    Serial.print(channel1);
    Serial.print(" ");
    Serial.print(channel2);
    Serial.print(" ");
    Serial.print(channel3);
    Serial.print(" ");
    Serial.println(channel4);
  } else if (msg.startsWith("frame")) {
    String arg = msg.substring(6); // 6="frame "
    int duration = arg.toInt();
    
    if (duration == 0) {
      Serial.println("error: arg is invalid or zero");
      return;
    }
    
    int channel1, channel2, channel3, channel4;
    unsigned long time_end = millis() + duration;
    

    byte values[] = {1, 0, 0, 0, 0};
    while (millis() <= time_end) {
      values[0] = 1;
      values[1] = analogRead(pin_1) >> 2;
      values[2] = analogRead(pin_2) >> 2;
      values[3] = analogRead(pin_3) >> 2;
      values[4] = analogRead(pin_4) >> 2;
      
      Serial.write(values, 5);
    }
    byte endValues[] = {0, 0, 0, 0, 0};
    Serial.write(endValues, 5);
  } else {
    Serial.println("error: unknown command: " + msg);
  }
  
  digitalWrite(13, LOW);
}
