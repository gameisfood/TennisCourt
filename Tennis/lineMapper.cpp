//
//  LineMapper.cpp
//  Tennis
//
//  Created by Sidharth Agarwal on 2/13/16.
//  Copyright © 2016 Sidharth Agarwal. All rights reserved.
//

#include "LineMapper.hpp"
cv::Vec2f base; //the line on which the entire prespective is based

LineMapper::LineMapper(std::vector<cv::Vec2f> tempLines) {
    centreServiceLine[0] = 0;
    centreServiceLine[1] = 0;
    serviceLine[0] = 0;
    serviceLine[1] = 0;
    singlesSideline1[0] = 0;
    singlesSideline1[1] = 0;
    singlesSideline2[0] = 0;
    singlesSideline2[1] = 0;
    outLine1[0] = 0;
    outLine1[1] = 0;
    outLine2[0] = 0;
    outLine2[1] = 0;
    baseLine[0] = 0;
    baseLine[1] = 0;
    
    lines = tempLines;
    base = lines[0];
    segregateLines();
    
    //TODO: error out if no lines on either axis.
    
    int noNeed;
    int hLineIntersections = 0, vLineIntersections = 0;
    cv::Vec2f hLine = Hlines[0];
    for( size_t i = 0; i < Vlines.size(); i++ ) {
        if(intersectionPoint(Vlines[i][0], Vlines[i][1], hLine[0], hLine[1],  noNeed,  noNeed))
            hLineIntersections ++;
    }
    cv::Vec2f vLine = Vlines[0];
    for( size_t i = 0; i < Hlines.size(); i++) {
        if(intersectionPoint(Hlines[i][0], Hlines[i][1], vLine[0], vLine[1], noNeed, noNeed))
            vLineIntersections ++;
    }
    if(vLineIntersections>hLineIntersections) {
        std::vector<cv::Vec2f> temp = Vlines;
        Vlines = Hlines;
        Hlines = Vlines;
    }
    mapVariousLines();
}
void LineMapper::mapVariousLines() {
    cv::Vec2f hLine = Hlines[0];
    std::vector<int> hIntersections;
    for( size_t i = 0; i < Vlines.size(); i++ ) {
        int pointX, pointY;
        if(intersectionPoint(Vlines[i][0], Vlines[i][1], hLine[0], hLine[1], pointX, pointY)) {
            if(hLine[1]>=0.75 && hLine[1]<=2.25)
                hIntersections.push_back(pointX);
            else
                hIntersections.push_back(pointY);
        }
    }
    std::sort(hIntersections.begin(), hIntersections.end());
    bool found = false;
    while(!found) {
        //look for 2 of the biggest consicutive gaps
        int p1=0,p2=0,p3=0;
        int sideline1=-1, sideline2=-1;
        bool sideline1Found=false, sideline2Found=false;
        float pAccuracy = 100.0;
        for( size_t i = 0; i < hIntersections.size() - 2; i++ ) {
            if((hIntersections[i+1]-hIntersections[i])*tolarence>=hIntersections[i+2]-hIntersections[i+1] || (hIntersections[i+1]-hIntersections[i])<=(hIntersections[i+2]-hIntersections[i+1])*tolarence) {
                float tempAccu;
                if(hIntersections[i+1]-hIntersections[i] > hIntersections[i+2]-hIntersections[i+1]){
                    tempAccu = (hIntersections[i+1]-hIntersections[i])/(hIntersections[i+2]-hIntersections[i+1]);
                }
                else tempAccu = (hIntersections[i+2]-hIntersections[i+1]) / (hIntersections[i+1]-hIntersections[i]);
                if(tempAccu<pAccuracy) {
                    p1 = (int)i;
                    p2 = (int)i+1;
                    p3 = (int)i+2;
                    pAccuracy = tempAccu;
                }
            }
        }
        singlesSideline1 = lineAtIntersectionPoint(Vlines, hLine, hIntersections[p1]);
        singlesSideline2 = lineAtIntersectionPoint(Vlines, hLine, hIntersections[p3]);
        centreServiceLine = lineAtIntersectionPoint(Vlines, hLine, hIntersections[p2]);
        //printf("\nps %d, %d, %d, \taccu %f", hIntersections[p1], hIntersections[p2], hIntersections[p3], pAccuracy);
        if(p1!=0) {
            for(int i=p1-1; i>0 && !sideline1Found; i--)
                if(((hIntersections[p1]-hIntersections[i])*3*tolarence >= hIntersections[p2]-hIntersections[p1]) && ((hIntersections[p1]-hIntersections[i])*3*(2-tolarence) <= hIntersections[p2]-hIntersections[p1]))
                {
                    //p1-1 is the singlesSideline1
                    sideline1Found=true;
                    sideline1 = i;
                    outLine1 = lineAtIntersectionPoint(Hlines, hLine, hIntersections[i]);
                }
        }
        if(p3<hIntersections.size()-1) {
            for(int i=p3+1; i<hIntersections.size() && !sideline2Found; i++)
                if(((hIntersections[i]-hIntersections[p3])*3*tolarence >= hIntersections[p3]-hIntersections[p2]) && ((hIntersections[i]-hIntersections[p3])*3*(2-tolarence) <= hIntersections[p3]-hIntersections[p2]))
                {
                    //p3+1 is the singlesSideline2
                    sideline2Found=true;
                    sideline2=i;
                    outLine2 = lineAtIntersectionPoint(Vlines, hLine, hIntersections[i]);
                }
        }
        found = true;
        //things are not found, but more lines are there, then this isnt the solution
        if(!sideline1Found && !sideline2Found && hIntersections.size()>reEvalThreshold && pAccuracy!=1.0){
            found = false;
        }
    }
    hIntersections.clear();
    cv::Vec2f vLine = Vlines[0];
    std::vector<int> vIntersections;
    for( size_t i = 0; i < Hlines.size(); i++ ) {
        int pointX, pointY;
        if(intersectionPoint(Hlines[i][0], Hlines[i][1], vLine[0], vLine[1], pointX, pointY)) {
            if(vLine[1]>=0.75 && vLine[1]<=2.25)
                vIntersections.push_back(pointX);
            else
                vIntersections.push_back(pointY);
        }
    }
    std::sort(vIntersections.begin(), vIntersections.end());
    baseLine = lineAtIntersectionPoint(Hlines, vLine, vIntersections[0]);
    serviceLine = lineAtIntersectionPoint(Hlines, vLine, vIntersections[1]);
    vIntersections.clear();
}
void LineMapper::segregateLines() {
    Hlines = *new std::vector<cv::Vec2f>;
    Vlines = *new std::vector<cv::Vec2f>;
    std::vector<cv::Vec2f> list(lines);
    for( size_t i = 0; i < list.size(); i++ ) { //change the theta range from [0,3] to [-1.5, 1.5], to make the calculations easier
        float baseTheta = base[1] - 1.5;
        float theta = list[i][1] - 1.5;
        if((theta>=(baseTheta - perpendicularTrigger) && theta<=(baseTheta + perpendicularTrigger)) ||
           (theta-3 >=(baseTheta - perpendicularTrigger) && theta-3 <=(baseTheta + perpendicularTrigger)) ||
           (theta>=(baseTheta-3 - perpendicularTrigger) && theta<=(baseTheta-3 + perpendicularTrigger)))
        Vlines.push_back(list[i]);
        else Hlines.push_back(list[i]);
    }
}
std::vector<cv::Vec2f> LineMapper::getHLines() {
    return Hlines;
}
std::vector<cv::Vec2f> LineMapper::getVLines() { //this one
    return Vlines;
}
int LineMapper::intersectionPoint(float r1, float t1, float r2, float t2, int &x, int &y) {
    float ct1=cosf(t1);     //matrix element a
    float st1=sinf(t1);     //b
    float ct2=cosf(t2);     //c
    float st2=sinf(t2);     //d
    float d=ct1*st2-st1*ct2;        //determinant
    if(d!=0.0f) {
        x=(int)((st2*r1-st1*r2)/d);
        y=(int)((-ct2*r1+ct1*r2)/d);
        return(1);
    } else { //parallel lines
        return(0);
    }
}
cv::Vec2f LineMapper::lineAtIntersectionPoint(std::vector<cv::Vec2f> list, cv::Vec2f line, int point) {
    for( size_t i = 0; i < list.size(); i++ ) {
        int x, y;
        if(intersectionPoint( line[0],  line[1],  list[i][0],  list[i][1],  x,  y)) {
            if(x==point || y==point)
                return list[i];
        }
    }
    printf("\nError: intersection point not found");
    return *new cv::Vec2f;
}

cv::Vec2f LineMapper::getCentreServiceLine() {
    return centreServiceLine;
}
cv::Vec2f LineMapper::getServiceLine() {
    return serviceLine;
}
cv::Vec2f LineMapper::getSinglesSideline1() {
    return singlesSideline1;
}
cv::Vec2f LineMapper::getSinglesSideline2() {
    return singlesSideline2;
}
cv::Vec2f LineMapper::getOutLine1() {
    return outLine1;
}
cv::Vec2f LineMapper::getOutLine2() {
    return outLine2;
}
cv::Vec2f LineMapper::getBaseLine() {
    return baseLine;
}

LineMapper::~LineMapper() {
    Hlines.clear();
    Vlines.clear();
    lines.clear();
}