//
//  LineFilter.hpp
//  Tennis
//
//  Created by Sidharth Agarwal on 2/13/16.
//  Copyright © 2016 Sidharth Agarwal. All rights reserved.
//  DBSCAN

#ifndef LineFilter_hpp
#define LineFilter_hpp

#include <stdio.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
/*
 Uses DBSCAN algorithm to reduce noise and group lines
 */
const float rhoBracket = 10;
const float thetaBracket = 0.1;
class LineFilter {
public:
    std::vector<cv::Vec2f> filterLines(std::vector<cv::Vec2f>);
private:
    //runs regionQuery on all neighbours recursively, to get the entire cluster
    std::vector<cv::Vec2f> getNeighboursRecursive (std::vector<cv::Vec2f>, cv::Vec2f);
    //gets the neighbours within the specifived bracket by the consts above
    std::vector<cv::Vec2f> regionQuery (std::vector<cv::Vec2f>, cv::Vec2f line);
    //if it exists, it returns the index, otherwise -1
    int checkIfExists (std::vector<cv::Vec2f>, cv::Vec2f);
    
    
};
#endif /* LineFilter_hpp */
