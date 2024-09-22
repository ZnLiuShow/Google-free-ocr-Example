// tesseractocr.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <opencv2/opencv.hpp>
using namespace tesseract;


PIX* cvtMat2PIX(cv::Mat imgGray)
{
    int cols = imgGray.cols;
    int rows = imgGray.rows;

    PIX* pixS = pixCreate(cols, rows, 8);

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            pixSetPixel(pixS, j, i, (l_uint32)imgGray.at<uchar>(i, j));
    return pixS;
}

int main()
{
    // 初始化TesseractOCR
    char* outText;
    tesseract::TessBaseAPI* api = new tesseract::TessBaseAPI();
    if (api->Init(NULL, "chi_sim")) {  // "chi_sim" for Simplified Chinese
        fprintf(stderr, "Could not initialize tesseract.\n");
        exit(1);
    }

    // 设置识别白色文本
    api->SetVariable("tessedit_char_black_list", "");

    // 加载图片
    cv::Mat img = cv::imread("1.png", cv::IMREAD_COLOR);
    if (img.empty()) {
        fprintf(stderr, "Could not open or find the image.\n");
        exit(1);
    }

    // 转换图片格式从BGR到灰度
    cv::Mat img_gray;
    cv::cvtColor(img, img_gray, cv::COLOR_BGR2GRAY);

    // 二值化图片
    cv::Mat img_binary;
    cv::threshold(img_gray, img_binary, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    cv::imshow("1", img_binary);
    cv::waitKey(0);
    // 将图片转换成TesseractOCR可以处理的图片格式
    Pix* pix = cvtMat2PIX(img_binary);

    // 运行OCR识别
    api->SetImage(pix);
    outText = api->GetUTF8Text();
    printf("OCR output:\n%s", outText);

    // 清理
    api->End();
    delete[] outText;
    pixDestroy(&pix);

    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
