#include <TerabeeEvoHub.h>
#include <crc8.h>

/**
 * @brief Setup communication between microcontroller and Terabee EvoHub
 * @param serialPort Select which HardwareSerial Port you want to use. e.g. Serial1, Serial2 , ...
 * @param refreshRate select one from the list 50-600Hz
 * @param mode select how the sensors are read by the hub
 * @param style Text or Binary mode
 */
TerabeeEvoHub::TerabeeEvoHub(HardwareSerial &serialPort, byte refreshRate, byte mode, byte style) {
  _serialPort = &serialPort;
  _style = style;
  _refreshRate = refreshRate;
  _mode = mode;

  _serialPort->begin(HUB_BAUD);
  delay(10);
  _serialPort->write(styleList[_style], 4);
  delay(10);
  _serialPort->write(refreshList[_refreshRate], 5);
  delay(10);
  _serialPort->write(modeList[_mode], 4);
  delay(10);
}

void TerabeeEvoHub::start(){
  _serialPort->write(runList[START], 5);
}

void TerabeeEvoHub::stop(){
  _serialPort->write(runList[STOP], 5);
}

void TerabeeEvoHub::translate(){
  if(Framereceived[0] == 'T'){
    //Convert bytes to distances
    dist  = (Framereceived[2]<<8) + Framereceived[3];
    dist1 = (Framereceived[4]<<8) + Framereceived[5];
    dist2 = (Framereceived[6]<<8) + Framereceived[7];
    dist3 = (Framereceived[8]<<8) + Framereceived[9];
    dist4 = (Framereceived[10]<<8) + Framereceived[11];
    dist5 = (Framereceived[12]<<8) + Framereceived[13];
    dist6 = (Framereceived[14]<<8) + Framereceived[15];
    dist7 = (Framereceived[16]<<8) + Framereceived[17];
  }
}

void TerabeeEvoHub::read(int &slot1){
  translate();
  slot1 = dist;
  reset();
}
void TerabeeEvoHub::read(int &slot1, int &slot2){
  translate();
  slot1 = dist;
  slot2 = dist1;
  reset();
}
void TerabeeEvoHub::read(int &slot1, int &slot2, int &slot3){
  translate();
  slot1 = dist;
  slot2 = dist1;
  slot3 = dist2;
  reset();
}
void TerabeeEvoHub::read(int &slot1, int &slot2, int &slot3, int &slot4){
  translate();
  slot1 = dist;
  slot2 = dist1;
  slot3 = dist2;
  slot4 = dist3;
  reset();
}
void TerabeeEvoHub::read(int &slot1, int &slot2, int &slot3, int &slot4, int &slot5){
  translate();
  slot1 = dist;
  slot2 = dist1;
  slot3 = dist2;
  slot4 = dist3;
  slot5 = dist4;
  reset();
}
void TerabeeEvoHub::read(int &slot1, int &slot2, int &slot3, int &slot4, int &slot5, int &slot6){
  translate();
  slot1 = dist;
  slot2 = dist1;
  slot3 = dist2;
  slot4 = dist3;
  slot5 = dist4;
  slot6 = dist5;
  reset();
}
void TerabeeEvoHub::read(int &slot1, int &slot2, int &slot3, int &slot4, int &slot5, int &slot6, int &slot7){
  translate();
  slot1 = dist;
  slot2 = dist1;
  slot3 = dist2;
  slot4 = dist3;
  slot5 = dist4;
  slot6 = dist5;
  slot7 = dist6;
  reset();
}
void TerabeeEvoHub::read(int &slot1, int &slot2, int &slot3, int &slot4, int &slot5, int &slot6, int &slot7, int &slot8){
  translate();
  slot1 = dist;
  slot2 = dist1;
  slot3 = dist2;
  slot4 = dist3;
  slot5 = dist4;
  slot6 = dist5;
  slot7 = dist6;
  slot8 = dist7;
  reset();
}

void TerabeeEvoHub::update(){
  
  if (_serialPort->available() > 0) {
    uint8_t inChar = _serialPort->read();
    if (index == 0) {
      if (inChar == 'T')
      {
        //Looking for frame start 'H'
        Framereceived[index++] = inChar;
      }
      else return;
    }
    else if ((index >= 1) && (index < 19))
    {
      //Gathering data
      Framereceived[index++] = inChar;
    }
    else if(index == 20) {
      reset();
    }
  }
}

void TerabeeEvoHub::reset(){
  index = 0;
  Framereceived[0] = 0;
}


uint8_t TerabeeEvoHub::crc8(uint8_t *p, uint8_t len) {
  uint8_t i;
  uint8_t crc = 0x0;
  while (len--) {
    i = (crc ^ *p++) & 0xFF;
    crc = (crc_table[i] ^ (crc << 8)) & 0xFF;
  }
  return crc & 0xFF;
}