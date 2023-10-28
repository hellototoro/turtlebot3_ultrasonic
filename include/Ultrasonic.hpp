#ifndef ULTRASONIC_HPP
#define ULTRASONIC_HPP

#include <wiringPi.h>
#include <iostream>

class Ultrasonic {
public:
    Ultrasonic() = delete;
    Ultrasonic(int trigPin, int echoPin, long timeOut = 20000UL)
        : trigPin_(trigPin), echoPin_(echoPin), timeOut_(timeOut) {
        if (wiringPiSetup() == -1) {
            std::cout << "Failed to setup wiringPi." << std::endl;
            exit(1);
        }

        pinMode(trigPin_, OUTPUT);
        pinMode(echoPin_, INPUT);
    }

    int getDistance() {
        digitalWrite(trigPin_, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin_, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin_, LOW);

        long startTime = micros();
        while ((digitalRead(echoPin_) == LOW) && ((micros() - startTime) < timeOut_));

        startTime = micros();
        while ((digitalRead(echoPin_) == HIGH) && ((micros() - startTime) < timeOut_));
        long travelTime = micros() - startTime;

        int distance = travelTime / 58;

        return distance;
    }

private:
    int trigPin_;
    int echoPin_;
    long timeOut_;
};

#endif // ULTRASONIC_HPP
