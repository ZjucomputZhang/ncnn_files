/* Includes ------------------------------------------------------------------*/
#include "hide_on_bush.hpp"
#include "img_show.hpp"
#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <signal.h>
#include <thread>
#include <unistd.h>

namespace HideOnBush = HideOnBush;
/* Private constants ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define LOCK_THRES                  10
#define NONE_OBJECT_DETECTED        100
/* Private types -------------------------------------------------------------*/
typedef enum TrashClass : uint8_t {
    kTrashClassHarm,
    kTrashClassRecycle,
    kTrashClassKitchen,
    kTrashCLassOther,
} trash_class_e;

typedef struct CNT {
    uint16_t lock_cnt = 0;
    uint16_t failed_cnt = 0;
} cnt_t;

typedef struct ObjData {
    uint16_t trash_index = 0;
    uint16_t trash_label = NONE_OBJECT_DETECTED;
    trash_class_e trash_class = TrashClass::kTrashClassHarm;
    uint16_t trash_num = 0;
} obj_data_t;
/* Private variables ---------------------------------------------------------*/
HideOnBush::Thread* thread_ptr = nullptr;
HideOnBush::ImgShow* img_ptr = nullptr;
static cnt_t cnt;
static obj_data_t obj_data;
static uint16_t object_index_last = 0;
static uint16_t locked_object_index = 0;
static uint32_t sys_tick = 0;
/* External variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported function definitions ---------------------------------------------*/
/* Private function definitions ----------------------------------------------*/
/* run reset & update*/
static void Update();
static void Reset();
static void Run();
/* Timers*/
static void SigactionInit();
static void TimerInit();
/* boot*/
static void Boot();
/* functions*/
static uint16_t ObjectLock();

static uint16_t ObjectLock() {
    uint16_t object_index_curr = GetDetectLabel();
    if ((object_index_curr != object_index_last || object_index_curr == NONE_OBJECT_DETECTED ) && cnt.lock_cnt < LOCK_THRES) {
        cnt.lock_cnt = 0;
        object_index_last = object_index_curr;
        return NONE_OBJECT_DETECTED;
    } else {
        cnt.lock_cnt ++;
    }
    if (cnt.lock_cnt > LOCK_THRES) {
        cnt.lock_cnt = 0;
        locked_object_index = object_index_last;
        return locked_object_index;
    } else {
        return NONE_OBJECT_DETECTED;
    }
    return NONE_OBJECT_DETECTED;
}



static void Update() {
    printf("<--------------------------------------------------------------->\n");
    printf("trash class: %d\n", thread_ptr->trash_class);
    printf("detetcted %d objects!\n", GetDetectNum());
    if (GetDetectNum() && thread_ptr->thread_state == HideOnBush::ThreadState::KThreadStateSuspend) {
        thread_ptr->thread_state = HideOnBush::ThreadState::KThreadStateDetect;
    } else if (thread_ptr->thread_state == HideOnBush::ThreadState::KThreadStateDetect) {
        obj_data.trash_label = ObjectLock();
        if (obj_data.trash_label == NONE_OBJECT_DETECTED) {
            cnt.failed_cnt ++;
            if (cnt.failed_cnt > LOCK_THRES) {
                thread_ptr->thread_state = HideOnBush::ThreadState::KThreadStateSuspend;
                cnt.failed_cnt = 0;
            }
        } else {
            thread_ptr->thread_state = HideOnBush::ThreadState::kThreadStateSort;
        }
    }
    obj_data.trash_class = (trash_class_e)thread_ptr->trash_class;
    obj_data.trash_num = 1;
};

static void Reset() {
    DetectSet(640,480); 
    VideoSet();   
};

static void Run() {
    if (thread_ptr->thread_state == HideOnBush::ThreadState::KThreadStateSuspend) {
        thread_ptr->suspendThread();
    } else if (thread_ptr->thread_state == HideOnBush::ThreadState::KThreadStateDetect) {
        thread_ptr->detectThread(obj_data.trash_index, obj_data.trash_label, obj_data.trash_num);

   } else if (thread_ptr->thread_state == HideOnBush::ThreadState::kThreadStateSort) {
        thread_ptr->sortThread(locked_object_index);
        thread_ptr->thread_state = HideOnBush::ThreadState::KThreadStateSuspend;
        obj_data.trash_index ++; 
    }
    Reset();
    VideoBroadcast();
    Yolov5liteDetect();
};

void TimerCallBack(int signo) {
    Update();

    sys_tick ++;
};

static void SigactionInit() {
    struct sigaction act;
    act.sa_handler = TimerCallBack;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    sigaction(SIGPROF, &act, NULL);
};

static void TimerInit() {
    struct itimerval value;
    value.it_value.tv_sec = 1;
    value.it_value.tv_usec = 0;
    value.it_interval.tv_sec = 0;
    value.it_interval.tv_usec = 200*1000;       //200 ms
    setitimer(ITIMER_PROF, &value, NULL);
};

static void Boot() {
    memset(&obj_data, 0, sizeof(obj_data_t));
    img_ptr = new HideOnBush::ImgShow();
    thread_ptr = new HideOnBush::Thread();
    thread_ptr->registerImgShow(img_ptr);
    thread_ptr->thread_state = HideOnBush::ThreadState::KThreadStateSuspend;              
    SigactionInit();
    TimerInit();
    Reset();
};



int main(int argc, char** argv) {
    Boot();
    while (true) {
        Run();
    }
};


