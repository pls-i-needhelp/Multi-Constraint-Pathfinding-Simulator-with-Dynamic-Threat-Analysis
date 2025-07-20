// ---------------------------------------------------------------
//  cs2_tactical_path.cpp  –  Basic tactical path-finding demo
//  -------------------------------------------------------------
//  Features
//   • Grid map with obstacles, cover, and bomb danger
//   • Danger radiates out from bombs (inverse-distance fade)
//   • A* search:  cost = 1 + danger*5  –  cover*0.4
//   • ASCII visualisation of map and chosen path
// ---------------------------------------------------------------

#include <bits/stdc++.h>
using namespace std;

/* ---------- Terrain types ------------------------------------ */
enum Terrain { OPEN, COVER, OBSTACLE, BOMB };

/* ---------- Cell definition ---------------------------------- */
struct Cell {
    Terrain type  = OPEN;
    double  danger = 0.0;   // 0-1 : higher means riskier
    double  cover  = 0.0;   // 0-1 : higher means safer
    int     x = 0, y = 0;
    Cell() = default;
    Cell(int _x,int _y):x(_x),y(_y) {}
};

/* ---------- Map class ---------------------------------------- */
class Map {
    int W, H;
public:
    vector<vector<Cell>> g;
    Map(int w,int h):W(w),H(h),g(h,vector<Cell>(w)) {
        for(int y=0;y<H;++y)
            for(int x=0;x<W;++x)
                g[y][x] = Cell(x,y);
    }
    bool inside(int x,int y) const { return 0<=x && x<W && 0<=y && y<H; }

    /* --- editing helpers ------------------------------------ */
    void addCover(int x,int y,double val=0.8){
        if(inside(x,y)){ g[y][x].type=COVER; g[y][x].cover=val; }
    }
    void addObstacle(int x,int y){
        if(inside(x,y)) g[y][x].type=OBSTACLE;
    }
    void addBomb(int x,int y,int r=3){
        if(!inside(x,y)) return;
        g[y][x].type=BOMB;
        for(int dx=-r; dx<=r; ++dx)
            for(int dy=-r; dy<=r; ++dy){
                int nx=x+dx, ny=y+dy;
                if(!inside(nx,ny)) continue;
                double dist = hypot(dx,dy);
                if(dist<=r){
                    double d = 1.0 - dist/r;                // fades to 0 at radius edge
                    g[ny][nx].danger = max(g[ny][nx].danger,d);
                }
            }
    }

    /* --- ASCII print ---------------------------------------- */
    void draw(set<pair<int,int>> path={}, 
              pair<int,int> S={-1,-1}, pair<int,int> G={-1,-1}) const{
        cout << '\n';
        for(int y=H-1; y>=0; --y){
            for(int x=0; x<W; ++x){
                pair<int,int> p{x,y};
                if(p==S)               { cout<<"S "; continue; }
                if(p==G)               { cout<<"G "; continue; }
                if(path.count(p))      { cout<<"* "; continue; }

                const Cell& c = g[y][x];
                switch(c.type){
                    case OBSTACLE: cout<<"X "; break;
                    case COVER:    cout<<"# "; break;
                    case BOMB:     cout<<"B "; break;
                    default:
                        if(c.danger>0.7)      cout<<"! ";
                        else if(c.danger>0.3) cout<<"o ";
                        else                  cout<<". ";
                }
            }
            cout << '\n';
        }
    }
};

/* ---------- A* search --------------------------------------- */
struct Node{
    int x, y;
    double g, f;                          // g = cost so far, f = g + h
    bool operator<(const Node& o) const { return f>o.f; } // for min-heap
};

vector<pair<int,int>> reconstruct(
        unordered_map<long long,pair<int,int>>& parent,
        pair<int,int> cur){
    vector<pair<int,int>> path;
    while(parent.count(((long long)cur.first<<32)|cur.second)){
        path.push_back(cur);
        cur = parent[((long long)cur.first<<32)|cur.second];
    }
    reverse(path.begin(),path.end());
    return path;
}

vector<pair<int,int>> aStar(Map& m, pair<int,int> S, pair<int,int> G){
    auto key = [](int x,int y){ return ((long long)x<<32)|y; };

    priority_queue<Node> pq;
    unordered_map<long long,double> best;
    unordered_map<long long,pair<int,int>> parent;

    auto h = [&](int x,int y){ return abs(x-G.first)+abs(y-G.second); };

    pq.push({S.first,S.second,0.0,(double)h(S.first,S.second)});
    best[key(S.first,S.second)] = 0.0;

    int dir[4][2]={{1,0},{-1,0},{0,1},{0,-1}};

    while(!pq.empty()){
        Node cur = pq.top(); pq.pop();
        if(make_pair(cur.x,cur.y)==G)
            return reconstruct(parent,G);

        for(auto& d:dir){
            int nx=cur.x+d[0], ny=cur.y+d[1];
            if(!m.inside(nx,ny)) continue;
            const Cell& c = m.g[ny][nx];
            if(c.type==OBSTACLE) continue;

            double moveCost = 1.0 + c.danger*5.0 - c.cover*0.4;
            double ng = cur.g + moveCost;
            auto k = key(nx,ny);
            if(!best.count(k) || ng<best[k]){
                best[k]=ng;
                parent[k]={cur.x,cur.y};
                pq.push({nx,ny,ng,ng+h(nx,ny)});
            }
        }
    }
    return {};      // no path
}

/* ---------- Demo main --------------------------------------- */
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    Map map(15,10);

    /* Place cover */
    map.addCover(3,3); map.addCover(3,4);
    map.addCover(7,6); map.addCover(7,7);
    map.addCover(11,2); map.addCover(11,3);

    /* Walls */
    for(int y=2; y<=7; ++y) map.addObstacle(2,y);

    /* Bombs */
    map.addBomb(8,5,3);
    map.addBomb(12,7,3);
    map.addBomb(12,5,6);

    pair<int,int> start{1,1};
    pair<int,int> goal {10,8};

    cout << "=== MAP BEFORE SEARCH ===\n";
    map.draw({},start,goal);

    auto path = aStar(map,start,goal);
    if(path.empty()){
        cout << "\nNo path found.\n";
        return 0;
    }

    set<pair<int,int>> P(path.begin(),path.end());
    cout << "\n=== MAP WITH PATH ===\n";
    map.draw(P,start,goal);

    /* Stats */
    double dangerSum=0;
    for(auto &p:path) dangerSum += map.g[p.second][p.first].danger;
    cout << "\nPath length : " << path.size()
         << "\nDanger sum  : " << fixed << setprecision(2) << dangerSum << '\n';
    return 0;
}
