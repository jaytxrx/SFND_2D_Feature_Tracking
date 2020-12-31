/* INCLUDES FOR THIS PROJECT */
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <limits>
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>

#include "dataStructures.h"
#include "matching2D.hpp"

using namespace std;

/* MAIN PROGRAM */
int main(int argc, const char *argv[])
{

    /* INIT VARIABLES AND DATA STRUCTURES */

    // data location
    string dataPath = "../";

    // camera
    string imgBasePath = dataPath + "images/";
    string imgPrefix = "KITTI/2011_09_26/image_00/data/000000"; // left camera, color
    string imgFileType = ".png";
    int imgStartIndex = 0; // first file index to load (assumes Lidar and camera names have identical naming convention)
    int imgEndIndex = 9;   // last file index to load
    int imgFillWidth = 4;  // no. of digits which make up the file index (e.g. img-0001.png)

    // misc
    int dataBufferSize = 2;       // no. of images which are held in memory (ring buffer) at the same time
    vector<DataFrame> dataBuffer; // list of data frames which are held in memory at the same time
    bool bVis = false;            // visualize results

#ifdef MIDTERMLOOP
    //For MidTerm Project Evaluation MP.7, MP.8, and MP.9
    vector<string> detector_type_names = {"SHITOMASI", "HARRIS", "FAST", "BRISK", "ORB", "AKAZE", "SIFT"};
    vector<string> descriptor_type_names = {"BRISK", "BRIEF", "ORB", "FREAK", "AKAZE", "SIFT"};

    vector<int> kptsVehicle;
    vector<int> matchedKpts;

#ifdef CSVOUT
            cout<<"Detector Type,Descriptor Type,Average Keypoints,Matched Keypoints,Avg Descriptor t(ms),Avg Detector t(ms)"<< endl;
#endif

    for(auto detector_type_name:detector_type_names) // start loop detector_types
    {
        for(auto descriptor_type_name:descriptor_type_names) // start loop descriptor_types
        {
            if(detector_type_name.compare("AKAZE")!=0 && descriptor_type_name.compare("AKAZE")==0)
                continue; //N/A

            if(detector_type_name.compare("AKAZE")==0 && descriptor_type_name.compare("AKAZE")==0)
                continue;

            if(detector_type_name.compare("SIFT")==0 && descriptor_type_name.compare("ORB")==0)
                continue; //out of memory issue

#ifndef CSVOUT
            cout << "==STARTING========Detector Type: " << detector_type_name << "   Descriptor Type: " << descriptor_type_name << "======="<< endl;
#endif

            dataBuffer.clear(); //start from a fresh image buffer
#endif

    /* MAIN LOOP OVER ALL IMAGES */

    for (size_t imgIndex = 0; imgIndex <= imgEndIndex - imgStartIndex; imgIndex++)
    {
        /* LOAD IMAGE INTO BUFFER */

        // assemble filenames for current index
        ostringstream imgNumber;
        imgNumber << setfill('0') << setw(imgFillWidth) << imgStartIndex + imgIndex;
        string imgFullFilename = imgBasePath + imgPrefix + imgNumber.str() + imgFileType;

        // load image from file and convert to grayscale
        cv::Mat img, imgGray;
        img = cv::imread(imgFullFilename);
        cv::cvtColor(img, imgGray, cv::COLOR_BGR2GRAY);

        //// STUDENT ASSIGNMENT
        //// TASK MP.1 -> replace the following code with ring buffer of size dataBufferSize

        // push image into data frame buffer
        DataFrame frame;
        frame.cameraImg = imgGray;
        dataBuffer.push_back(frame);

        /* deleting from the buffer to make it a ring buffer */
        // as the data will be pushed to the buffer one at a time, below logic will work
        if(dataBuffer.size() == (dataBufferSize+1)) //wait for buffer to have atleast "dataBufferSize" elements before deleting
        {
            dataBuffer.erase (dataBuffer.begin());
        }

#ifndef MIDTERMLOOP
        //// EOF STUDENT ASSIGNMENT
        cout << "#1 : LOAD IMAGE INTO BUFFER done" << endl;
#endif

        /* DETECT IMAGE KEYPOINTS */

        // extract 2D keypoints from current image
        vector<cv::KeyPoint> keypoints; // create empty feature list for current image

#ifdef MIDTERMLOOP
        string detectorType = detector_type_name;
#else
        string detectorType = "AKAZE";
#endif

        //// STUDENT ASSIGNMENT
        //// TASK MP.2 -> add the following keypoint detectors in file matching2D.cpp and enable string-based selection based on detectorType
        //// -> HARRIS, FAST, BRISK, ORB, AKAZE, SIFT

        if (detectorType.compare("SHITOMASI") == 0)
        {
            detKeypointsShiTomasi(keypoints, imgGray, false);
        }
        else if (detectorType.compare("HARRIS") == 0)
        {
            detKeypointsHarris(keypoints, imgGray, false);
        }
        else
        {
            /* FAST, BRISK, ORB, AKAZE, SIFT */
            detKeypointsModern(keypoints, imgGray, detectorType, false);
        }

        //// EOF STUDENT ASSIGNMENT

        //// STUDENT ASSIGNMENT
        //// TASK MP.3 -> only keep keypoints on the preceding vehicle

        // only keep keypoints on the preceding vehicle
        bool bFocusOnVehicle = true;
        cv::Rect vehicleRect(535, 180, 180, 150);
        if (bFocusOnVehicle)
        {
            for (auto it = keypoints.begin(); it != keypoints.end(); /* NOTHING */)
            {
                if (vehicleRect.contains(it->pt))
                    ++it;
                else
                    it = keypoints.erase(it);
            }
        }

#ifndef MIDTERMLOOP
        //MP.7 number of keypoints on the preceding vehicle for all 10 images
        cout << "Number of keypoints on the preceding vehicle are "<<keypoints.size() << endl;
#endif

#ifdef MIDTERMLOOP
        kptsVehicle.push_back(keypoints.size());
#endif

        //// EOF STUDENT ASSIGNMENT

        // optional : limit number of keypoints (helpful for debugging and learning)
        bool bLimitKpts = false;
        if (bLimitKpts)
        {
            int maxKeypoints = 50;

            if (detectorType.compare("SHITOMASI") == 0)
            { // there is no response info, so keep the first 50 as they are sorted in descending quality order
                keypoints.erase(keypoints.begin() + maxKeypoints, keypoints.end());
            }
            cv::KeyPointsFilter::retainBest(keypoints, maxKeypoints);
#ifndef MIDTERMLOOP
            cout << " NOTE: Keypoints have been limited!" << endl;
#endif
        }

        // push keypoints and descriptor for current frame to end of data buffer
        (dataBuffer.end() - 1)->keypoints = keypoints;
#ifndef MIDTERMLOOP
        cout << "#2 : DETECT KEYPOINTS done" << endl;
#endif

        /* EXTRACT KEYPOINT DESCRIPTORS */

        //// STUDENT ASSIGNMENT
        //// TASK MP.4 -> add the following descriptors in file matching2D.cpp and enable string-based selection based on descriptorType
        //// -> BRIEF, ORB, FREAK, AKAZE, SIFT

        cv::Mat descriptors;

#ifdef MIDTERMLOOP
        string descriptorType = descriptor_type_name;
#else
        string descriptorType = "AKAZE"; // BRIEF, ORB, FREAK, AKAZE, SIFT
#endif

        descKeypoints((dataBuffer.end() - 1)->keypoints, (dataBuffer.end() - 1)->cameraImg, descriptors, descriptorType);
        //// EOF STUDENT ASSIGNMENT

        // push descriptors for current frame to end of data buffer
        (dataBuffer.end() - 1)->descriptors = descriptors;

#ifndef MIDTERMLOOP
        cout << "#3 : EXTRACT DESCRIPTORS done" << endl;
#endif

        if (dataBuffer.size() > 1) // wait until at least two images have been processed
        {

            /* MATCH KEYPOINT DESCRIPTORS */

            vector<cv::DMatch> matches;
            string matcherType = "MAT_BF";        // MAT_BF, MAT_FLANN
            //below line comment to avoid confusion. New name is descriptorClass
            //string descriptorType = "DES_BINARY"; // DES_BINARY, DES_HOG
            string selectorType = "SEL_KNN";       // SEL_NN, SEL_KNN
            //KNN selected for MidTermProject "TASK MP.8 use the BF approach with the descriptor distance ratio set to 0.8."

            string descriptorClass{};
            if (descriptorType.compare("SIFT")==0) 
            {
                descriptorClass = "DES_HOG";
            } 
            else 
            {
                descriptorClass = "DES_BINARY";
            }

            //// STUDENT ASSIGNMENT
            //// TASK MP.5 -> add FLANN matching in file matching2D.cpp
            //// TASK MP.6 -> add KNN match selection and perform descriptor distance ratio filtering with t=0.8 in file matching2D.cpp

            matchDescriptors((dataBuffer.end() - 2)->keypoints, (dataBuffer.end() - 1)->keypoints,
                             (dataBuffer.end() - 2)->descriptors, (dataBuffer.end() - 1)->descriptors,
                             matches, descriptorClass, matcherType, selectorType);

            //// EOF STUDENT ASSIGNMENT

            // store matches in current data frame
            (dataBuffer.end() - 1)->kptMatches = matches;

#ifndef MIDTERMLOOP
            //MP.8 matched keypoints
            cout << "Number of matched keypoints are "<< matches.size() << endl;
#endif

#ifdef MIDTERMLOOP
        matchedKpts.push_back(matches.size());
#endif

#ifndef MIDTERMLOOP
            cout << "#4 : MATCH KEYPOINT DESCRIPTORS done" << endl;
#endif

            // visualize matches between current and previous image
            bVis = true;

#ifdef MIDTERMLOOP
            bVis = false;
#endif

            if (bVis)
            {
                cv::Mat matchImg = ((dataBuffer.end() - 1)->cameraImg).clone();
                cv::drawMatches((dataBuffer.end() - 2)->cameraImg, (dataBuffer.end() - 2)->keypoints,
                                (dataBuffer.end() - 1)->cameraImg, (dataBuffer.end() - 1)->keypoints,
                                matches, matchImg,
                                cv::Scalar::all(-1), cv::Scalar::all(-1),
                                vector<char>(), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

                string windowName = "Matching keypoints between two camera images";
                cv::namedWindow(windowName, 7);
                cv::imshow(windowName, matchImg);
                cout << "Press key to continue to next image" << endl;
                cv::waitKey(0); // wait for key to be pressed
            }
            bVis = false;
        }

    } // eof loop over all images

#ifdef MIDTERMLOOP

    double avg_kptsVehicle=double(std::accumulate(kptsVehicle.begin(), kptsVehicle.end(), 0.0) / kptsVehicle.size());
    double avg_matchedKpts=double(std::accumulate(matchedKpts.begin(), matchedKpts.end(), 0.0) / matchedKpts.size());
    double avg_descriptorTime=double(std::accumulate(descriptorTime.begin(), descriptorTime.end(), 0.0) / descriptorTime.size());
    double avg_detectorTime=double(std::accumulate(detectorTime.begin(), detectorTime.end(), 0.0) / detectorTime.size());

#ifdef CSVOUT
    cout<<detector_type_name<<","<<descriptor_type_name<<","<< avg_kptsVehicle<<","<< avg_matchedKpts<<","<< avg_descriptorTime<<","<<avg_detectorTime << endl;
#else
    cout << "=========================MidTerm Project Report====================" << endl;
    cout << "Detector Type: " << detector_type_name << "   Descriptor Type: " << descriptor_type_name << endl;
    cout << "===================================================================" << endl;

    cout << "Average Keypoints on vehicle: " <<avg_kptsVehicle<< endl;
    cout << "Average Matched Keypoints: " <<avg_matchedKpts<< endl;
    cout << "Average Descriptor Time: " <<avg_descriptorTime<< " ms"<< endl;
    cout << "Average Detector Time: " <<avg_detectorTime<< " ms"<<endl;

    cout << "=============================REPORT END============================" << endl;
#endif

    //empty all the buffers
    descriptorTime.clear();
    detectorTime.clear();
    kptsVehicle.clear();
    matchedKpts.clear();

#endif

#ifdef MIDTERMLOOP
        } // end loop descriptor_types
    } // end loop detector_types
#endif

    return 0;
}
