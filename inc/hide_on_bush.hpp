/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef HIDE_ON_BUSH_HPP
#define HIDE_ON_BUSH_HPP
/* Includes ------------------------------------------------------------------*/
#include "yolov5_lite.hpp"
#include "video_broadcast.hpp"
#include "img_show.hpp"
/* Exported macro ------------------------------------------------------------*/

namespace HideOnBush {

enum ThreadState : uint8_t {
    KThreadStateSuspend,
    KThreadStateDetect,
    kThreadStateSort,
    KTHreadStateNum,
};


class Thread {
    public:
        Thread(){};
        ~Thread(){};

        void suspendThread();
        void detectThread(int index, int label, int num);
        void sortThread(uint16_t index);
        void registerImgShow(ImgShow* ptr) {
            img_ptr_ = ptr;
        }

        ThreadState thread_state = ThreadState::KThreadStateSuspend;
        uint8_t trash_class = 0;
    private:
        ImgShow* img_ptr_ = nullptr;
        

};

};
/* Exported constants --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

#endif 
