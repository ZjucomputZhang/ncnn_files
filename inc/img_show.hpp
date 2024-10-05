/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef IMG_SHOW_HPP
#define IMG_SHOW_HPP
/* Includes ------------------------------------------------------------------*/
#include "yolov5_lite.hpp"
#include "string.h"
#include <unistd.h>
/* Exported macro ------------------------------------------------------------*/

namespace HideOnBush {


class ImgShow {
    public:
        ImgShow(){};
        ~ImgShow(){};

        void imgCreate(uint16_t index, uint16_t trash_class, uint16_t num) {
            index_ = index;
            trash_class_ = trash_class;
            trash_num_ = num;
            cv::Mat img(640, 480, CV_8UC4, cv::Scalar(255, 255, 255, 255));
            char text[256];
            if (trash_class == 0) {
                sprintf(text, "%d harm trash %d sorting ..", index_ + 1, trash_num_);
            } else if (trash_class == 1) {
                sprintf(text, "%d recycle trash %d sorting ..", index_ + 1, trash_num_);
            } else if (trash_class == 2) {
                sprintf(text, "%d food waste %d sorting ..", index_ + 1, trash_num_);
            } else if (trash_class == 3) {
                sprintf(text, "%d other waste %d sorting ..", index_ + 1, trash_num_);
            }
            cv::putText(img, text, cv::Point(30, 240), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255, 255), 2);
            cv::imshow("img", img);
            cv::waitKey(25);
        };
        void imgUpdate() {
            cv::Mat img(640, 480, CV_8UC4, cv::Scalar(255, 255, 255, 255));
            char text[256];
            if (trash_class_ == 0) {
                sprintf(text, "%d harm trash %d sort finished!", index_ + 1, trash_num_);
            } else if (trash_class_ == 1) {
                sprintf(text, "%d recycle trash %d sort finished!", index_ + 1, trash_num_);
            } else if (trash_class_ == 2) {
                sprintf(text, "%d food waste %d sort finished!", index_ + 1, trash_num_);
            } else if (trash_class_ == 3) {
                sprintf(text, "%d other waste %d sort finished!", index_ + 1, trash_num_);
            }
            cv::putText(img, text, cv::Point(30, 240), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255, 255), 2);
            cv::imshow("img", img);
            cv::waitKey(25);
       };
        void imgClose() {
            cv::destroyWindow("img");
        }


    private:
        uint8_t trash_class_ = 0;
        uint8_t index_ = 0;
        uint8_t trash_num_ = 0;
};      

};
/* Exported constants --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

#endif 
