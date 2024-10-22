/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef IMG_SHOW_HPP
#define IMG_SHOW_HPP
/* Includes ------------------------------------------------------------------*/
#include "yolov5_lite.hpp"
#include "string.h"
#include <unistd.h>
#include "wiringPi.h"
#include "wiringSerial.h"

#define MAXLEN 5

class TSerial {
  public:
     struct DataPackage {
      struct rx_pack_t {
        uint8_t is_task_finished = 0;
      } rx_pack;
      struct tx_pack_t {
        uint8_t sort_start = 0;
        uint8_t trash_class = 0; // default: harm trash
      } tx_pack;
    };
    DataPackage serial_data;

    TSerial(){};
    ~TSerial(){};

    void setup() {
      fd = serialOpen("/dev/ttyUSB0", 9600);
      serial_data.rx_pack.is_task_finished = 0;
      serial_data.tx_pack.sort_start = 0;
      serial_data.tx_pack.trash_class = 0; 
    };

    void run() {
        if (serialDataAvail(fd) <= 0) {
            return;
        }
        dataRec();
        decoder();
        encoder();
        dataSend();
    }
    void dataRec() {
        uint8_t byte = Serial.read();
        if (rec_state == 0 && byte == 0xAA) {
            rec_state = 1;
            rx_data[0] = byte;
        } else if (rec_state == 1 && byte == 0xBB) {
            rec_state = 2;
            rx_data[1] = byte;
        } else if (rec_state == 2) {
            rx_data[rx_index ++] = byte;
        } else if (byte == 0xFF) {
            if (rx_index != 5) {
            memset(rx_data, 0, MAXLEN*sizeof(uint8_t));
            }
            rec_state = 0;
            rx_index = 2;
        }
    };
    void decoder() {
        if (rx_data[2] == 0x00) {
            serial_data.rx_pack.is_task_finished = 0;    
        } else if (rx_data[2] == 0x01){
            serial_data.rx_pack.is_task_finished = 1;
        }
    }
    void encoder() {
        tx_data[0] = 0xAA;
        tx_data[1] = 0xBB;
        if (serial_data.tx_pack.trash_class == 0) {
            tx_data[2] = 0x00;
        } else if (serial_data.tx_pack.trash_class == 1) {
            tx_data[2] = 0x01;
        } else if (serial_data.tx_pack.trash_class == 2) {
            tx_data[2] = 0x02;
        } else {
            tx_data[2] = 0x03;
        }
        if (serial_data.tx_pack.sort_start == 0) {
            tx_data[3] = 0x00;
        } else {
            tx_data[3] = 0x01;
        }
        tx_data[4] = 0xFF;
    }
    void dataSend() {
        SerialPutchar(fd, &tx_data[tx_index ++]);
        if (tx_index >= MAXLEN) {
            tx_index = 0;
        }
    }

    int fd;
    uint8_t tx_data[MAXLEN] = {0};
    uint8_t rx_data[MAXLEN] = {0};
    uint8_t rec_state = 0;
    uint8_t rx_index = 2;
    uint8_t tx_index = 0;
};
/* Exported macro ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

#endif 