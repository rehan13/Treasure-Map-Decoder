#include "treasureMap.h"
#include "queue.h"
using namespace std;

treasureMap::treasureMap(const PNG & baseim, const PNG & mazeim, pair<int,int> s)
{
/* YOUR CODE HERE */
    base = baseim;
    maze = mazeim;
    start = s;
}

void treasureMap::setGrey(PNG & im, pair<int,int> loc){
    /* YOUR CODE HERE */
    RGBAPixel* pixel = im.getPixel(loc.first, loc.second);

    if (pixel == NULL) {
        return;
    }

    pixel->r = 2 * (pixel->r / 4);
    pixel->g = 2 * (pixel->g / 4);
    pixel->b = 2 * (pixel->b / 4);

}

void treasureMap::setLOB(PNG & im, pair<int,int> loc, int d) {
    /* YOUR CODE HERE */
    RGBAPixel* pixel = im.getPixel(loc.first, loc.second);

    if (pixel == NULL) {
        return;
    }
    
    
    pixel->r = ((int) ((d & 0b110000) >> 4)) | (pixel->r & 0b11111100);
    pixel->g = ((int) ((d & 0b001100) >> 2)) | (pixel->g & 0b11111100);
    pixel->b = ((int) ((d & 0b000011))) | (pixel->b & 0b11111100);

}


PNG treasureMap::renderMap(){
    /* YOUR CODE HERE */

    // 0
    PNG copyBase = PNG(base);

    // 1
    vector<vector<bool>> visitedLocation(copyBase.width(), vector<bool> (copyBase.height(), false));
    vector<vector<int>> pathLength(copyBase.width(), vector<int> (copyBase.height(), 0));

    // 2
    Queue<pair<int,int>> exploreLoc;

    // 3
    visitedLocation[start.first][start.second] = true;
    pathLength[start.first][start.second] = 0;
    int dStart = 0;
    setLOB(copyBase, start, dStart);
    exploreLoc.enqueue(start);

    // 4
    while (!exploreLoc.isEmpty()) {
        pair<int,int> curr = exploreLoc.dequeue();

        vector<pair<int,int>> neighboursCurr = neighbors(curr);

        for (pair<int,int> p : neighboursCurr) {
            
            if (good(visitedLocation, curr, p)) {
                visitedLocation[p.first][p.second] = true;
                pathLength[p.first][p.second] = pathLength[curr.first][curr.second] + 1;
                int dPath = pathLength[p.first][p.second];
                setLOB(copyBase, p, dPath);
                exploreLoc.enqueue(p);
            }
        }
    }

    return copyBase;
}


PNG treasureMap::renderMaze(){
    /* YOUR CODE HERE */
    PNG copyBase = PNG(base);
    pair<int,int> startingPoint = start;


    int col = startingPoint.first;
    int row = startingPoint.second;

    // // Darkening of maze
    vector<vector<bool>> visitedLocation(copyBase.width(), vector<bool> (copyBase.height(), false));
    Queue<pair<int,int>> exploreLoc;

    visitedLocation[start.first][start.second] = true;
    exploreLoc.enqueue(start);

    while (!exploreLoc.isEmpty()) {
        pair<int,int> curr = exploreLoc.dequeue();

        vector<pair<int,int>> neighboursCurr = neighbors(curr);

        for (pair<int,int> p : neighboursCurr) {
            
            if (good(visitedLocation, curr, p)) {
                visitedLocation[p.first][p.second] = true;
                setGrey(copyBase, p);
                exploreLoc.enqueue(p);
            }
        }
    }

    // Red starting point 7px by 7px
    int rowL = row - 3;
    int rowR = row + 3;
    int colU = col - 3;
    int colD = col + 3;
    
    for (int i = colU; i <= colD; i++) {
        for (int j = rowL; j <= rowR; j++) {
            if ((i >= 0) && (j >= 0) && (i < copyBase.width()) && (j < copyBase.height())) { 
                RGBAPixel* pixel = copyBase.getPixel(i, j);
                pixel->r = 255;
                pixel->g = 0;
                pixel->b = 0;
            }
        }
    }

    return copyBase;
}

bool treasureMap::notWhite(RGBAPixel* pixel) {

    RGBAPixel* original = maze.getPixel(start.first, start.second);

    return (*original == *pixel);
}

bool treasureMap::good(vector<vector<bool>> & v, pair<int,int> curr, pair<int,int> next){
    /* YOUR CODE HERE */

    int nextCol = next.first;
    int nextRow = next.second;

    if ((nextCol >= 0) && (nextRow >= 0) && (nextCol < (int) maze.width()) && (nextRow < (int) maze.height())) {
        if (!v[nextCol][nextRow]) {

            RGBAPixel* currPixel = maze.getPixel(curr.first, curr.second);
            RGBAPixel* nextPixel = maze.getPixel(next.first, next.second);

            if (*currPixel == *nextPixel) {
                return true;
            }
        }
    }

    return false;

}

vector<pair<int,int>> treasureMap::neighbors(pair<int,int> curr) {
    /* YOUR CODE HERE */
    vector<pair<int,int>> locations = vector<pair<int,int>>();

    int col = curr.first;
    int row = curr.second;

    pair<int,int> left = make_pair((col - 1), row);
    pair<int,int> below = make_pair(col, (row + 1));
    pair<int,int> right = make_pair((col + 1), row);
    pair<int,int> above = make_pair(col, (row - 1));

    locations.push_back(left);
    locations.push_back(below);
    locations.push_back(right);
    locations.push_back(above);

    return locations;

}

