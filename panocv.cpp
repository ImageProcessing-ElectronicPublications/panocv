/*
 * panorama.cpp
 *
 *  Created on: Jun 26, 2012
 *      Author: miquel
 */

#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/stitching.hpp>

int main(int argc, char* argv[])
{
    int num_imgs=0;
    cv::Stitcher stitcher = cv::Stitcher::createDefault();

    std::cout << "\n[PANORAMA]: creation of a panorama from a two or more images." << std::endl;

    if (argc < 3)
    {
        std::cout << "\tUsage: " << argv[0] << " output.jpg input1.jpg input2.jpg... " << std::endl;
        return -1;
    }

    num_imgs = argc - 2;
    std::string output = argv[1];
    std::vector<cv::Mat> imgs(num_imgs);

    std::cout << "[I] Input: " << num_imgs << " images." << std::endl;

    //try to open the images:
    for(int i=0; i<num_imgs; i++)
    {
        std::cout << "[I] Opening image " << argv[2+i] << std::endl;
        try
        {
            imgs[i]=cv::imread(argv[2+i]);
            if( !imgs[i].data )
                throw "Could not read image";
            std::cout << "[I] Image is " << imgs[i].cols << "x" << imgs[i].rows << std::endl;
        }
        catch( char * str )
        {
            std::cout << "Exception raised: " << str  << argv[2+i] << std::endl;
        }
    }

    //find features
    cv::Ptr<cv::detail::FeaturesFinder> features_finder(new cv::detail::OrbFeaturesFinder(cv::Size(3,1), 1500, 1.2f, 8));
    stitcher.setFeaturesFinder(features_finder);

    cv::Mat pano;

    std::cout << "[I] Stitching..." << std::endl;

    cv::Stitcher::Status status = stitcher.stitch(imgs, pano);
    if (status != cv::Stitcher::OK)
    {
        std::cout << "Can't stitch images, error code = " << status << std::endl;
        return -1;
    }

    std::cout << "[I] Writing pano in " << output << std::endl;
    std::cout << "[I] Pano is " << pano.cols << "x" << pano.rows << std::endl;
    cv::imwrite(output,pano);

    return 0;
}
