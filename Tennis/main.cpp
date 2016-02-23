//
//  main.cpp
//  Tennis
//
//  Created by Sidharth Agarwal on 2/13/16.
//  Copyright © 2016 Sidharth Agarwal. All rights reserved.
//

#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "LineFilter.hpp"
#include "LineMapper.hpp"
#include "HoughToCanny.hpp"
#include "CSVwriter.hpp"
#include <thread>
const int imgHeight = 550;
const int imgWidth = 1392;

int main(int argc, const char * argv[]) {
    CvSize rawSize;
    rawSize.height = imgHeight;
    rawSize.width = imgWidth;
    
    uchar * buffer = new uchar[imgHeight * imgWidth];
    //load in the image data
    FILE *fp = fopen(argv[1], "rb");
    if (fp) {
        fread(buffer, imgWidth * imgHeight, 1, fp);
        fclose(fp);
    }
    
    //benchmarking my code, and improving it
    clock_t t1,t2;
    t1=clock();
    
    //cv::Mat src = cv::imread("field.jpg", 0);
    cv::Mat src(imgHeight, imgWidth,CV_8U,buffer);
    if(src.empty()) std::cout<<"empty image\n";
    cv::Mat dst, cdst;
    Canny(src, dst, 50, 200, 3);
    cvtColor(dst, cdst, CV_GRAY2BGR);
    
    std::vector<cv::Vec2f> lines;
    HoughLines(dst, lines, 1, CV_PI/180, 170, 0, 0 );
    if(lines.size()==0){
        printf("\nNo lines to be mapped\n");
        return 0;
    }
    //filter all lines from the noise and get one line for each actual line
    LineFilter filter = *new LineFilter();
    std::vector<cv::Vec2f> newLines = filter.filterLines(lines);
    lines.clear();
    
    //map each line to the field lines
    LineMapper map = *new LineMapper(newLines);
    
    //get Hough lines
    std::vector<cv::Vec4i> linesClose;
    cv::HoughLinesP(dst, linesClose, 1, CV_PI/180, 60, 30, 10);
    
    //map field lines with bounds using hugo lines.
    HoughToCanny mapper2 = *new HoughToCanny(linesClose, map.getCentreServiceLine(), map.getServiceLine(), map.getSinglesSideline1(), map.getSinglesSideline2(), map.getOutLine1(), map.getOutLine2(), map.getBaseLine());
    //cv::Mat cdst2;
    //Canny(src, cdst2, 50, 200, 3);
    {
        cv::Vec4i line2Map = mapper2.getCentreServiceLine();
        line( src, cv::Point(line2Map[0], line2Map[1]),
             cv::Point(line2Map[2], line2Map[3]), cvScalar(255,255,0), 3, 8 );
    }
    {
        cv::Vec4i line2Map = mapper2.getServiceLine();
        line( src, cv::Point(line2Map[0], line2Map[1]),
             cv::Point(line2Map[2], line2Map[3]), cvScalar(255,255,0), 3, 8 );
    }
    {
        cv::Vec4i line2Map = mapper2.getSinglesSideline1();
        line( src, cv::Point(line2Map[0], line2Map[1]),
             cv::Point(line2Map[2], line2Map[3]), cvScalar(255,255,0), 3, 8 );
    }
    {
        cv::Vec4i line2Map = mapper2.getSinglesSideline2();
        line( src, cv::Point(line2Map[0], line2Map[1]),
             cv::Point(line2Map[2], line2Map[3]), cvScalar(255,255,0), 3, 8 );
    }
    {
        cv::Vec4i line2Map = mapper2.getOutLine1();
        line( src, cv::Point(line2Map[0], line2Map[1]),
             cv::Point(line2Map[2], line2Map[3]), cvScalar(255,255,0), 3, 8 );
    }
    {
        cv::Vec4i line2Map = mapper2.getOutLine2();
        line( src, cv::Point(line2Map[0], line2Map[1]),
             cv::Point(line2Map[2], line2Map[3]), cvScalar(255,255,0), 3, 8 );
    }
    {
        cv::Vec4i line2Map = mapper2.getBaseLine();
        line( src, cv::Point(line2Map[0], line2Map[1]),
             cv::Point(line2Map[2], line2Map[3]), cvScalar(255,255,0), 3, 8 );
    }
    
    t2=clock();
    float diff ((float)t2-(float)t1);
    float secs = diff / CLOCKS_PER_SEC;
    printf("\nTime taken for mapping:%f", secs);
    
    //write each line to file
    CSVwriter *writer = new CSVwriter();
    cv::Vec4i curline = mapper2.getCentreServiceLine();
    writer->putCentreServiceLine(curline[0], curline[1], curline[2], curline[3]);
    curline = mapper2.getServiceLine();
    writer->putServiceLine(curline[0], curline[1], curline[2], curline[3]);
    curline = mapper2.getSinglesSideline1();
    writer->putSinglesSideline1(curline[0], curline[1], curline[2], curline[3]);
    curline = mapper2.getSinglesSideline2();
    writer->putSinglesSideline2(curline[0], curline[1], curline[2], curline[3]);
    curline = mapper2.getOutLine1();
    writer->putOutLine1(curline[0], curline[1], curline[2], curline[3]);
    curline = mapper2.getOutLine2();
    writer->putOutLine2(curline[0], curline[1], curline[2], curline[3]);
    curline = mapper2.getBaseLine();
    writer->putBaseLine(curline[0], curline[1], curline[2], curline[3]);
    writer->writeFile((char*)argv[2]);
    
    t2=clock();
    diff = ((float)t2-(float)t1);
    secs = diff / CLOCKS_PER_SEC;
    printf("\nTime taken for mapping+writing:%f", secs);
    imshow("Source", src);
    
    cv::waitKey();
    return 0;
}
