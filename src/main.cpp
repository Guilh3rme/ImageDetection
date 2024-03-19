#include <iostream>
#include <opencv2/opencv.hpp>

int main() {
    // Read an image
    cv::Mat image = cv::imread("path_to_your_image.jpg");

    if (image.empty()) {
        std::cerr << "Failed to load image" << std::endl;
        return 1;
    }

    // Display the image
    cv::imshow("Image", image);
    cv::waitKey(0); // Wait for any key press

    return 0;
}
