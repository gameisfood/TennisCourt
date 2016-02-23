//
//  HoughToCanny.hpp
//  Tennis
//
//  Created by Sidharth Agarwal on 2/14/16.
//  Copyright © 2016 Sidharth Agarwal. All rights reserved.
//

#ifndef HoughToCanny_hpp
#define HoughToCanny_hpp

#include <stdio.h>
#include <opencv2/highgui/highgui.hpp>

const float distanceTolaranceRatio = 0.15;
const int distanceTolarance = 5;
/*
 Takes list of all canny lines, and mapped lines.
 Maps them to give start and end point of each mapped lines.
 */
class HoughToCanny {
public:
    //const: takes in list of canny lines, and mapped lines
    //args: canny lines, centreServiceLine, serviceLine, singlesSideline1, singlesSideline2, outLine1, outLine2, baseLine
    HoughToCanny(std::vector<cv::Vec4i>, cv::Vec2f, cv::Vec2f, cv::Vec2f, cv::Vec2f, cv::Vec2f, cv::Vec2f, cv::Vec2f);
    cv::Vec4i getCentreServiceLine();
    cv::Vec4i getServiceLine();
    cv::Vec4i getSinglesSideline1();
    cv::Vec4i getSinglesSideline2();
    cv::Vec4i getOutLine1();
    cv::Vec4i getOutLine2();
    cv::Vec4i getBaseLine();
    
    ~HoughToCanny();
private:
    //returns the sart and end point of this particular line.
    //args: list of canny lines, and the line wanted
    cv::Vec4i getBounds (std::vector<cv::Vec4i>, cv::Vec2f);
    
    //Distance btween a line and a point
    //args: A,B,C,x,y
    float distanceBWLineAndPoint(float, float, float, float, float);
    
    //distance between two points
    //args: x1, y1, x2, y2
    float distanceBWPointAndPoint(float, float, float, float);
    
    //gets the length of the line
    //args: Vec4i(line represented by two points)
    float lineLength(cv::Vec4i);
    
    //internal data
    cv::Vec4i centreServiceLine;
    cv::Vec4i serviceLine;
    cv::Vec4i singlesSideline1;
    cv::Vec4i singlesSideline2;
    cv::Vec4i outLine1;
    cv::Vec4i outLine2;
    cv::Vec4i baseLine;
};

#endif /* HoughToCanny_hpp */
