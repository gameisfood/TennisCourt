//
//  HoughToCanny.cpp
//  Tennis
//
//  Created by Sidharth Agarwal on 2/14/16.
//  Copyright © 2016 Sidharth Agarwal. All rights reserved.
//

#include "HoughToCanny.hpp"

HoughToCanny::HoughToCanny(std::vector<cv::Vec4i> cannyLines, cv::Vec2f centreServiceLine, cv::Vec2f serviceLine, cv::Vec2f singlesSideline1, cv::Vec2f singlesSideline2, cv::Vec2f outLine1, cv::Vec2f outLine2, cv::Vec2f baseLine) {
    
    //get the bouds of each mapped line using hough lines
    HoughToCanny::centreServiceLine = getBounds(cannyLines, centreServiceLine);
    HoughToCanny::serviceLine = getBounds(cannyLines, serviceLine);
    HoughToCanny::singlesSideline1 = getBounds(cannyLines, singlesSideline1);
    HoughToCanny::singlesSideline2 = getBounds(cannyLines, singlesSideline2);
    HoughToCanny::outLine1 = getBounds(cannyLines, outLine1);
    HoughToCanny::outLine2 = getBounds(cannyLines, outLine2);
    HoughToCanny::baseLine = getBounds(cannyLines, baseLine);
    //if service line and base line were confused swap them
    if(lineLength(HoughToCanny::serviceLine)>lineLength(HoughToCanny::baseLine)) {
        cv::Vec4i temp = HoughToCanny::serviceLine;
        HoughToCanny::serviceLine = HoughToCanny::baseLine;
        HoughToCanny::baseLine = temp;
    }
    cannyLines.clear();
}

cv::Vec4i HoughToCanny::getBounds (std::vector<cv::Vec4i> list, cv::Vec2f line) {
    if(line[0]==0 && line[1]==0){
        cv::Vec4i point;
        for(int i=0;i<4;i++) point[i]=0.0;
        return point;
    }
    //extreme points
    int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
    
    float A, B, C;//equation of the line
    A = cos(line[1]);
    B = sin(line[1]);
    C = line[0];
    for( size_t i = 0; i < list.size(); i++ ) {
        cv::Vec4i line = list[i];
        float distance1 = distanceBWLineAndPoint(A, B, C, line[0], line[1]);
        float distance2 = distanceBWLineAndPoint(A, B, C, line[2], line[3]);
        if(distance1<distanceTolarance && distance2<distanceTolarance){
            //printf("in service line bound");
            float a1, b1, a2, b2;
            if(distanceBWPointAndPoint(0, 0, line[0], line[1])<distanceBWPointAndPoint(0, 0, line[2], line[3])) {
                a1 = line[0]; b1 = line[1]; a2 = line[2]; b2 = line[3];
            }
            else {
                a1 = line[2]; b1 = line[3]; a2 = line[0]; b2 = line[1];
            }
            if(x1 == 0 && y1==0 && x2==0 && y2==0){
                x1=a1;
                y1=b1;
                x2=a2;
                y2=b2;
            }
            else {
                if(distanceBWPointAndPoint(x2, y2, a1, b1) > distanceBWPointAndPoint(x2, y2, x1, y1)){
                    x1 = a1; y1 = b1;
                }
                if(distanceBWPointAndPoint(x1, y1, a2, b2) > distanceBWPointAndPoint(x2, y2, x1, y1)){
                    x2 = a2; y2 = b2;
                }
            }
        }
    }
    
    cv::Vec4i point;
    point[0] = x1;
    point[1] = y1;
    point[2] = x2;
    point[3] = y2;
    return point;
}

float HoughToCanny::distanceBWLineAndPoint(float A, float B, float C, float m, float n) {
    C=-C;
    float up = (A*m + B*n + C);
    if (up<0) up*=-1;
    float down = sqrtf(A*A + B*B);
    return up/down;
}

float HoughToCanny::lineLength(cv::Vec4i line) {
    return sqrt((line[0]-line[2])*(line[0]-line[2]) + (line[1]-line[3])*(line[1]-line[3]));
}

float HoughToCanny::distanceBWPointAndPoint(float x1, float y1, float x2, float y2) {
    return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}

cv::Vec4i HoughToCanny::getCentreServiceLine() {
    return centreServiceLine;
}
cv::Vec4i HoughToCanny::getServiceLine() {
    return serviceLine;
}
cv::Vec4i HoughToCanny::getSinglesSideline1() {
    return singlesSideline1;
}
cv::Vec4i HoughToCanny::getSinglesSideline2() {
    return singlesSideline2;
}
cv::Vec4i HoughToCanny::getOutLine1() {
    return outLine1;
}
cv::Vec4i HoughToCanny::getOutLine2() {
    return outLine2;
}
cv::Vec4i HoughToCanny::getBaseLine() {
    return baseLine;
}

HoughToCanny::~HoughToCanny() {
    
}