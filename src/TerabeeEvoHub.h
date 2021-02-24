#ifndef TerabeeEvoHub_H
#define TerabeeEvoHub_H

#include <Arduino.h>

/**
 *  @param START Tells the sensor to start sending values via UART
 *  @param STOP Sensor stops streaming values via UART
*/
enum stream {
    START,
    STOP
};

const byte runList[2][5]        =  {{0x00, 0x52, 0x02, 0x01, 0xDF},
                                    {0x00, 0x52, 0x02, 0x00, 0xD8}};

/** 
 * @param TEXT
 *
 *  TH /t XXXX /t XXXX /t XXXX /t XXXX /t XXXX /t XXXX /t XXXX /t XXXX /r /n (Without Spaces)
 *
 *  - Header (two characters): T (84 decimal / 0x54 hex) and H (72 decimal /0x48 hex)
 *  - Tabulation: /t (9 decimal / 0x09 hex)
 *  - Distance reading in millimeters (maximum 5 bytes per sensor): XXXX
 *  - Carriage return character: /r (13 decimal / 0x0D hex)
 *  - New line character: /n (10 decimal / 0x0A hex)
 *
 *  @param BINARY
 *
 *  TH XXXXXXXXXXXXXXXX M CRC8
 *
 *  - Header (two characters): T (84 decimal / 0x54 hex) and H (72 decimal /0x48 hex)
 *  - Distance reading in millimeters** (2 bytes per sensor): XX
 *  - Mask (1 byte) Each bit of this byte correspond to one sensor connected to the hub. 
 *  It gives an indication if the slotance corresponding to the sensor is new (bit at 1) or old (bit at 0):M
 *  - Checksum (1 byte) of previous 19 bytes: CRC8
 *
 *  **if a sensor is not connected or the TeraRanger Hub Evo is unable to obtain the
 *  slotance measurement from the TeraRanger Evo sensor, the associated slotance
 *  value is replaced by the hexadecimal value 0x0001.

*/
enum style {
    TEXT,
    BINARY
};

const byte styleList[2][4]       = {{0x00, 0x11, 0x01, 0x45},
                                    {0x00, 0x11, 0x02, 0x4C}};
/**
 * @param SIMULTANIOUS
 * Simultaneous mode supports simultaneous sensor operation. 
 * When using this mode it is important to configure your sensors in such a way
 * that their fields of view do not overlap and create the potential for sensor cross-talk.
 * @param SEQUENTIAL
 * Sequential mode ensures that sensors connected to Hub Evo are synchronized to avoid any signal interference 
 * between operating sensors. Operating sensors in sequential mode gives more freedom for 
 * the physical placement of the sensors but can result in a decrease in overall measurement repetition rates.
 * @param TOWER
 * With the Tower mode, the sensors are triggered up to 4 at a time: alternatively the
 * sensors connected on the odd port numbers of the Evo Hub, then the sensors
 * connected on the even port numbers. This mode works best when the sensors are
 * positioned in a configuration where crosstalk is likely
*/
enum mode {
    SIMULTANIOUS,
    SEQUENTIAL,
    TOWER
};

const byte modeList[3][4]        = {{0x00, 0x31, 0x01, 0xEB},
                                    {0x00, 0x31, 0x02, 0xE2},
                                    {0x00, 0x31, 0x03, 0xE5}};

/**
 * @brief Except ASAP all refresh rates are fixed.
 * 
 * @param REFRESH_ASAP
 * A variable rate which is dependent on how quickly the Hub receives an update from the last sensor in the chain.
 * @param REFRESH_50HZ  50 Hz refresh rate
 * @param REFRESH_100HZ 100 Hz refresh rate
 * @param REFRESH_250HZ 250 Hz refresh rate
 * @param REFRESH_500HZ 500 Hz refresh rate
 * @param REFRESH_600HZ 600 Hz refresh rate
*/
enum refreshRate {
    REFRESH_ASAP,
    REFRESH_50HZ,
    REFRESH_100HZ,
    REFRESH_250HZ,
    REFRESH_500HZ,
    REFRESH_600HZ
};

const byte refreshList[6][5]     = {{0x00, 0x52, 0x03, 0x01, 0xCA},
                                    {0x00, 0x52, 0x03, 0x02, 0xC3},
                                    {0x00, 0x52, 0x03, 0x03, 0xC4},
                                    {0x00, 0x52, 0x03, 0x04, 0xD1},
                                    {0x00, 0x52, 0x03, 0x05, 0xD6},
                                    {0x00, 0x52, 0x03, 0x06, 0xDF}};

/**
 * Baud rate of the Evo Hub UART-board with broken out connections. 
 * USB baud is fixed to 115200.
 */
#define HUB_BAUD            921600

/**
 *  If a sensor is not connected or the TeraRanger Hub Evo is unable to obtain the
 *  slotance measurement from the TeraRanger Evo sensor, the associated slotance
 *  value is replaced by the hexadecimal value “-1”.
*/
#define NO_SENSOR           -1

/**
 *  If the target is too close from the TeraRanger Evo sensor (below the minimum
 *  slotance), the associated slotance value is replaced by “-Inf”.
*/
#define TO_FAR              '-Inf'

/**
 *  If the target is too far from the TeraRanger Evo sensor (above the maximum
 *  slotance), the associated slotance value is replaced by “+Inf”.
*/
#define TO_CLOSE            '+Inf'

class TerabeeEvoHub {

    public:
        TerabeeEvoHub(HardwareSerial &serialPort, byte refreshRate, byte mode, byte style = BINARY);
        void start();
        void stop();
        void read(int &slot1);
        void read(int &slot1, int &slot2);
        void read(int &slot1, int &slot2, int &slot3);
        void read(int &slot1, int &slot2, int &slot3, int &slot4);
        void read(int &slot1, int &slot2, int &slot3, int &slot4, int &slot5);
        void read(int &slot1, int &slot2, int &slot3, int &slot4, int &slot5, int &slot6);
        void read(int &slot1, int &slot2, int &slot3, int &slot4, int &slot5, int &slot6, int &slot);
        void read(int &slot1, int &slot2, int &slot3, int &slot4, int &slot5, int &slot6, int &slot7, int &slot8);
        void update();

    private:
        uint8_t crc8(uint8_t *p, uint8_t len);
        uint8_t Framereceived[20];// The variable "Framereceived[]" will contain the frame sent by the TeraRanger
        int index = 0;
        HardwareSerial* _serialPort;
        byte _style;
        byte _refreshRate;
        byte _mode;

        char inData[20]; // Allocate some space for the string
        char inChar; // Where to store the character read
        bool tempBool = false;

        void translate();
        void reset();
        int dist, dist1, dist2, dist3, dist4, dist5, dist6, dist7 = 0;

};


#endif