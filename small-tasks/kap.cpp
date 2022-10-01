// Patryk Jędrzejczak

#include <iostream>
#include <vector>
#include <set>
#include <unordered_map>
#include <algorithm>

using namespace std;

template<class T> using V = vector<T>;
using L = long long;

struct Island {
    int x, y, id;

    Island() {}
    Island(int x, int y, int id) : x(x), y(y), id(id) {}
};

bool cmp_x(Island is1, Island is2) {
    return is1.x < is2.x;
}

bool cmp_y(Island is1, Island is2) {
    return is1.y < is2.y;
}

V<Island> isles; 
V<V<pair<int, int>>> g;
V<L> dist;
L INF = 100000000000000000LL;

void dijkstra(int start) {
    dist[start] = 0;
    set<pair<L, int>> labels = {{0, start}};
    while (!labels.empty()) {
        int v = labels.begin()->second;
        labels.erase(labels.begin());
        for (auto &edge : g[v]) {
            int u = edge.first;
            L d = (L)edge.second;
            if (dist[u] > dist[v] + d) {
                auto it = labels.find(make_pair(dist[u], u));
                if (it != labels.end()) {
                    labels.erase(it);
                }
                dist[u] = dist[v] + d;
                labels.insert(make_pair(dist[u], u));
            }
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    g.resize(n);
    dist.resize(n, INF);

    for (int i = 0; i < n; i++) {
        int x, y;
        cin >> x >> y;
        isles.emplace_back(Island(x, y, i));
    }

    sort(isles.begin(), isles.end(), cmp_x);
    for (int i = 0; i < n - 1; i++) {
        g[isles[i].id].emplace_back(make_pair(isles[i + 1].id, isles[i + 1].x - isles[i].x));
        g[isles[i + 1].id].emplace_back(make_pair(isles[i].id, isles[i + 1].x - isles[i].x));
    }

    sort(isles.begin(), isles.end(), cmp_y);
    for (int i = 0; i < n - 1; i++) {
        g[isles[i].id].emplace_back(make_pair(isles[i + 1].id, isles[i + 1].y - isles[i].y));
        g[isles[i + 1].id].emplace_back(make_pair(isles[i].id, isles[i + 1].y - isles[i].y));
    }

    dijkstra(0);
    
    cout << dist[n - 1];
}
