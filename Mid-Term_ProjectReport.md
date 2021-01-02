## Mid-Term Project Report - Camera Based 2D Feature Tracking

This report describes how the assignments points were addressed.

All the tables shown below were generated from [report.csv](report.csv)

### MP.1 Data Buffer Optimization

Ring buffer is implemented in the file *MidTermProject_Camera_Student.cpp* by checking the size of the buffer. If the buffer size is more than the configured buffer size, the first element in the buffer is deleted using erase function of the vector.

### MP.2 Keypoint Detection

In the file *matching2D_Student.cpp*, two functions were newly implemented *detKeypointsHarris()* and *detKeypointsModern()*

*detKeypointsHarris()* is similar to the function *detKeypointsShiTomasi()* with only a difference in the boolean flag **useHarrisDetector** passed to *cv::goodFeaturesToTrack()*. In  *detKeypointsHarris()* , the flag is set to **TRUE**.

*detKeypointsModern()* implemented the detectors FAST, BRISK, ORB, AKAZE, SIFT. After creating  each of detector using the corresponding function *create()* present inside each of the class, the function *detect()* was called to detect the keypoints. Execution time was measured by checking how much time the function *detect()* took. IF..ELSE statement was used to select the keypoint detector.

### MP.3 Keypoint Removal

The keypoints outside the rectangle (region of interest - preceding car) was deleted with support of the function *contains()* inside the class *cv::Rect*. If the keypoint didnt exist inside the rectange, that corresponding kepoint was deleted and the iterator was updated with the next element. If the keypoint existed inside the rectagle, then the iterator was incremented to the next keypoint.

### MP.4 Keypoint Descriptors

In the file *matching2D_Student.cpp*, the function *descKeypoints()* was updated with the descriptor implementation for BRIEF, ORB, FREAK, AKAZE and SIFT. After creating  each of descriptor using the corresponding function *create()* present inside each of the class, the function *compute()* was called to compute the descriptors. Execution time was measured by checking how much time the function *compute()* took. IF..ELSE statement was used to select the descriptor.

### MP.5 Descriptor Matching

Descriptor Matching based on bruteforce or FLANN is implemented inside the function *matchDescriptors()*  in *matching2D_Student.cpp*.

If bruteforce matching is used, the distance function *NORM_HAMMING* was selected if the discriptor type was binary or else(for SIFT) the *NORM_L2* (Sum of Squared Differences - SSD) was selected.

For FLANN based matching, the function *cv::DescriptorMatcher::create(cv::DescriptorMatcher::FLANNBASED)* is called.

Depending on Bruteforce or FLANN, the matching was done based on Nearest Neighbour or K Nearest Neighbour(k=2).

### MP.6 Descriptor Distance Ratio

Inside the function *matchDescriptors()* in *matching2D_Student.cpp*, for K Nearest Neighbour, the distance ratio that were passing the threshold(0.8) was only selected.

### MP.7 - Number of detected Keypoints on the preceding vehicle for total of 10 images

| Detector Type | Average Keypoints |
|---------------|-------------------|
| SHITOMASI     | 117.9             |
| HARRIS        | 56                |
| FAST          | 149.1             |
| BRISK         | 276.2             |
| ORB           | 116.1             |
| AKAZE         | 167               |
| SIFT          | 138.6             |

### TASK MP.8 - Number of matched keypoints for all 10 images using all possible combinations of detectors and descriptors

| Detector Type | Descriptor Type | Matched Keypoints |
|---------------|-----------------|-------------------|
| HARRIS        | BRISK           | 43.6667           |
| HARRIS        | FREAK           | 44                |
| ORB           | FREAK           | 46.6667           |
| HARRIS        | ORB             | 50.1111           |
| HARRIS        | SIFT            | 51                |
| HARRIS        | BRIEF           | 51.1111           |
| ORB           | BRIEF           | 60.5556           |
| SIFT          | BRISK           | 65.7778           |
| SIFT          | FREAK           | 65.8889           |
| SIFT          | BRIEF           | 78                |
| ORB           | BRISK           | 83.4444           |
| ORB           | ORB             | 84.7778           |
| ORB           | SIFT            | 84.7778           |
| SHITOMASI     | BRISK           | 85.2222           |
| SHITOMASI     | FREAK           | 85.3333           |
| SIFT          | SIFT            | 88.8889           |
| FAST          | FREAK           | 97.5556           |
| FAST          | BRISK           | 99.8889           |
| SHITOMASI     | ORB             | 100.889           |
| SHITOMASI     | SIFT            | 103               |
| SHITOMASI     | BRIEF           | 104.889           |
| FAST          | SIFT            | 116.222           |
| FAST          | ORB             | 119               |
| FAST          | BRIEF           | 122.111           |
| AKAZE         | ORB             | 131.333           |
| AKAZE         | FREAK           | 131.889           |
| AKAZE         | BRISK           | 135               |
| AKAZE         | BRIEF           | 140.667           |
| AKAZE         | SIFT            | 141.111           |
| BRISK         | ORB             | 168.222           |
| BRISK         | FREAK           | 169.333           |
| BRISK         | BRISK           | 174.444           |
| BRISK         | SIFT            | 182.889           |
| BRISK         | BRIEF           | 189.333           |

### TASK MP.9 - Time it takes for keypoint detection and descriptor extraction

Below table was generated by adding several new columns to rank the algorithms
1) ***Average Kpt Rank*** and ***Matched Kpt Rank*** was created by sorting the column **Average Keypoints** and **Matched Keypoints** in descending order and giving the highest number of matched keypoints the rank 1. As we want the highest number of points, the column is sorted in descending order
2) ***Descriptor Speed Rank*** and ***Detector Speed Rank*** was created by sorting the column **Avg Descriptor t(ms)** and **Avg Detector t(ms)** in ascending order and giving the lowest time the rank 1. As we want the fastest execution, the column is sorted in ascending order
3) ***Speed Rank*** was created by adding **Descriptor Speed Rank** and **Detector Speed Rank**. The top rank in *Speed Rank* will give the fastest executing algorithm combination.
4) ***Keypoint Rank*** was created by adding **Average Kpt Rank** and **Matched Kpt Rank**. The top rank in *Keypoint Rank* will give the algorithm combination which can detect the highest number of points.
5) ***Total Rank*** was created by adding **Speed Rank** and **Keypoint Rank**. The top rank *Total Rank* will give the algorithm combination which can detect the highest number of points and also at the fastest speed.

Based on this, following conclusions were made
1) **FAST** detector and **BRIEF** descriptor is the best algorithm combination. It detected good amount of keypoints within the minimal time
2) **FAST** detector and **BRIEF** descriptor is also the fastest executing algorithm combination.
3) **BRISK** detector and **BRIEF** descriptor is the algorithm combination which detected the highest amount of keypoints on the vehicle and also matched them between successive frames.

| Detector Type | Descriptor Type | Average Keypoints | Average Kpt Rank | Matched Keypoints | Matched Kpt Rank | Avg Descriptor t(ms) | Descriptor Speed Rank | Avg Detector t(ms) | Detector Speed Rank | Speed Rank | Keypoint Rank | Total Rank |
|---------------|-----------------|-------------------|------------------|-------------------|------------------|----------------------|-----------------------|--------------------|---------------------|------------|---------------|------------|
| **FAST**      | **BRIEF**       | 149.1             | 11               | 122.111           | 11               | 0.620395             | 2                     | 1.17414            | 1                   | **3**      | 22            | **25**     |
| **BRISK**     | **BRIEF**       | 276.2             | 1                | 189.333           | 1                | 0.951298             | 7                     | 35.3391            | 22                  | 29         | **2**         | 31         |
| FAST          | ORB             | 149.1             | 12               | 119               | 12               | 1.12693              | 9                     | 1.18227            | 2                   | 11         | 24            | 35         |
| BRISK         | BRISK           | 276.2             | 3                | 174.444           | 3                | 3.07299              | 17                    | 37.2134            | 25                  | 42         | 6             | 48         |
| AKAZE         | BRIEF           | 167               | 7                | 140.667           | 7                | 0.867913             | 6                     | 59.7732            | 29                  | 35         | 14            | 49         |
| FAST          | BRISK           | 149.1             | 14               | 99.8889           | 17               | 1.95599              | 15                    | 1.23959            | 4                   | 19         | 31            | 50         |
| BRISK         | ORB             | 276.2             | 5                | 168.222           | 5                | 5.3645               | 19                    | 35.2182            | 21                  | 40         | 10            | 50         |
| BRISK         | SIFT            | 276.2             | 2                | 182.889           | 2                | 24.9663              | 25                    | 36.0973            | 24                  | 49         | 4             | 53         |
| FAST          | SIFT            | 149.1             | 13               | 116.222           | 13               | 16.9828              | 23                    | 1.28359            | 5                   | 28         | 26            | 54         |
| SHITOMASI     | BRIEF           | 117.9             | 20               | 104.889           | 14               | 0.669775             | 4                     | 11.723             | 16                  | 20         | 34            | 54         |
| BRISK         | FREAK           | 276.2             | 4                | 169.333           | 4                | 26.7675              | 27                    | 35.4772            | 23                  | 50         | 8             | 58         |
| AKAZE         | BRISK           | 167               | 8                | 135               | 8                | 2.36254              | 16                    | 57.6727            | 27                  | 43         | 16            | 59         |
| FAST          | FREAK           | 149.1             | 15               | 97.5556           | 18               | 25.9139              | 26                    | 1.20314            | 3                   | 29         | 33            | 62         |
| AKAZE         | SIFT            | 167               | 6                | 141.111           | 6                | 22.8612              | 24                    | 55.8837            | 26                  | 50         | 12            | 62         |
| ORB           | BRIEF           | 116.1             | 28               | 60.5556           | 28               | 0.621004             | 3                     | 8.49575            | 6                   | 9          | 56            | 65         |
| SHITOMASI     | ORB             | 117.9             | 22               | 100.889           | 16               | 1.31738              | 11                    | 11.7331            | 17                  | 28         | 38            | 66         |
| AKAZE         | ORB             | 167               | 10               | 131.333           | 10               | 3.9352               | 18                    | 61.4734            | 30                  | 48         | 20            | 68         |
| HARRIS        | BRIEF           | 56                | 30               | 51.1111           | 29               | 0.45996              | 1                     | 10.8992            | 10                  | 11         | 59            | 70         |
| ORB           | ORB             | 116.1             | 25               | 84.7778           | 22               | 6.20971              | 20                    | 8.56612            | 8                   | 28         | 47            | 75         |
| SHITOMASI     | SIFT            | 117.9             | 21               | 103               | 15               | 16.4505              | 22                    | 12.1083            | 18                  | 40         | 36            | 76         |
| SHITOMASI     | BRISK           | 117.9             | 24               | 85.2222           | 21               | 1.88528              | 14                    | 14.0916            | 19                  | 33         | 45            | 78         |
| AKAZE         | FREAK           | 167               | 9                | 131.889           | 9                | 32.92                | 33                    | 58.692             | 28                  | 61         | 18            | 79         |
| SIFT          | BRIEF           | 138.6             | 17               | 78                | 25               | 0.714301             | 5                     | 88.7002            | 32                  | 37         | 42            | 79         |
| ORB           | BRISK           | 116.1             | 27               | 83.4444           | 24               | 1.60481              | 12                    | 23.7704            | 20                  | 32         | 51            | 83         |
| HARRIS        | ORB             | 56                | 32               | 50.1111           | 31               | 1.22202              | 10                    | 10.9022            | 11                  | 21         | 63            | 84         |
| ORB           | SIFT            | 116.1             | 26               | 84.7778           | 23               | 30.5053              | 31                    | 8.54921            | 7                   | 38         | 49            | 87         |
| SHITOMASI     | FREAK           | 117.9             | 23               | 85.3333           | 20               | 30.2207              | 29                    | 11.6527            | 15                  | 44         | 43            | 87         |
| HARRIS        | BRISK           | 56                | 34               | 43.6667           | 34               | 0.956531             | 8                     | 10.9543            | 13                  | 21         | 68            | 89         |
| SIFT          | BRISK           | 138.6             | 19               | 65.7778           | 27               | 1.87043              | 13                    | 109.026            | 34                  | 47         | 46            | 93         |
| HARRIS        | SIFT            | 56                | 31               | 51                | 30               | 15.2316              | 21                    | 10.9229            | 12                  | 33         | 61            | 94         |
| ORB           | FREAK           | 116.1             | 29               | 46.6667           | 32               | 30.0147              | 28                    | 8.59295            | 9                   | 37         | 61            | 98         |
| SIFT          | SIFT            | 138.6             | 16               | 88.8889           | 19               | 81.331               | 34                    | 88.7116            | 33                  | 67         | 35            | 102        |
| SIFT          | FREAK           | 138.6             | 18               | 65.8889           | 26               | 30.2921              | 30                    | 86.4918            | 31                  | 61         | 44            | 105        |
| HARRIS        | FREAK           | 56                | 33               | 44                | 33               | 30.7433              | 32                    | 11.0697            | 14                  | 46         | 66            | 112        |

CSV to markdown was converted using [this website](https://thisdavej.com/copy-table-in-excel-and-paste-as-a-markdown-table/)
