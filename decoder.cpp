#include "decoder.h"
#include "queue.h"
#include "stack.h"
using namespace std;

decoder::decoder(const PNG & tm, pair<int,int> s)
   :start(s),mapImg(tm) {

   /* YOUR CODE HERE */
   start = s;

   // pathPts.push_back(s);
    
   PNG copyBase = PNG(mapImg);
   
   vector<vector<bool>> visitedLocation(copyBase.width(), vector<bool> (copyBase.height(), false));
   vector<vector<int>> shortDistance(copyBase.width(), vector<int> (copyBase.height(), 0));
   pair<int,int> negLocNotS;
   negLocNotS.first = -5;
   negLocNotS.second = -5;
   
   vector<vector<pair<int,int>>> prevLocation(copyBase.width(), vector<pair<int,int>> (copyBase.height(), negLocNotS));

   Queue<pair<int,int>> exploreLoc;

   visitedLocation[start.first][start.second] = true;
   shortDistance[start.first][start.second] = 0;
   pair<int,int> negLoc;
   negLoc.first = -1;
   negLoc.second = -1;
   prevLocation[start.first][start.second] = negLoc;
   exploreLoc.enqueue(start);

   pair<int,int> treasurePoint;

   while (!exploreLoc.isEmpty()) {
      pair<int,int> curr = exploreLoc.dequeue();

      vector<pair<int,int>> neighboursCurr = neighbors(curr);

      for (pair<int,int> p : neighboursCurr) {

         if (good(visitedLocation, shortDistance, curr, p)) {
            visitedLocation[p.first][p.second] = true;
            shortDistance[p.first][p.second] = shortDistance[curr.first][curr.second] + 1;
            prevLocation[p.first][p.second] = curr;
            exploreLoc.enqueue(p);
         }
      }

      if (exploreLoc.isEmpty()) {
         treasurePoint = curr;
      }
   }

   // setting the path to treasure
   vector<pair<int,int>> revPath;
   revPath.push_back(treasurePoint);

   pair<int,int> needToPush = treasurePoint;

   
   while(((needToPush.first >= 0) && (needToPush.second >= 0) && (needToPush.first < copyBase.width()) && (needToPush.second <  copyBase.height()))) {
      revPath.push_back(prevLocation[needToPush.first][needToPush.second]);
      needToPush = prevLocation[needToPush.first][needToPush.second];
   }



   while (!revPath.empty()) {

      pathPts.push_back(revPath[revPath.size() - 1]);
      revPath.pop_back();
   }
   pathPts[0] = start;


   mapImg = PNG(copyBase);
}

PNG decoder::renderSolution(){

   /* YOUR CODE HERE */
   PNG copyImg = PNG(mapImg);
   for (pair<int,int> point : pathPts) {
      RGBAPixel* p = copyImg.getPixel(point.first, point.second);
      p->r = 255;
      p->g = 0;
      p->b = 0;
   }

   return copyImg;

}

PNG decoder::renderMaze(){

   /* YOUR CODE HERE */

   PNG copyBase = PNG(mapImg);
   pair<int,int> startingPoint = start;


   int col = startingPoint.first;
   int row = startingPoint.second;

   // // Darkening of maze
   vector<vector<bool>> visitedLocation(copyBase.width(), vector<bool> (copyBase.height(), false));
   vector<vector<int>> shortDistance(copyBase.width(), vector<int> (copyBase.height(), 0));

   Queue<pair<int,int>> exploreLoc;

   visitedLocation[start.first][start.second] = true;
   exploreLoc.enqueue(start);
   
   
   while (!exploreLoc.isEmpty()) {
      pair<int,int> curr = exploreLoc.dequeue();

      vector<pair<int,int>> neighboursCurr = neighbors(curr);

      for (pair<int,int> p : neighboursCurr) {
         if (good(visitedLocation, shortDistance, curr, p)) {
            int d = shortDistance[curr.first][curr.second] + 1;
            shortDistance[p.first][p.second] = d;
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

void decoder::setGrey(PNG & im, pair<int,int> loc){

   /* YOUR CODE HERE */
   RGBAPixel* pixel = im.getPixel(loc.first, loc.second);

   if (pixel == NULL) {
      return;
   }

   pixel->r = 2 * (pixel->r / 4);
   pixel->g = 2 * (pixel->g / 4);
   pixel->b = 2 * (pixel->b / 4);


}

pair<int,int> decoder::findSpot(){

   /* YOUR CODE HERE */
   return pathPts[pathPts.size() - 1];

}

int decoder::pathLength(){

   /* YOUR CODE HERE */
   return pathPts.size() - 1;

}

bool decoder::good(vector<vector<bool>> & v, vector<vector<int>> & d, pair<int,int> curr, pair<int,int> next){

   /* YOUR CODE HERE */
   int nextCol = next.first;
   int nextRow = next.second;
   
   if ((nextCol >= 0) && (nextRow >= 0) && (nextCol < (int) mapImg.width()) && (nextRow < (int) mapImg.height())) {
      
      if (!v[nextCol][nextRow]) {

         RGBAPixel* currPixel = mapImg.getPixel(curr.first, curr.second);
         RGBAPixel* nextPixel = mapImg.getPixel(nextCol, nextRow);
         
         
         if (compare(*nextPixel, d[curr.first][curr.second])) {
            return true;
         }
      }
   }

   return false;

}

vector<pair<int,int>> decoder::neighbors(pair<int,int> curr) {

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


bool decoder::compare(RGBAPixel p, int d){

   /* YOUR CODE HERE */
   int red = p.r;
   int green = p.g;
   int blue = p.b;

   int d1 = (((red % 4) * 16) + ((green % 4) * 4) + (blue % 4));

   int d2 = ((d + 1) % 64);

   return (d2 == d1);

}
