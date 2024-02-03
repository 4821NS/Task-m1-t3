// Abstract class for sensors
class Sensor {
public:
  virtual void setupSensor() = 0;
  virtual void handleInterrupt() = 0;
};

// Derived class for PIR sensor
class PIRSensor : public Sensor {
private:
  const int interruptPin;
  const int ledPin;
  static PIRSensor* pirSensorInstance; // Declare static instance

public:
  PIRSensor(int interruptPin, int ledPin) : interruptPin(interruptPin), ledPin(ledPin) {
    pirSensorInstance = this; // Set the instance
  }

  void setupSensor() override {
    pinMode(ledPin, OUTPUT);
    attachInterrupt(digitalPinToInterrupt(interruptPin), pirInterruptHandler, HIGH);
  }

  static void pirInterruptHandler() {
    pirSensorInstance->handleInterrupt();
  }

  void handleInterrupt() override {
    int motion = digitalRead(interruptPin);

    Serial.print("PIR Sensor: ");

    if (motion == HIGH) {
      digitalWrite(ledPin, HIGH);
      Serial.println("Detected motion!");
      
    } else {
      Serial.println("No motion");
    }
  }
};

PIRSensor* PIRSensor::pirSensorInstance = nullptr;

class ButtonSensor : public Sensor {
private:
  const int buttonPin;
  const int ledPin;
  static ButtonSensor* buttonSensorInstance; // Declare static instance

public:
  ButtonSensor(int buttonPin, int ledPin) : buttonPin(buttonPin), ledPin(ledPin) {
    buttonSensorInstance = this; // Set the instance
  }

  void setupSensor() override {
    pinMode(ledPin, OUTPUT);
    pinMode(buttonPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(buttonPin), buttonInterruptHandler, FALLING);
  }

  static void buttonInterruptHandler() {
    buttonSensorInstance->handleInterrupt();
  }

  void handleInterrupt() override {
    static bool ledState = false;
    static bool highIntensity = false;

    ledState = !ledState;
    highIntensity = (analogRead(A3) > 512); // Adjust the threshold as needed

    if (!highIntensity && ledState) {
      digitalWrite(ledPin, HIGH);
      Serial.println("Low Light intensity, Button LED: ON");
    } else {
      Serial.println("High Light intensity, Button LED: OFF");
      digitalWrite(ledPin, LOW);
      ledState = false;
    }
  }
};

// Initialize the static member
ButtonSensor* ButtonSensor::buttonSensorInstance = nullptr;

// Create instances of the sensors
PIRSensor pirSensor(2, 7);
ButtonSensor buttonSensor(3, 13);

void setup() {
  Serial.begin(9600);
  Serial.println("Multiple interrupt function: welcome");

  pirSensor.setupSensor();
  buttonSensor.setupSensor();
}

void loop() {
  // Your main code can go here if needed
}
