#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#define PROGRAM_NAME create_mask

namespace PROGRAM_NAME{
cv::Mat img, call_back_img;
int mask_type = 0;
int	drag = 0;
cv::Point point;
}

static void
mouse_call_back_rectangle(int event, int x, int y, int flags, void* userdata) {
    PROGRAM_NAME::call_back_img = PROGRAM_NAME::img.clone();
    cv::putText(PROGRAM_NAME::call_back_img, "Mask type is: rectangle.", cv::Point(0, 30), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 0, 255), 1);
    if (event == cv::EVENT_LBUTTONDOWN && !PROGRAM_NAME::drag) {
        PROGRAM_NAME::point = cv::Point(x, y);
        PROGRAM_NAME::drag = 1;
    }
    if (event == cv::EVENT_MOUSEMOVE && PROGRAM_NAME::drag) {
        cv::rectangle(PROGRAM_NAME::call_back_img, PROGRAM_NAME::point, cv::Point(x, y), cv::Scalar(0, 255, 0));
        cv::imshow("create_mask", PROGRAM_NAME::call_back_img);
    }
    // save mask
    if (event == cv::EVENT_LBUTTONUP && PROGRAM_NAME::drag) {
        PROGRAM_NAME::drag = 0;
    }
}

static void
mouse_call_back_ellipse(int event, int x, int y, int flags, void* userdata) {
    PROGRAM_NAME::call_back_img = PROGRAM_NAME::img.clone();
    cv::putText(PROGRAM_NAME::call_back_img, "Mask type is: ellipse.", cv::Point(0, 30), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 0, 255), 1);
    if (event == cv::EVENT_LBUTTONDOWN && !PROGRAM_NAME::drag) {
        PROGRAM_NAME::point = cv::Point(x, y);
        PROGRAM_NAME::drag = 1;
    }
    if (event == cv::EVENT_MOUSEMOVE && PROGRAM_NAME::drag) {
        cv::ellipse(PROGRAM_NAME::call_back_img, cv::Point(int((x + PROGRAM_NAME::point.x) / 2), int((y + PROGRAM_NAME::point.y) / 2) ), cv::Size(std::abs(PROGRAM_NAME::point.x - x) / 2, std::abs(PROGRAM_NAME::point.y - y) / 2), 0, 0, 360, cv::Scalar( 0, 255, 0 ));
        cv::imshow("create_mask", PROGRAM_NAME::call_back_img);
    }
    // save mask
    if (event == cv::EVENT_LBUTTONUP && PROGRAM_NAME::drag) {
        PROGRAM_NAME::drag = 0;
    }
}

static void
mouse_call_back_polygon(int event, int x, int y, int flags, void* userdata) {
    cv::Point s;
    cv::Mat tmp_img;
    cv::putText(PROGRAM_NAME::call_back_img, "Mask type is: polygon.", cv::Point(0, 30), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 0, 255), 1);
    if (event == cv::EVENT_LBUTTONDOWN && !PROGRAM_NAME::drag) {
        PROGRAM_NAME::point = cv::Point(x, y);
        s = PROGRAM_NAME::point;
        cv::circle(PROGRAM_NAME::call_back_img, PROGRAM_NAME::point, 2, cv::Scalar(0, 255, 0));
        PROGRAM_NAME::drag = 1;
    }
    if (event == cv::EVENT_LBUTTONDOWN && PROGRAM_NAME::drag) {
        cv::circle(PROGRAM_NAME::call_back_img, PROGRAM_NAME::point, 2, cv::Scalar(0, 255, 0));
        cv::line(PROGRAM_NAME::call_back_img, PROGRAM_NAME::point, cv::Point(x, y), cv::Scalar(0, 0, 255));
        PROGRAM_NAME::point = cv::Point(x, y);
        cv::imshow("create_mask", PROGRAM_NAME::call_back_img);
    }
    // save mask
    // if (event == cv::EVENT_LBUTTONUP && drag) {
    //     drag = 0;
    // }

}

static void
on_trackbar( int, void* ) {
    PROGRAM_NAME::drag = 0;
    PROGRAM_NAME::call_back_img = PROGRAM_NAME::img.clone();
    cv::Mat local_img = PROGRAM_NAME::img.clone();
    std::string msg = "Mask type is: ";
    if (PROGRAM_NAME::mask_type == 0) {
        msg += "rectangle.";
        cv::setMouseCallback("create_mask", mouse_call_back_rectangle, NULL);
    }
    else if (PROGRAM_NAME::mask_type == 1) {
        msg += "ellipse.";
        cv::setMouseCallback("create_mask", mouse_call_back_ellipse, NULL);
    }
    else {
        msg += "polygon.";
        cv::setMouseCallback("create_mask", mouse_call_back_polygon, NULL);
    }
    cv::putText(local_img, msg, cv::Point(0, 30), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 0, 255), 1);
    cv::imshow("create_mask", local_img);
}

namespace _create_mask {
int PROGRAM_NAME() {
    PROGRAM_NAME::img = cv::imread("./src/create_mask/data/wyz.jpg");
    cv::namedWindow("create_mask");
    cv::setMouseCallback("create_mask", mouse_call_back_rectangle, NULL);
    cv::Mat tmp_img = PROGRAM_NAME::img.clone();
    cv::putText(tmp_img, "Mask type is: rectangle.", cv::Point(0, 30), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 0, 255), 1);
    cv::imshow("create_mask", tmp_img);
    cv::createTrackbar("mask_type", "create_mask", &PROGRAM_NAME::mask_type, 2, on_trackbar);
    cv::waitKey(0);
    return 0;
}
}