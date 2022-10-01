#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

template<class T> using V = vector<T>;

V<V<int>> graph;
V<int> p;
V<int> max_p;
V<int> post;
V<bool> vis;
int post_count = 0;

void dfs(int v) {
  vis[v] = true;
  for (int u : graph[v]) {
    if (!vis[u]) {
      dfs(u);
    }
  }
  post[post_count] = v;
  post_count++;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  int n, m, k;
  cin >> n >> m >> k;

  graph.resize(n);
  p.resize(n);
  max_p.resize(n);
  post.resize(n);
  vis.resize(n);

  for (int i = 0; i < n; i++) {
    cin >> p[i];
    max_p[i] = p[i];
  }

  for (int i = 0; i < m; i++) {
    int a, b;
    cin >> a >> b;
    graph[b - 1].emplace_back(a - 1);
  }

  for (int v = 0; v < n; v++) {
    if (!vis[v]) {
      dfs(v);
    }
  }

  reverse(post.begin(), post.end());
  
  for (int i = 0; i < n; i++) {
    for (int u : graph[post[i]]) {
      max_p[u] = max(max_p[u], max_p[post[i]]);
    }
  }

  sort(max_p.begin(), max_p.end());

  if (k == 0) {
    cout << 0;
  }
  else {
    cout << max_p[k - 1];
  }
}