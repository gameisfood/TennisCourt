# TennisCourt
A test program that can map lines on a Tennis court from an image.
Developed on Xcode(C++11) and OpenCV-3.1.0.

# Classes
LineFilter: This class takes in all the Hough lines detected by openCV and, uses the DBSCAN algorithm to reduce noise and get only significant lines out of the many intersecting and close lines.

LineMapper: Taking the lines outputted by LineFilter, it looks at the lines and maps lines to the lines on fields, using various parameters such as orientation, number of intersections on a line, ratio of distance between lines. It gets rid of lines that cannot be mapped.

HugohToCanny: LineMapper outputs are taken, along with Canny lines from OpenCV. The Canny lines are bounded to the mapped lines, to get the start and end points of the lines.

CSVwriter: Once all the lines are found and mapped, they can be set in this class. Once all the lines are set, calling write(“filename.csv”) writes the data to the file. The output is a list of points along the lines. The point intervals are set at about 50 pixels apart, which can be changed in the header file for that class.


#Constants
imgHeight and imgWidth: in the main program defines the size of the image.

rhoBacket and thetaBracket: are the bracket limits which are used to find neighbours in LineFilter.

tolerance: how much error is allowed, in LineMapper.

distanceTolarance: in HoughToCanny, within how much distance is two lines the same.


#Performance
The program is capable of processing 30 frames/second on a single thread. However, multithreading is not implemented.

#Version 1.0

#Screenshots
![alt tag](http://modmygad.com/wp-content/uploads/2016/02/Screen-Shot-2016-02-14-at-10.02.42-AM.png)
![alt tag](http://modmygad.com/wp-content/uploads/2016/02/Screen-Shot-2016-02-23-at-10.37.17-PM.png)

## Copyright
Copyright 2016 Sidharth Agarwal
