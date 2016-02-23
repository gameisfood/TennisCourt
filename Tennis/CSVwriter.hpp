//
//  CSVwriter.hpp
//  Tennis
//
//  Created by Sidharth Agarwal on 2/14/16.
//  Copyright © 2016 Sidharth Agarwal. All rights reserved.
//

#ifndef CSVwriter_hpp
#define CSVwriter_hpp

#include <stdio.h>
#include <string.h>
#include <fstream>
const int interval = 50;
/*
 CSVwriter.
 writes all the relevant data to the csv
 */
class CSVwriter {
public:
    void writeFile(char*);
    void putCentreServiceLine(int, int, int, int);
    void putServiceLine(int, int, int, int);
    void putSinglesSideline1(int, int, int, int);
    void putSinglesSideline2(int, int, int, int);
    void putOutLine1(int, int, int, int);
    void putOutLine2(int, int, int, int);
    void putBaseLine(int, int, int, int);
private:
    int centreServiceLine[4];
    int serviceLine[4];
    int singlesSideline1[4];
    int singlesSideline2[4];
    int outLine1[4];
    int outLine2[4];
    int baseLine[4];
    std::ofstream myfile;
    bool pointNOnLine(int, int, int, int, int, int&, int&);
    void writeLinetoFile(char*, int*);
};
#endif /* CSVwriter_hpp */
