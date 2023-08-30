#include "QTRSensors.h"
#include <stdio.h>

// define constant for QTR sensor
#define NUM_SENSORS   8     // number of sensors used
#define TIMEOUT       2500  // waits for 2500 us for sensor outputs to go low
#define EMITTER_PIN   12

// Motor Connections (All must use PWM pins)

// IN1 = Backward , IN2 = Forward / A = Left Motor , B = Right Motor

#define IN1A 2
#define IN1B 3

#define IN2A 4
#define IN2B 5

//declaration variable
int state = 0;
int count = 0 ;
int count2 = 0;
int count3 = 0;

// sensor connected through analog pins A0 - A5 i.e. digital pins 14 - 19
unsigned char sensorPins[] = {A1, A2, A3, A4, A5, A6, A7, A8};
QTRSensorsRC qtrrc(sensorPins, NUM_SENSORS, TIMEOUT, EMITTER_PIN);

// variable to store all sensor status
unsigned int sensorValues[NUM_SENSORS];
#define CALIBRATE_SPEED (30.0 / 100.0 * 255.0)

// function declaration
void goBackwardRight(unsigned int delaySpeed, unsigned int turnSpeed, unsigned int duration);
void goBackwardLeft(unsigned int delaySpeed, unsigned int turnSpeed, unsigned int duration);
void goBackward(unsigned int motorSpeed, unsigned int duration);
void goStraight(unsigned int motorSpeed, unsigned int duration);
void turnRight(unsigned int delaySpeed, unsigned int turnSpeed, unsigned int duration);
void turnLeft(unsigned int delaySpeed, unsigned int turnSpeed, unsigned int duration);
void stopRobot(unsigned int duration);

void setup()
{
  // initialize communication buad rate
  Serial.begin(9600);
  
  for (int i = 0; i < 50; i++)
  {
    // calibrate for sometime by sliding the sensors across the line,
    // or you may use auto-calibration instead
    // comment this part out for automatic calibration

    // turn left or turn right to expose the sensors to the brightest and darkest
    // readings that may be encountered
    if ( i  < 12 || i >= 38 )
      turnLeft(CALIBRATE_SPEED, CALIBRATE_SPEED, 0);
    else
      turnRight(CALIBRATE_SPEED, CALIBRATE_SPEED, 0);

    qtrrc.calibrate();
  }

  stopRobot(0);

  Serial.print("Min: ");
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(qtrrc.calibratedMinimumOn[i]);
    Serial.print("\t");
  }
  Serial.println();

  //delay(5);
  Serial.print("Max: ");
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(qtrrc.calibratedMaximumOn[i]);
    Serial.print("\t");
  }
  Serial.println();
}

void loop()
{
  do
  {
    int position = qtrrc.readLine(sensorValues);
    //state = Serial.parseInt();
    Serial.println("State: " + String(state));
    switch (state)
    { //go to A,B,.. station state if case >=1 integer but case 0 is initial state (wait for order)
      case 0 :  // wait for order
        //stopRobot(0);
        if (position == -1) //Stop
              stopRobot(0);
            else if (position >= 5500 && position <= 7000) //turnleft
              turnLeft(20, 40, 0);
            else if (position > 2500 && position < 5500 ) //gostraight
              goStraight(40, 0);
            else if (position >= 0 && position <= 2500 )  //turnright
              turnRight(20, 40, 0);
        break ;

      case 1 : //go to station A after then, stop at station A and Return to initial station

        switch (count)
        {
          case 0: // go to station A
            if (count3 == 0) //sensor detect line
            {
            Serial.println("Go to A station");
            if (position == -1) //Stop
              stopRobot(0);
            else if (position >= 5500 && position <= 7000) //turnleft
              turnLeft(20, 40, 0);
            else if (position > 2500 && position < 5500 ) //gostraight
              goStraight(40, 0);
            else if (position >= 0 && position <= 2500 )  //turnright
              turnRight(20, 40, 0);
            
              if (sensorValues[7] == 1000 && sensorValues[6 ] == 1000 && sensorValues[5] == 1000 && sensorValues[4] == 1000 && sensorValues[3] == 1000 && sensorValues[2] == 1000 && sensorValues[1] == 1000 && sensorValues[0] == 1000 ) //detect all sensor 8 == 1000 count3 ++
              {
                count3 ++;
                goStraight(40, 3000);
              }
            }
            if (count3 > 1)
              count = 1;
            break ;
          case 1://stop at station A and Return to initial station
            switch (count2) //stop at station A after then,uturn and return to initial station
            {
              case 0://stop at station and uturn
                Serial.println("stop at station A");
                stopRobot(10000);
                Serial.println("Uturn");
                uturnRobot(3000);
                count2 = 1;
                count3 = 0;
                break;
              case 1://return to initial station
                if (count3 == 0) //if not detect initial station , line follower
                {
                  Serial.println("go to initial station");
                   if (position == -1) //Stop
                      stopRobot(0);
                   else if (position >= 5500 && position <= 7000) //turnleft
                      turnLeft(20, 40, 0);
                   else if (position > 2500 && position < 5500 ) //goStraight
                      goStraight(40, 0);
                   else if (position >= 0 && position <= 2500 ) //turnright
                      turnRight(20, 40, 0);
                }
                if (sensorValues[7] == 1000 && sensorValues[6 ] == 1000 && sensorValues[5] == 1000 && sensorValues[4] == 1000 && sensorValues[3] == 1000 && sensorValues[2] == 1000 && sensorValues[1] == 1000 && sensorValues[0] == 1000 && position == 3500) //detect all sensor 8 == 1000 count3 ++
                {
                  count3 ++;
                  delay(3000);
                }
                else//if detect initial station stop and uturn then, back to initial state wait for order
                {
                  Serial.println("stop at initial station");
                  stopRobot(2000);
                  Serial.println("Uturn");
                  uturnRobot(3000);
                  count2 = 0;
                  count = 0;
                  state = 0;
                  count3 = 0;
                }
                break;
            }

            break ;
        }
        break;
      case 2 : //go to station B after then, stop at station B and Return to initial station

        switch (count)
        {
          case 0: // go to station B
            if (count3 == 0) //sensor detect line
            {
              Serial.println("Go to B station");
               if (position == -1) //Stop
                  stopRobot(0);
               else if (position >= 5500 && position <= 7000) //turnleft
                  turnLeft(20, 40, 0);
               else if (position > 2500 && position < 5500 ) //goStraight
                  goStraight(40, 0);
               else if (position >= 0 && position <= 2500 ) //turnright
                  turnRight(20, 40, 0);
              Serial.print("count3 = ");
              Serial.println(count3);
              if (sensorValues[7] == 1000 && sensorValues[6 ] == 1000 && sensorValues[5] == 1000 && sensorValues[4] == 1000 && sensorValues[3] == 1000 && sensorValues[2] == 1000 && sensorValues[1] == 1000 && sensorValues[0] == 1000 && position == 3500) //detect all sensor 8 == 1000 count3 ++
              {
                count3 ++;
                goStraight(40, 2000);
              }
            }
            else if (count3 == 2)
              count = 1;
            break ;
          case 1://stop at station B and Return to initial station
            switch (count2) //stop at station B after then,uturn and return to initial station
            {
              case 0://stop at station and uturn
                Serial.println("stop at station B");
                stopRobot(10000);
                Serial.println("Uturn");
                uturnRobot(6000);
                count2 = 1;
                count3 = 0;
                break;
              case 1://return to initial station
                if (count3 == 0) //if not detect initial station , line follower
                {
                  Serial.println("go to initial station");
                  if (position == -1) //Stop
                      stopRobot(0);
                  else if (position >= 5500 && position <= 7000) //turnleft
                      turnLeft(20, 40, 0);
                  else if (position > 2500 && position < 5500 ) //goStraight
                      goStraight(40, 0);
                  else if (position >= 0 && position <= 2500 ) //turnright
                      turnRight(20, 40, 0);
                  Serial.print("count3 = ");
                  Serial.println(count3);
                  if (sensorValues[7] == 1000 && sensorValues[6 ] == 1000 && sensorValues[5] == 1000 && sensorValues[4] == 1000 && sensorValues[3] == 1000 && sensorValues[2] == 1000 && sensorValues[1] == 1000 && sensorValues[0] == 1000 && position == 3500) //detect all sensor 8 == 1000 count3 ++
                  {
                    count3 ++;
                      goStraight(40, 2000);
                  }
                }
                else//if detect initial station stop and uturn then, back to initial state wait for order
                {
                  Serial.println("stop at initial station");
                  stopRobot(2000);
                  Serial.println("Uturn");
                  uturnRobot(6000);
                  count2 = 0;
                  count = 0;
                  state = 0;
                  count3 = 0;
                }
                break;
            }

            break ;
        }
        break;
    }



    Serial.print(position);
    Serial.print("\t");
    Serial.print(sensorValues[7]);
    Serial.print("\t");
    Serial.print(sensorValues[6]);
    Serial.print("\t");
    Serial.print(sensorValues[5]);
    Serial.print("\t");
    Serial.print(sensorValues[4]);
    Serial.print("\t");
    Serial.print(sensorValues[3]);
    Serial.print("\t");
    Serial.print(sensorValues[2]);
    Serial.print("\t");
    Serial.print(sensorValues[1]);
    Serial.print("\t");
    Serial.print(sensorValues[0]);
    Serial.println();

    delay(500);
  } while (1);
}

void goBackwardRight(unsigned int delaySpeed, unsigned int turnSpeed, unsigned int duration)
{
  // IN1 = Backward , IN2 = Forward / A = Left Motor , B = Right Motor

  //Forward
  analogWrite(IN2A, 0);
  analogWrite(IN2B, 0);

  //Backward
  digitalWrite(IN1A, turnSpeed);
  digitalWrite(IN1B, delaySpeed);

  delay(duration);
}

void goBackwardLeft(unsigned int delaySpeed, unsigned int turnSpeed, unsigned int duration)
{
  // IN1 = Backward , IN2 = Forward / A = Left Motor , B = Right Motor

  //Forward
  analogWrite(IN2A, 0);
  analogWrite(IN2B, 0);

  //Backward
  digitalWrite(IN1A, delaySpeed);
  digitalWrite(IN1B, turnSpeed);

  delay(duration);
}

void goBackward(unsigned int motorSpeed, unsigned int duration)
{
  // IN1 = Backward , IN2 = Forward / A = Left Motor , B = Right Motor

  //Forward
  analogWrite(IN2A, 0);
  analogWrite(IN2B, 0);

  //Backward
  digitalWrite(IN1A, motorSpeed);
  digitalWrite(IN1B, motorSpeed);

  delay(duration);
}

void goStraight(unsigned int motorSpeed, unsigned int duration)
{
  // IN1 = Backward , IN2 = Forward / A = Left Motor , B = Right Motor

  //Forward
  analogWrite(IN2A, motorSpeed);
  analogWrite(IN2B, motorSpeed);

  //Backward
  digitalWrite(IN1A, 0);
  digitalWrite(IN1B, 0);

  delay(duration);
}

void turnLeft(unsigned int delaySpeed, unsigned int turnSpeed, unsigned int duration)
{
  // IN1 = Backward , IN2 = Forward / A = Left Motor , B = Right Motor

  //Forward
  analogWrite(IN2A, turnSpeed); //Left Motor
  analogWrite(IN2B, delaySpeed); //Right Motor

  //Backward
  digitalWrite(IN1A, 0); //Left Motor
  digitalWrite(IN1B, 0); //Right Motor

  //duration of turning
  delay(duration);
}

void turnRight(unsigned int delaySpeed, unsigned int turnSpeed, unsigned int duration)
{
  // IN1 = Backward , IN2 = Forward / A = Left Motor , B = Right Motor

  //Forward
  analogWrite(IN2A, delaySpeed); //Left Motor
  analogWrite(IN2B, turnSpeed); //Right Motor

  //Backward
  digitalWrite(IN1A, 0); //Left Motor
  digitalWrite(IN1B, 0); //Right Motor

  //duration of turning
  delay(duration);
}

void stopRobot(unsigned int duration)
{
  // IN1 = Backward , IN2 = Forward / A = Left Motor , B = Right Motor

  //Forward
  analogWrite(IN2A, 0); //Left Motor
  analogWrite(IN2B, 0); //Right Motor

  //Backward
  digitalWrite(IN1A, 0); //Left Motor
  digitalWrite(IN1B, 0); //Right Motor

  //duration of turning
  delay(duration);
}
void uturnRobot(unsigned int duration)
{
  // IN1 = Backward , IN2 = Forward / A = Left Motor , B = Right Motor

  digitalWrite(IN1A, LOW);
  digitalWrite(IN2B, LOW);
  analogWrite(IN1B, 80);
  analogWrite(IN2A, 80);

  //duration of turning
  delay(duration);
}
