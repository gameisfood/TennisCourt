//
//  CSVwriter.cpp
//  Tennis
//
//  Created by Sidharth Agarwal on 2/14/16.
//  Copyright © 2016 Sidharth Agarwal. All rights reserved.
//

#include "CSVwriter.hpp"
#include <math.h>
#include <thread>

void CSVwriter::writeFile(char* fileName) {
    
    myfile.open (fileName);
    
    char name[] = "Service Line";
    writeLinetoFile(name, serviceLine);
    char name1[] = "Centre Service Line";
    writeLinetoFile(name1, centreServiceLine);
    char name2[] = "Single Side Line";
    writeLinetoFile(name2, singlesSideline1);
    writeLinetoFile(name2, singlesSideline2);
    char name4[] = "Side Line";
    writeLinetoFile(name4, outLine1);
    writeLinetoFile(name4, outLine2);
    char name6[] = "Base Line";
    writeLinetoFile(name6, baseLine);
    
    myfile.close();
}

void CSVwriter::writeLinetoFile(char* name, int* curline) {
    myfile<<name<<",";
    int n = 0;
    int X, Y;
    
    //curline = serviceLine;
    if(curline[0]==0 && curline[1]==0 && curline[2]==0 && curline[3]==0) {
        myfile<<"N/A\n";
    }
    else {
        while(pointNOnLine(curline[0], curline[1], curline[2], curline[3], n, X, Y)) {
            myfile<<"("<<X<<" "<<Y<<")"<<",";
            n++;
        }
        myfile<<"("<<curline[2]<<" "<<curline[3]<<")"<<"\n";
    }
}

bool CSVwriter::pointNOnLine(int x1, int y1, int x2,int y2, int n, int& X, int& Y) {
    float length = sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
    float pieces = length/interval;
    //printf("\nlength:%f \tPieces:%f \tn%d", length, pieces, n);
    if(n>pieces)
        return false;
    float dx = x2-x1;
    float dy = y2-y1;
    X = x1+(dx/pieces)*n;
    Y = y1+(dy/pieces)*n;
    
    return true;
}

void CSVwriter::putCentreServiceLine(int x1, int y1, int x2, int y2) {
    centreServiceLine[0] = x1;
    centreServiceLine[1] = y1;
    centreServiceLine[2] = x2;
    centreServiceLine[3] = y2;
}
void CSVwriter::putServiceLine(int x1, int y1, int x2, int y2){
    serviceLine[0] = x1;
    serviceLine[1] = y1;
    serviceLine[2] = x2;
    serviceLine[3] = y2;
}
void CSVwriter::putSinglesSideline1(int x1, int y1, int x2, int y2) {
    singlesSideline1[0] = x1;
    singlesSideline1[1] = y1;
    singlesSideline1[2] = x2;
    singlesSideline1[3] = y2;
}
void CSVwriter::putSinglesSideline2(int x1, int y1, int x2, int y2) {
    singlesSideline2[0] = x1;
    singlesSideline2[1] = y1;
    singlesSideline2[2] = x2;
    singlesSideline2[3] = y2;
}
void CSVwriter::putOutLine1(int x1, int y1, int x2, int y2) {
    outLine1[0] = x1;
    outLine1[1] = y1;
    outLine1[2] = x2;
    outLine1[3] = y2;
}
void CSVwriter::putOutLine2(int x1, int y1, int x2, int y2) {
    outLine2[0] = x1;
    outLine2[1] = y1;
    outLine2[2] = x2;
    outLine2[3] = y2;
}
void CSVwriter::putBaseLine(int x1, int y1, int x2, int y2) {
    baseLine[0] = x1;
    baseLine[1] = y1;
    baseLine[2] = x2;
    baseLine[3] = y2;
}