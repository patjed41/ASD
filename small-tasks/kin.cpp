#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

#define L long long

const L MOD = (L)(1e9);

struct Tree {
    int TREE_SIZE;
    int LEAVES;
    vector<L> sum;

    Tree(int n) {
        int leaves = 1;
        while (leaves < n) {
            leaves *= 2;
        }
        TREE_SIZE = 2 * leaves;
        LEAVES = leaves;
        sum.resize(TREE_SIZE);
    }

    void insert(int pos, L value) {
        pos += LEAVES;

        while (pos > 0) {
            sum[pos] += value;
            pos /= 2;
        }
    }

    L get_sum(int l, int r) {
        if (l > r) {
            return 0;
        }

        l += LEAVES, r += LEAVES;
        L result = (l == r ? sum[l] : sum[l] + sum[r]);

        while (l / 2 != r / 2) {
            if (l % 2 == 0) {
                result += sum[l + 1];
            }
            if (r % 2 == 1) {
                result += sum[r - 1];
            }
            l /= 2, r /= 2;
        }

        return result % MOD;
    }
};

int main() {
    int n, k;
    cin >> n >> k;
    vector<L> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    vector<Tree> dp_tree(k, Tree(n + 1));
    for (int i = 0; i < n; i++) {
        dp_tree[0].insert(a[i], 1);
        for (int j = 1; j < k; j++) {
            dp_tree[j].insert(a[i], dp_tree[j - 1].get_sum(a[i] + 1, n) % MOD);
        }
    }

    cout << dp_tree[k - 1].get_sum(1, n);
}