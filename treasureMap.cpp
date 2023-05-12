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
    RGBAPixel *pixel = im.getPixel(loc.first, loc.second);
    pixel->r = 2*(pixel->r/4);
    pixel->g = 2*(pixel->g/4);
    pixel->b = 2*(pixel->b/4);
}

void treasureMap::setLOB(PNG & im, pair<int,int> loc, int d){
    RGBAPixel *pixel = im.getPixel(loc.first, loc.second);
    
    int num = d%64;
    
    pixel->r = 4*(pixel->r/4) + num/16;
    pixel->g = 4*(pixel->g/4) + (num%16)/4;
    pixel->b = 4*(pixel->b/4) + num%4;

}

PNG treasureMap::renderMap(){
    PNG copy = base;
    vector<vector<bool>> visited;
    vector<vector<int>> pathLength;
    Queue<pair<int,int>> explore;
    
    visited.resize(base.height());
    pathLength.resize(base.height());
    for (unsigned int i = 0; i < base.height(); i++) {
        visited[i].resize(base.width());
        pathLength[i].resize(base.width());
    }

    int s_x = start.first;
    int s_y = start.second;

    visited[s_y][s_x] = true;
    pathLength[s_y][s_x] = 0;
    setLOB(copy, start, 0);
    explore.enqueue(start);

    while (!explore.isEmpty()) {
        pair<int,int> curr = explore.dequeue();
        vector<pair<int,int>> neigh;
        neigh = neighbors(curr);

        for (int i = 0; i < 4; i++) {
            pair<int,int> p = neigh[i];
            if (good(visited, curr, p)) {
                int x = p.first;
                int y = p.second;
                visited[y][x] = true;
                pathLength[y][x] = pathLength[curr.second][curr.first] + 1;
                setLOB(copy, p, pathLength[y][x]);
                explore.enqueue(p);
            }
        }
    }

    return copy;

}


PNG treasureMap::renderMaze(){
    PNG copy = base;
    vector<vector<bool>> visited;
    Queue<pair<int,int>> explore;
    
    visited.resize(base.height());
    for (unsigned int i = 0; i < base.height(); i++) {
        visited[i].resize(base.width());
    }

    int s_x = start.first;
    int s_y = start.second;

    visited[s_y][s_x] = true;
    setGrey(copy, start);
    explore.enqueue(start);

    while (!explore.isEmpty()) {
        pair<int,int> curr = explore.dequeue();
        vector<pair<int,int>> neigh;
        neigh = neighbors(curr);

        for (int i = 0; i < 4; i++) {
            pair<int,int> p = neigh[i];
            if (good(visited, curr, p)) {
                int x = p.first;
                int y = p.second;
                visited[y][x] = true;
                setGrey(copy, p);
                explore.enqueue(p);
            }
        }
    }

    for (int y = (start.second - 3); y <= start.second+3; y++) {
        for(int x = (start.first - 3); x <= start.first+3; x++) {
            if ( (x >= 0 && x < (int) base.width()) && (y >= 0 && y < (int) base.height()) ) {
                RGBAPixel *pixel = copy.getPixel(x, y);
                pixel->r = 255;
                pixel->g = 0;
                pixel->b = 0;
            }
        }
    }

    return copy;

}

bool treasureMap::good(vector<vector<bool>> & v, pair<int,int> curr, pair<int,int> next){
    
    if (next.first < 0 || next.first >= (int) base.width() || next.second < 0 || next.second >= (int) base.height()) {
        return false;
    } 

    if (v[next.second][next.first]) {
        return false;
    }

    RGBAPixel *curr_pixel = maze.getPixel(curr.first, curr.second);
    RGBAPixel *next_pixel = maze.getPixel(next.first, next.second);
    if (*curr_pixel != *next_pixel) {
        return false;
    }

    return true;
}

vector<pair<int,int>> treasureMap::neighbors(pair<int,int> curr) {
    vector<pair<int,int>> vertices;
    int x = curr.first;
    int y = curr.second;

    pair<int,int> left;
    left.first = x-1;
    left.second = y;
    pair<int,int> down;
    down.first = x;
    down.second = y+1;
    pair<int,int> right;
    right.first = x+1;
    right.second = y;
    pair<int,int> above;
    above.first = x;
    above.second = y-1;
    vertices.push_back(left);
    vertices.push_back(down);
    vertices.push_back(right);
    vertices.push_back(above);

    return vertices;

}

