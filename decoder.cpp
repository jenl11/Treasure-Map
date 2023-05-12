#include "decoder.h"
#include "queue.h"
#include "stack.h"

using namespace std;

decoder::decoder(const PNG & tm, pair<int,int> s)
   :start(s),mapImg(tm) {
    PNG copy = mapImg;
    Stack<pair<int,int>> solPoints;
      
    vector<pair<int,int>> list;
    vector<vector<bool>> v;
    vector<vector<int>> d;
    vector<vector<pair<int,int>>> predecessor; 
    Queue<pair<int,int>> explore;

    v.resize(copy.height());
    d.resize(copy.height());
    predecessor.resize(copy.height());
    for (unsigned int i = 0; i < copy.height(); i++) {
        v[i].resize(copy.width());
        d[i].resize(copy.width());
        predecessor[i].resize(copy.width());
    }

    int s_x = s.first;
    int s_y = s.second;

    v[s_y][s_x] = true;
    d[s_y][s_x] = 0;
    explore.enqueue(s);

    while (!explore.isEmpty()) {
        pair<int,int> curr = explore.dequeue();
        vector<pair<int,int>> neigh;
        neigh = neighbors(curr);

        for (int i = 0; i < 4; i++) {
            pair<int,int> p = neigh[i];
            if (good(v, d, curr, p)) {
                int x = p.first;
                int y = p.second;
                v[y][x] = true;
                d[y][x] = d[curr.second][curr.first] + 1;
                predecessor[y][x] = curr;
                list.push_back(p);
                explore.enqueue(p);
            }
        }
    } 


    pair<int, int> max = list[0];
    for (int i = 1; i < list.size(); i++) {
        pair<int, int> check = list[i];
        if (d[max.second][max.first] <= d[check.second][check.first]) {
            max.first = check.first;
            max.second = check.second;
        }
    }

    Queue<pair<int,int>> path;
    solPoints.push(max);
    path.enqueue(max);

    while(!path.isEmpty()) {
        pair<int,int> a = path.dequeue();
        if (a == s) {
            break;
        }
        vector<pair<int,int>> n = neighbors(a);

        for (int i = 0; i < 4; i++) {
            pair<int,int> b = n[i];
            if ((b.first >= 0 && b.first < (int) copy.width() && b.second >= 0 && b.second < (int) copy.height()) && v[b.second][b.first]) {
                if (predecessor[a.second][a.first] == b) {
                    solPoints.push(b);
                    path.enqueue(b);
                }
            }
        }

    }
    pathPts.push_back(solPoints.pop());

    while(!solPoints.isEmpty()) {
        pathPts.push_back(solPoints.pop());
    }

}

PNG decoder::renderSolution(){
   PNG copy = mapImg;
   for (int i = 0; i < pathLength(); i++) {
      pair<int,int> path = pathPts[i];
      RGBAPixel *pixel = copy.getPixel(path.first, path.second);
      pixel->r = 255;
      pixel->g = 0;
      pixel->b = 0;
   }
   return copy;
}

PNG decoder::renderMaze(){
    PNG copy = mapImg;
    vector<vector<bool>> v;
    vector<vector<int>> d;
    Queue<pair<int,int>> explore;
    
    v.resize(copy.height());
    d.resize(copy.height());
    for (unsigned int i = 0; i < copy.height(); i++) {
        v[i].resize(copy.width());
        d[i].resize(copy.width());
    }

    int s_x = start.first;
    int s_y = start.second;

    v[s_y][s_x] = true;
    d[s_y][s_x] = 0;
    setGrey(copy, start);
    explore.enqueue(start);

    while (!explore.isEmpty()) {
        pair<int,int> curr = explore.dequeue();
        vector<pair<int,int>> neigh = neighbors(curr);

        for (int i = 0; i < 4; i++) {
            pair<int,int> p = neigh[i];
            if (good(v, d, curr, p)) {
                int x = p.first;
                int y = p.second;
                v[y][x] = true;
                d[y][x] = d[curr.second][curr.first] + 1;
                setGrey(copy, p);
                explore.enqueue(p);
            }
        }
    }

    for (int y = (start.second - 3); y <= (start.second + 3); y++) {
        for(int x = (start.first - 3); x <= (start.first + 3); x++) {
            if ( (x >= 0 && x < (int) copy.width()) && (y >= 0 && y < (int) copy.height()) ) {
                RGBAPixel *pixel = copy.getPixel(x, y);
                pixel->r = 255;
                pixel->g = 0;
                pixel->b = 0;
                pixel->a = 1;

            }
        }
    }

    return copy;
}

void decoder::setGrey(PNG & im, pair<int,int> loc){
    RGBAPixel *pixel = im.getPixel(loc.first, loc.second);
    pixel->r = 2*(pixel->r/4);
    pixel->g = 2*(pixel->g/4);
    pixel->b = 2*(pixel->b/4);
}

pair<int,int> decoder::findSpot(){
   return pathPts[pathLength()-1];
}

int decoder::pathLength(){
   return pathPts.size();
}

bool decoder::good(vector<vector<bool>> & v, vector<vector<int>> & d, pair<int,int> curr, pair<int,int> next){
    if (next.first < 0 || next.first >= (int) mapImg.width() || next.second < 0 || next.second >= (int) mapImg.height()) {
        return false;
    } 

    if (v[next.second][next.first]) {
        return false;
    }

   RGBAPixel *pixel = mapImg.getPixel(next.first, next.second);
   if (!compare(*pixel, d[curr.second][curr.first])) {
      return false;
   }

   return true;
}

vector<pair<int,int>> decoder::neighbors(pair<int,int> curr) {
    vector<pair<int,int>> vertices;
    int x = curr.first;
    int y = curr.second;

    pair<int,int> left;
    left.first = x-1;
    left.second = y;
    pair<int,int> below;
    below.first = x;
    below.second = y+1;
    pair<int,int> right;
    right.first = x+1;
    right.second = y;
    pair<int,int> above;
    above.first = x;
    above.second = y-1;

    vertices.push_back(left);
    vertices.push_back(below);
    vertices.push_back(right);
    vertices.push_back(above);

    return vertices;

}


bool decoder::compare(RGBAPixel p, int d){
   int r = (p.r % 4) * 16;
   int g = (p.g % 4) * 4;
   int b = p.b % 4;
   int compare = r + g + b;
   if (compare == ((d + 1) % 64)) {
      return true;
   }
   return false;
   
}
