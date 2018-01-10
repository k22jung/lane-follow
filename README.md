# Lane-Following

**Lane-Following** is a program for detecting lane markers on a road to calculate the alignment of the road with a vehicle. The tools and algorithms used are as follows:

* Color Filters
* Gaussian Blur
* Canny Edge Detector
* Selecting a Region of Interest
* Hough Transform

More details of how these components work together may be found in the commented source file. Some fine-tuning is still required to account for all test cases.

<p align="center"> 
<img src="https://github.com/k22jung/lane-follow/blob/master/samples/sample_yellow.png">
</p>

The videos are from [Udacity's Lane Finding Project for Self-Driving Car ND](https://github.com/udacity/CarND-LaneLines-P1).
 

## Dependencies

The program was ran and created for Ubuntu 16.04
- [OpenCV 3.2.0](http://opencv.org/releases.html)

You must build OpenCV with extra modules, following these [instructions](https://github.com/opencv/opencv_contrib). The latest release for the extra modules can be found [here](https://github.com/opencv/opencv_contrib/releases).

## Running

This is an Eclipse project, you may simply compile it.





