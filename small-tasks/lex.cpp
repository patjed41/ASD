// Patryk Jędrzejczak

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>

using namespace std;

template<class T> using V = vector<T>;
using L = long long;

L P = 1001, M = 1000696969;
V<L> pot, h;

L get_hash(int l, int r) {
    return (h[r] - h[l - 1] * pot[r - l + 1] + M * M) % M;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    cin >> n >> m;
    V<char> s(n + 1);
    pot.resize(n + 1);
    h.resize(n + 1);
    pot[0] = 1;
    for (int i = 1; i <= n; i++) {
        cin >> s[i];
        pot[i] = (pot[i - 1] * P) % M;
        h[i] = (h[i - 1] * P + s[i] - 96) % M;
    }

    for (int i = 0; i < m; i++) {
        int a, b, c, d;
        cin >> a >> b >> c >> d;

        if (get_hash(a, b) == get_hash(c, d)) {
            cout << "=\n";
            continue;
        }

        int l = 0;
        int r = min(b - a + 1, d - c + 1);
        while (l < r) {
            int s = (l + r) / 2;
            if (get_hash(a, a + s) == get_hash(c, c + s)) {
                l = s + 1;
            }
            else {
                r = s;
            }
        }

        if (r == min(b - a + 1, d - c + 1)) {
            cout << (b - a < d - c ? "<\n" : ">\n");
        }
        else {
            cout << (s[a + l] < s[c + l] ? "<\n" : ">\n");
        }
    }
}