#include <Arduino.h>
#include <TerabeeEvoHub.h>
#include <Ticker.h>

TerabeeEvoHub hub(Serial8, REFRESH_ASAP, SIMULTANIOUS);

void update(){
    int temp, temp1, temp2, temp3;
    hub.read(temp, temp1, temp2, temp3);
    if((temp > 1) && (temp1 > 1) && (temp2 > 1) && (temp3 > 1)){
        Serial.printf("Distenace: \nSlot1: %imm \nSlot2: %imm\nSlot3: %imm\nSlot4: %imm \n\n\r", temp, temp1, temp2, temp3);   
    }
}

Ticker timer(update, 100);
void setup(){
    Serial.begin(921600);
    delay(1000);
    Serial.println("Start...");
    delay(1000);
    timer.start();
    hub.start();
    
}

void loop(){
    timer.update();
    hub.update();
}