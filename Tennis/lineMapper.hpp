//
//  LineMapper.hpp
//  Tennis
//
//  Created by Sidharth Agarwal on 2/13/16.
//  Copyright © 2016 Sidharth Agarwal. All rights reserved.
//

#ifndef LineMapper_hpp
#define LineMapper_hpp

#include <stdio.h>
#include <opencv2/highgui/highgui.hpp>
const float perpendicularTrigger = 0.75;
const float tolarence = 1.15;//always >1
const int reEvalThreshold = 5;
/*
 Takes in all hugo lines, and figures out which lines map to which lines on the field
 */
class LineMapper {
public:
    //takes in filtered lines
    LineMapper(std::vector<cv::Vec2f>);
    
    //getters for the relevent lines
    cv::Vec2f getBaseline();
    std::vector<cv::Vec2f> getHLines();
    std::vector<cv::Vec2f> getVLines();
    cv::Vec2f getCentreServiceLine();
    cv::Vec2f getServiceLine();
    cv::Vec2f getSinglesSideline1();
    cv::Vec2f getSinglesSideline2();
    cv::Vec2f getOutLine1();
    cv::Vec2f getOutLine2();
    cv::Vec2f getBaseLine();
    
    ~LineMapper();
private:
    //internal class data
    cv::Vec2f centreServiceLine;
    cv::Vec2f serviceLine;
    cv::Vec2f singlesSideline1;
    cv::Vec2f singlesSideline2;
    cv::Vec2f outLine1;
    cv::Vec2f outLine2;
    cv::Vec2f baseLine;
    std::vector<cv::Vec2f> Hlines; //horizontal lines, irrespective of prespective
    std::vector<cv::Vec2f> Vlines; //vertical lines, irrespective of prespective
    std::vector<cv::Vec2f> lines; //all lines
    
    //maps the lines to the correct names
    void mapVariousLines();
    
    //segregate lines to horizontal and vertical
    void segregateLines();
    
    //find the intersection point of two lines
    //returns wheather they intersect or not
    //args: rho1, theta1, rho2, theta2, &X, &Y
    int intersectionPoint(float, float, float, float, int&, int&);
    
    //given a line, and an intersection point, find the other line at intersection
    cv::Vec2f lineAtIntersectionPoint(std::vector<cv::Vec2f>, cv::Vec2f, int);
};

#endif /* LineMapper_hpp */
