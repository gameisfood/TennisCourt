//
//  LineFilter.cpp
//  Tennis
//
//  Created by Sidharth Agarwal on 2/13/16.
//  Copyright © 2016 Sidharth Agarwal. All rights reserved.
//

#include "LineFilter.hpp"
#include <set>


std::vector<cv::Vec2f> LineFilter::filterLines(std::vector<cv::Vec2f> originalLines) {
    std::vector<cv::Vec2f> newlines = *new std::vector<cv::Vec2f>;
    std::vector<cv::Vec2f> lines(originalLines);
    
    while (lines.size()>0) {
        cv::Vec2f curLine = lines[0];
        lines.erase(lines.begin()+0);
        std::vector<cv::Vec2f> thisCluster = getNeighboursRecursive(lines, curLine);
        thisCluster.push_back(curLine);
        //remove the lines in thisCluster, from all the lines
        for(size_t i = 0; i < thisCluster.size(); i++){
            int index = checkIfExists(lines, thisCluster[i]);
            if(index!=-1) lines.erase(lines.begin()+index);
        }
        //get mean of thisCluster
        double rho = 0.0, theta = 0.0;
        for(size_t i = 0; i < thisCluster.size(); i++){
            rho += thisCluster[i][0];
            theta += thisCluster[i][1];
        }
        cv::Vec2f avgLine;
        avgLine[0] = rho/thisCluster.size();
        avgLine[1] = theta/thisCluster.size();
        newlines.push_back(avgLine);
        thisCluster.clear();
    }
    return newlines;
}

//get all the neighbouring lines, to this particular line
std::vector<cv::Vec2f> LineFilter::regionQuery(std::vector<cv::Vec2f> allLines, cv::Vec2f line) {
    std::vector<cv::Vec2f> cluster = *new std::vector<cv::Vec2f>;
    for( size_t i = 0; i < allLines.size(); i++ ) {
        //printf("\nComparing %f_%f \t %f_%f", allLines[i][0], line[0], allLines[i][1], line[1]);
        if((allLines[i][0] <= line[0] + rhoBracket && allLines[i][0] >= line[0] - rhoBracket) && (allLines[i][1] <= line[1] + thetaBracket && allLines[i][1] >= line[1] - thetaBracket)) {
            cluster.push_back(allLines[i]);
        }
    }
    return cluster;
}

//gets all the neighbours recursively, effectively gatering the entire cluster
std::vector<cv::Vec2f> LineFilter::getNeighboursRecursive (std::vector<cv::Vec2f> tempList, cv::Vec2f line) {
    std::vector<cv::Vec2f> list(tempList);
    std::vector<cv::Vec2f> neighbours = *new std::vector<cv::Vec2f>;
    std::vector<cv::Vec2f> curNeighbours = regionQuery(list, line);
    for(size_t i = 0; i < curNeighbours.size(); i++){
        int index = checkIfExists(list, curNeighbours[i]);
        if(index!=-1){
            list.erase(list.begin()+index);
        }
    }
    for(size_t i = 0; i < curNeighbours.size(); i++){
        std::vector<cv::Vec2f> reNeighbours = getNeighboursRecursive(list, curNeighbours[i]);
        if(reNeighbours.size()>0){
            std::vector<cv::Vec2f> newNeighbours = *new std::vector<cv::Vec2f>;
            newNeighbours.reserve(neighbours.size() + reNeighbours.size());
            newNeighbours.insert(newNeighbours.end(), neighbours.begin(), neighbours.end());
            newNeighbours.insert(newNeighbours.end(), reNeighbours.begin(), reNeighbours.end());
            neighbours.clear();
            reNeighbours.clear();
            neighbours = newNeighbours;
        }
    }
    
    if(curNeighbours.size()>0){
        std::vector<cv::Vec2f> newNeighbours = *new std::vector<cv::Vec2f>;
        newNeighbours.reserve(neighbours.size() + curNeighbours.size());
        newNeighbours.insert(newNeighbours.end(), neighbours.begin(), neighbours.end());
        newNeighbours.insert(newNeighbours.end(), curNeighbours.begin(), curNeighbours.end());
        neighbours.clear();
        curNeighbours.clear();
        neighbours = newNeighbours;
    }
    
    list.clear();
    return neighbours;
}

//check if the list includes the given line
int LineFilter::checkIfExists (std::vector<cv::Vec2f> list, cv::Vec2f line) {
    for( size_t i = 0; i < list.size(); i++ ) {
        if(list[i][0]==line[0] && list[i][1]==line[1])
            return (int)i;
    }
    return -1;
}
