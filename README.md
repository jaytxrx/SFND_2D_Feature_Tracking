# SFND 2D Feature Tracking

For project writeup, refer [Mid-Term_ProjectReport.md](Mid-Term_ProjectReport.md)

<img src="images/keypoints.png" width="820" height="248" />

The idea of the camera course is to build a collision detection system - that's the overall goal for the Final Project. As a preparation for this, you will now build the feature tracking part and test various detector / descriptor combinations to see which ones perform best. This mid-term project consists of four parts:

* First, you will focus on loading images, setting up data structures and putting everything into a ring buffer to optimize memory load. 
* Then, you will integrate several keypoint detectors such as HARRIS, FAST, BRISK and SIFT and compare them with regard to number of keypoints and speed. 
* In the next part, you will then focus on descriptor extraction and matching using brute force and also the FLANN approach we discussed in the previous lesson. 
* In the last part, once the code framework is complete, you will test the various algorithms in different combinations and compare them with regard to some performance measures. 

See the classroom instruction and code comments for more details on each of these parts. Once you are finished with this project, the keypoint matching part will be set up and you can proceed to the next lesson, where the focus is on integrating Lidar points and on object detection using deep-learning. 

## Dependencies for Running Locally
* cmake >= 2.8
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* OpenCV >= 4.1
  * This must be compiled from source using the `-D OPENCV_ENABLE_NONFREE=ON` cmake flag for testing the SIFT and SURF detectors.
  * The OpenCV 4.1.0 source code can be found [here](https://github.com/opencv/opencv/tree/4.1.0)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./2D_feature_tracking`.

### Note: In this repo, I have done certain changes to generate CSV file
The defines `#define MIDTERMLOOP` and `#define CSVOUT` in matching2D.hpp can be adjusted for different outputs
1. Activate `#define MIDTERMLOOP` only to view each results on the commandline
2. Activate `#define MIDTERMLOOP` and `#define CSVOUT` to get CSV output which can be opend in LibreOffice Cals, Microsoft Excel or Google Sheets.
On Linux you can redirect the outout to csv file using the following command `./2D_feature_tracking > report.csv`
If you cannot run the above redirect command, then `./2D_feature_tracking` and then paste the contents to file `report.csv`

## OpenCV Installation
To use this repo, you need to install the opencv and opencv_contrib package
Below steps were tried on Ubuntu 20.04

1. Install OpenCV, follow the below steps
```
git clone https://github.com/opencv/opencv.git
cd opencv
git checkout 4.1.0
cd .. #get out of opencv folder
```

2. Now install opencv_contrib
```
https://github.com/opencv/opencv_contrib/
cd opencv_contrib
git checkout 4.1.0
cd .. #get out of opencv_contrib folder
```

3. lets create a build directory inside opencv folder and install opencv where we will refer opencv_contrib
```
cd opencv
mkdir build
cd build
cmake -D CMAKE_BUILD_TYPE=RELEASE -D OPENCV_ENABLE_NONFREE=ON -D OPENCV_EXTRA_MODULES_PATH=../../opencv_contrib/modules -D WITH_GTK=ON ..
make
sudo make install
```

While running CMAKE on Ubuntu 20.04, there were some errors. I had to install the following packages.
```
sudo apt-get install libgtk2.0-dev pkg-config
sudo apt install gnome-themes-standard
```
