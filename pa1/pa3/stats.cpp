
#include "stats.h"

stats::stats(PNG & im){

/* your code here! */
int w = im.width();
int h = im.height();

vector<double> rowHueX;
vector<double> rowHueY;
vector<double> rowSat;
vector<double> rowLum;
vector<vector<int>> rowHist;

vector<int> baseHist;
for (int b = 0; b < 36; b++) {
   baseHist.push_back(0); 
}

for (int a = 0; a < w; a++) {
    rowHueX.push_back(0);
    rowHueY.push_back(0);
    rowSat.push_back(0);
    rowLum.push_back(0);
    rowHist.push_back(baseHist);
}

for (int j = 0; j < h; j++) { 
    double rowCumSum_hueX = 0;
    double rowCumSum_hueY = 0;
    double rowCumSum_s = 0;
    double rowCumSum_l = 0;
    vector<int> cumHist = baseHist;

    for (int i = 0; i < w; i++) {
        HSLAPixel currPixel = *(im.getPixel(i,j));

        // cumulate hue
        double currHue = (currPixel.h / 360) * 2 * PI;
        double hueX = cos(currHue);
        double hueY = sin(currHue);       
        rowHueX[i] = rowHueX[i] + rowCumSum_hueX + hueX;
        rowCumSum_hueX = rowCumSum_hueX + hueX;
        rowHueY[i] = rowHueY[i] + rowCumSum_hueY + hueY;
        rowCumSum_hueY = rowCumSum_hueY + hueY;

        // cumulate sat
        rowSat[i] = rowSat[i] + rowCumSum_s + currPixel.s;
        rowCumSum_s = rowCumSum_s + currPixel.s;

        // cumulate lum
        rowLum[i] = rowLum[i] + rowCumSum_l + currPixel.l;
        rowCumSum_l = rowCumSum_l + currPixel.l;

        // cumulate hist
        int idx = floor(currPixel.h / 10);
        rowHist[i][idx] = rowHist[i][idx] + 1;
        int len = cumHist.size();
        for (int k = 0; k < len; k++) {
            rowHist[i][k] = rowHist[i][k] + cumHist[k];
        }
        cumHist[idx] = cumHist[idx] + 1;   
    }
    sumHueX.push_back(rowHueX);
    sumHueY.push_back(rowHueY);
    sumSat.push_back(rowSat);
    sumLum.push_back(rowLum);
    hist.push_back(rowHist);   
}

}

long stats::rectArea(pair<int,int> ul, pair<int,int> lr){

/* your code here */
return (lr.first - ul.first + 1) * (lr.second - ul.second + 1);

}

HSLAPixel stats::getAvg(pair<int,int> ul, pair<int,int> lr){

/* your code here */
int x1 = ul.first;
int y1 = ul.second;
int x2 = lr.first;
int y2 = lr.second;

long area = rectArea(ul,lr);
double avgS = 0.0;
double avgL = 0.0;
double avgHx = 0.0;
double avgHy = 0.0;

if (y1-1 < 0 && x1-1 < 0) {
    avgS = (sumSat[y2][x2] - 0 - 0 + 0) / area;
    avgL = (sumLum[y2][x2] - 0 - 0 + 0) / area;
    avgHx = (sumHueX[y2][x2] - 0 - 0 + 0) / area;
    avgHy = (sumHueY[y2][x2] - 0 - 0 + 0) / area;  
}
else if (y1-1 >= 0 && x1-1 < 0) {
    avgS = (sumSat[y2][x2] - sumSat[y1-1][x2] - 0 + 0) / area;
    avgL = (sumLum[y2][x2] - sumLum[y1-1][x2] - 0 + 0) / area;
    avgHx = (sumHueX[y2][x2] - sumHueX[y1-1][x2] - 0 + 0) / area;
    avgHy = (sumHueY[y2][x2] - sumHueY[y1-1][x2] - 0 + 0) / area;
}
else if (y1-1 < 0 && x1-1 >= 0)  {
    avgS = (sumSat[y2][x2] - 0 - sumSat[y2][x1-1] + 0) / area;
    avgL = (sumLum[y2][x2] - 0 - sumLum[y2][x1-1] + 0) / area;
    avgHx = (sumHueX[y2][x2] - 0 - sumHueX[y2][x1-1] + 0) / area;
    avgHy = (sumHueY[y2][x2] - 0 - sumHueY[y2][x1-1] + 0) / area;
    
} else {
    avgS = (sumSat[y2][x2] - sumSat[y1-1][x2] - sumSat[y2][x1-1] + sumSat[y1-1][x1-1]) / area;
    avgL = (sumLum[y2][x2] - sumLum[y1-1][x2] - sumLum[y2][x1-1] + sumLum[y1-1][x1-1]) / area;
    avgHx = (sumHueX[y2][x2] - sumHueX[y1-1][x2] - sumHueX[y2][x1-1] + sumHueX[y1-1][x1-1]) / area;
    avgHy = (sumHueY[y2][x2] - sumHueY[y1-1][x2] - sumHueY[y2][x1-1] + sumHueY[y1-1][x1-1]) / area;
}


double avgH;
double result = atan2(avgHy, avgHx) * 180 / PI;
if (result < 0) {
    avgH = 360 + result;
} else {
    avgH = result;
}
return HSLAPixel(avgH, avgS, avgL);

}

double stats::entropy(pair<int,int> ul, pair<int,int> lr){

    vector<int> distn;

    /* using private member hist, assemble the distribution over the
    *  given rectangle defined by points ul, and lr into variable distn.
    *  You will use distn to compute the entropy over the rectangle.
    *  if any bin in the distn has frequency 0, then do not add that 
    *  term to the entropy total. see .h file for more details.
    */

    /* my code includes the following lines:
        if (distn[i] > 0 ) 
            entropy += ((double) distn[i]/(double) area) 
                                    * log2((double) distn[i]/(double) area);
    */
    int x1 = ul.first;
    int y1 = ul.second;
    int x2 = lr.first;
    int y2 = lr.second;

    auto area = rectArea(ul,lr);
    double entropy = 0;
    for (int i = 0; i < 36; i++) {
        if (y1-1 < 0 && x1-1 < 0) {
            distn.push_back(hist[y2][x2][i]);
        }
        else if (y1-1 >= 0 && x1-1 < 0) {  
            distn.push_back(hist[y2][x2][i] - hist[y1-1][x2][i]);
        }
        else if (y1-1 < 0 && x1-1 >= 0) {
            distn.push_back(hist[y2][x2][i] - hist[y2][x1-1][i]);
        } else {
            distn.push_back(hist[y2][x2][i] - hist[y1-1][x2][i] - hist[y2][x1-1][i] + hist[y1-1][x1-1][i]);
        }
        
        if (distn[i] > 0) {
            double pi = (double)distn[i] / (double)area;
            entropy = entropy + pi * log2(pi);           
        } 
    }
    return  -1 * entropy;
}
