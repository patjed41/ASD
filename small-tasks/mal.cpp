#include <iostream>
#include <vector>

using namespace std;

struct Tree {
    int TREE_SIZE;
    int LEAVES = 1;
    vector<int> sum;
    vector<int> lazy; // -1 -> no lazy

    Tree(int n) {
        while (LEAVES < n) {
            LEAVES *= 2;
        }
        TREE_SIZE = 2 * LEAVES;

        sum.resize(TREE_SIZE);
        lazy.resize(TREE_SIZE, -1);
    }

    void push(int v, int d) {
        if (lazy[v] != -1) {
            lazy[2 * v] = lazy[v];
            lazy[2 * v + 1] = lazy[v];
            sum[2 * v] = d / 2 * lazy[v];
            sum[2 * v + 1] = d / 2 * lazy[v];
            lazy[v] = -1;
        }
    }

    void set_value(int v, int l, int r, int a, int b, int val) {
        if (l == a && r == b) {
            lazy[v] = val;
            sum[v] = val * (r - l + 1);
        }
        else if (b >= a) {
            push(v, r - l + 1);

            int mid = (l + r) / 2;
            set_value(2 * v, l, mid, a, min(b, mid), val);
            set_value(2 * v + 1, mid + 1, r, max(a, mid + 1), b, val);

            sum[v] = sum[2 * v] + sum[2 * v + 1];
        }
    }

    int get_sum() {
        return sum[1];
    }
};

int main() {
    int n, m;
    cin >> n >> m;
    Tree tree(n + 1);

    for (int i = 0; i < m; i++) {
        int a, b;
        char event;
        cin >> a >> b >> event;

        if (event == 'C') {
            tree.set_value(1, 0, tree.LEAVES - 1, a, b, 0);
        }
        else if (event == 'B') {
            tree.set_value(1, 0, tree.LEAVES - 1, a, b, 1);
        }
        cout << tree.get_sum() << '\n';
    }
}