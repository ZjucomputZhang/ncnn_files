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
        ImgShow(){ 
            name_label_[0] = cv::imread("../harm.jpg");
            name_label_[1] = cv::imread("../recycle.jpg");
            name_label_[2] = cv::imread("../food_waste.jpg");
            name_label_[3] = cv::imread("../other_waste.jpg");
            finish_flag_ = cv::imread("../sort_finished.jpg");
            warning_flag_ = cv::imread("../warning.jpg");
            // img_ = cv::Mat(640, 480, CV_8UC4, cv::Scalar(255, 255, 255, 255)); 
            img_ = cv::Mat(1920, 1080, name_label_[0].type(), cv::Scalar(255, 255, 255, 255)); 
            };
        ~ImgShow(){};

        void imgCreate(uint16_t index, uint16_t trash_class, uint16_t num) {
            index_ = index;
            trash_class_ = trash_class;
            trash_num_ = num;
        };
        void imgUpdate() {
            char text[256];
            cv::Rect roi1(50, 15 + index_ * text_interval_, name_label_[trash_class_].cols, name_label_[trash_class_].rows);
            cv::Rect roi2(300, 5 + index_ * text_interval_, finish_flag_.cols, finish_flag_.rows);   
            name_label_[trash_class_].copyTo(img_(roi1));
            finish_flag_.copyTo(img_(roi2));
            if (index_ > 9) {
                cv::Rect roi3(500, 5, warning_flag_.cols, warning_flag_.rows);   
                warning_flag_.copyTo(img_(roi3));
            }
            sprintf(text, "%d                 %d ", index_ + 1, trash_num_);
            cv::putText(img_, text, cv::Point(30, 30 + index_ * text_interval_), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0, 0), 2);
            cv::imshow("img", img_);
            cv::waitKey(25);
       };
        void imgClose() {
            cv::destroyWindow("img");
        }


    private:
        uint8_t trash_class_ = 0;
        uint8_t index_ = 0;
        uint8_t trash_num_ = 0;
        uint8_t text_interval_ = 30;
        cv::Mat img_;
        cv::Mat name_label_[4];
        cv::Mat finish_flag_;
        cv::Mat warning_flag_;
};      

};
/* Exported constants --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

#endif 
