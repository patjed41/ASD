// Patryk Jędrzejczak

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>

using namespace std;

template<class T> using V = vector<T>;

// Returns one of the shortest paths from 1 to n in graph.
V<int> find_shortest_path(V<V<int> >& graph, int n) {
    V<bool> visited(n + 1);
    V<int> came_from(n + 1, -1); // came_from[v] = u -> we reached u from v in bfs
    visited[1] = true;

    queue<int> bfs_queue;
    bfs_queue.push(1);

    while (!bfs_queue.empty()) {
        int vert = bfs_queue.front();
        bfs_queue.pop();

        for (int adj : graph[vert]) {
            if (!visited[adj]) {
                visited[adj] = true;
                came_from[adj] = vert;
                bfs_queue.push(adj);
            }
        }
    }

    V<int> path = {n};
    int current_vert = n;
    while (came_from[current_vert] != -1) {
        current_vert = came_from[current_vert];
        path.emplace_back(current_vert);
    }

    reverse(path.begin(), path.end());
    return path;
}

// Returns maximum possible energy that professor can have at the end of the journey.
// Saves numbers of powerbanks that professor should use in used_powerbanks argument.
// If professor can't complete the journey, returns -1.
int find_max_energy(V<int>& path, V<int>& used_powerbanks,
                    V<bool>& is_illegal, V<int>& e, int p, int k) {
    // possible_energy[i][enrg] == true -> professor can reach path[i] city with
    //   energy equal to enrg (possibly using powerbank in path[i] city)
    V<V<bool> > possible_energy(path.size(), V<bool>(p + 1));
    possible_energy[0][p] = true;

    for (int v = 0; v < int(path.size()) - 1; v++) {
        for (int energy = 0; energy <= p; energy++) {
            if (!possible_energy[v][energy]) {
                continue;
            }

            if (energy - k >= 0) {
                // powerbank in path[v + 1] city is not used
                possible_energy[v + 1][energy - k] = true;
            }
            if (energy - k >= 0 && energy - k + e[v + 1] <= p
                && !is_illegal[energy - k + e[v + 1]]) {
                // powerbank in path[v + 1] city is used
                possible_energy[v + 1][energy - k + e[v + 1]] = true;
            }
        }
    }

    int max_energy = -1;
    for (int energy = p; energy >= 0; energy--) {
        if (possible_energy[path.size() - 1][energy]) {
            max_energy = energy;
            break;
        }
    }

    if (max_energy != -1) {
        int current_energy = max_energy;
        for (int v = path.size() - 1; v > 0; v--) {
            if (current_energy + k <= p && possible_energy[v - 1][current_energy + k]) {
                current_energy += k;
            }
            else {
                used_powerbanks.emplace_back(path[v]);
                current_energy += k - e[v];
            }
        }
        reverse(used_powerbanks.begin(), used_powerbanks.end());
    }

    return max_energy;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int p, k, z;
    cin >> p >> k >> z;

    V<bool> is_illegal(p);
    for (int i = 0; i < z; i++) {
        int zw;
        cin >> zw;
        is_illegal[zw] = true;
    }

    int n, m;
    cin >> n >> m;

    V<V<int> > city_graph(n + 1, V<int>());
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        city_graph[a].emplace_back(b);
        city_graph[b].emplace_back(a);
    }

    V<int> e(n);
    for (int i = 0; i < n; i++) {
        cin >> e[i];
    }

    V<int> path = find_shortest_path(city_graph, n);

    if (path.size() == 1) { // vert n is unreachable
        cout << -1;
    }
    else {
        V<int> used_powerbanks;
        int max_energy = find_max_energy(path, used_powerbanks, is_illegal, e, p, k);

        if (max_energy == -1) { // impossible to reach n by the shortest path
            cout << -1;
        }
        else {
            cout << path.size() << ' ' << max_energy << ' ' << used_powerbanks.size() << endl;
            for (int v : path) {
                cout << v << ' ';
            }
            cout << endl;
            for (int v : used_powerbanks) {
                cout << v << ' ';
            }
        }
    }
}
