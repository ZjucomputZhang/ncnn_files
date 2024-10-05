/* Includes ------------------------------------------------------------------*/
#include "hide_on_bush.hpp"
#include <stdint.h>
/* Private constants ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported function definitions ---------------------------------------------*/
/* Private function definitions ----------------------------------------------*/
namespace HideOnBush {

#define SHUTDOWN_THRES 25

static uint16_t shutdown_cnt = 0;
static bool is_img_created = false;

void Thread::suspendThread() {
    if (is_img_created) {
        shutdown_cnt ++;
        if (shutdown_cnt > SHUTDOWN_THRES) {
            img_ptr_->imgClose();
            shutdown_cnt = 0;
            is_img_created = false;
            return;
        }
    } else {
        shutdown_cnt = 0; 
    }
    return;
};

void Thread::detectThread(int index, int label, int num) {
    trash_class = 1; 
    if (label == 25) {
        trash_class = 2;
    } else if (label == 1 || label == 11 || label == 59) {
        trash_class = 0;
    } else if (label == 58) {
        trash_class = 3;
    }
    img_ptr_->imgCreate(index, trash_class, num);
    is_img_created = true;
};

void Thread::sortThread(uint16_t index) {
    img_ptr_->imgUpdate();  
    return;
};

};

