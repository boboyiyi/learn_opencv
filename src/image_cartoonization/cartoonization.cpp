#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
// #ifndef CVUI_IMPLEMENTATION
// #define CVUI_IMPLEMENTATION
// #endif
// #include "../cvui/cvui/cvui.h"

namespace _cartoonization {
void test() {
    cv::Mat img = cv::imread("./src/cvui/cvui/example/data/lena.jpg");
    cv::Mat gray;
    cv::cvtColor(img, gray, CV_BGR2GRAY);
    cv::Rect rect(0, 0, 10, 10);
    cv::Mat roi = gray(rect);
    unsigned char *input = (unsigned char*)(roi.data);
    input[0] = 0; input[1] = 0; input[10] = 0; input[11] = 9;
    std::cout << roi << std::endl;
    
    // blur
    cv::Mat blur_out;
    cv::blur(roi, blur_out, cv::Size(3, 3), cv::Point(-1, -1), cv::BORDER_DEFAULT);
    std::cout << blur_out << std::endl;
    // boxFilter
    cv::Mat boxFilter_norm_out, boxFilter_out;
    cv::boxFilter(roi, boxFilter_norm_out, -1, cv::Size(3, 3));
    std::cout << boxFilter_norm_out << std::endl;
    cv::boxFilter(roi, boxFilter_out, -1, cv::Size(3, 3), cv::Point(-1, -1), false);
    // std::cout << boxFilter_out << std::endl;
    // medianBlur
    cv::Mat medianBlur_out;
    cv::medianBlur(roi, medianBlur_out, 3);
    std::cout << medianBlur_out << std::endl;
    // filter2D
    cv::Mat filter2D_out;

    // GaussianBlur
    
    // bilateralFilter

    // blur
    //
}
void cartoonization() {
    // test();
    cv::Mat img = cv::imread("./src/cvui/cvui/example/data/lena.jpg");
    cv::Mat gray;
    while(true) {
        cv::cvtColor(img, gray, CV_BGR2GRAY);
        int MEDIAN_BLUR_FILTER_SIZE = 7;
        // apply a 7 x 7 median filter to the gray image for denoising
        // median filter can also keep edge sharp
        cv::medianBlur(gray, gray, MEDIAN_BLUR_FILTER_SIZE);
        cv::Mat edges;
        int LAPLACIAN_FILTER_SIZE = 5;
        // cv::Laplacian(gray, edges, CV_8U, LAPLACIAN_FILTER_SIZE);
        cv::Laplacian(gray, edges, CV_8U, 5);
        cv::imshow("edge", edges);
        cv::Mat mask;
        int EDGES_THRESHOLD = 80;
        // Laplacian之后边缘地方接近0，也就是黑色，而其他地方为接近白色，
        // 所以下面threshold用了THRESH_BINARY_INV，把黑白反过来，更容易直观理解。
        // THRESH_BINARY_INV 模式下edges中大于80的都为0，小于80的都为255
        cv::threshold(edges, mask, EDGES_THRESHOLD, 255, cv::THRESH_BINARY_INV);
        cv::Size size = img.size();
        cv::Size small_size = cv::Size(img.cols / 2, img.rows / 2);
        cv::Mat small_img = cv::Mat(small_size, CV_8UC3);
        cv::resize(img, small_img, small_size, 0,0, cv::INTER_LINEAR);
        cv::Mat tmp = cv::Mat(small_size, CV_8UC3);
        int repetitions = 7; // Repetitions for strong cartoon effect.
        for (int i=0; i<repetitions; i++) {
            int ksize = 9; // Filter size. Has a large effect on speed.
            double sigmaColor = 9; // Filter color strength.
            double sigmaSpace = 7; // Spatial strength. Affects speed.
            cv::bilateralFilter(small_img, tmp, ksize, sigmaColor, sigmaSpace);
            cv::bilateralFilter(tmp, small_img, ksize, sigmaColor, sigmaSpace);
        }
        cv::Mat big_img;
        cv::resize(small_img, big_img, size, 0,0, cv::INTER_LINEAR);
        cv::Mat out = cv::Mat(size, CV_8UC3);
        out.setTo(0);
        big_img.copyTo(out, mask);


        cv::imshow("ori", img);
        cv::imshow("out", out);
        if (cv::waitKey(30) == 27) {
            break;
        }
    }
}
}