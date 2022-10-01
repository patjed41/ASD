// Patryk Jędrzejczak

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

template<class T> using V = vector<T>;

V<pair<int, int> > sons;
V<int> deep;
V<int> deepest;
V<int> farthest;
V<int> far;
V<int> pre;
V<int> tree_size;
V<V<int> > jp;

int pre_count = 0;
int log_n = 1;

void dfs1(int v, int p) {
    int l = sons[v].first;
    int r = sons[v].second;

    deepest[v] = v;
    farthest[v] = v;
    pre[v] = pre_count++;
    tree_size[v] = 1;
    if (p != -1) {
        deep[v] = deep[p] + 1;
    }

    if (l > -1) {
        jp[l][0] = v;
        dfs1(l, v);
        tree_size[v] += tree_size[l];
    }
    if (r > -1) {
        jp[r][0] = v;
        dfs1(r, v);
        tree_size[v] += tree_size[r];
    }

    if (l > -1) {
        deepest[v] = deepest[l];
    }
    if (r > -1 && deep[deepest[r]] > deep[deepest[l]]) {
        deepest[v] = deepest[r];
    }
}

void dfs2(int v, int p) {
    int l = sons[v].first;
    int r = sons[v].second;

    if (p != -1) {
        farthest[v] = farthest[p];
        far[v] = far[p] + 1;
        if (v == sons[p].first && sons[p].second > -1 && deep[deepest[sons[p].second]] - deep[p] + 1 > far[v]) {
            farthest[v] = deepest[sons[p].second];
            far[v] = deep[deepest[sons[p].second]] - deep[p] + 1;
        }
        else if (v == sons[p].second && sons[p].first > -1 && deep[deepest[sons[p].first]] - deep[p] + 1 > far[v]) {
            farthest[v] = deepest[sons[p].first];
            far[v] = deep[deepest[sons[p].first]] - deep[p] + 1;
        }
    }

    if (l > -1) dfs2(l, v);
    if (r > -1) dfs2(r, v);
}

bool is_ancestor(int a, int b) {
    return !(pre[a] > pre[b] || pre[b] >= pre[a] + tree_size[a]);
}

int lca(int a, int b) {
    if (is_ancestor(a, b)) return a;
    if (is_ancestor(b, a)) return b;
    if (a == b) return a;

    int l = log_n - 1;
    while (l >= 0) {
        if (!is_ancestor(jp[a][l], b)) {
            a = jp[a][l];
        }
        l--;
    }
    return jp[a][0];
}

int jump(int a, int h) {
    int i = 0;
    int result = a;
    while (h > 0) {
        if (h & 1) result = jp[result][i];
        h >>= 1;
        i++;
    }
    return result;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    
    int n;
    cin >> n;

    sons.resize(n);
    deep.resize(n);
    deepest.resize(n);
    farthest.resize(n);
    far.resize(n);
    pre.resize(n);
    tree_size.resize(n);
    jp.resize(n);

    int power = 1;
    while (power < n) {
        power *= 2;
        log_n++;
    }
    for (int i = 0; i < n; i++) {
        jp[i].resize(log_n);
    }
    
    for (int i = 0; i < n; i++) {
        int a, b;
        cin >> a >> b;
        if (a > -1) a--;
        if (b > -1) b--;
        sons[i] = make_pair(a, b);
    }

    dfs1(0, -1);
    dfs2(0, -1);

    for (int j = 1; j < log_n; j++) {
        for (int i = 0; i < n; i++) {
            jp[i][j] = jp[jp[i][j - 1]][j - 1];
        }
    }

    int m;
    cin >> m;
    for (int i = 0; i < m; i++) {
        int a, d;
        cin >> a >> d;
        a--;
        if (deep[deepest[a]] - deep[a] >= d) {
            cout << jump(deepest[a], (deep[deepest[a]] - deep[a]) - d) + 1 << '\n';
        }
        else if (far[a] >= d) {
            int b = lca(a, farthest[a]);
            if (deep[a] - deep[b] >= d) {
                cout << jump(a, d) + 1 << '\n';
            }
            else {
                cout << jump(farthest[a], (deep[farthest[a]] - deep[b]) - (d - (deep[a] - deep[b]))) + 1 << '\n';
            }
        }
        else {
            cout << -1 << '\n';
        }
    }
}