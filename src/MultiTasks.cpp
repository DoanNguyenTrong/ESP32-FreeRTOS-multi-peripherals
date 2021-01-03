#include "MultiTasks.h"

#include "main.cpp"


/* this function will be invoked when additionalTask was created */
void servoThread( void * parameter ){
  for(;;){
    Serial.println("Control LED in anotherTask!");
    digitalWrite(servoPin, HIGH);
    delay(1000);
    digitalWrite(servoPin, LOW);
    delay(1000);

  }

  vTaskDelete( NULL );
}


