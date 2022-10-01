#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define L long long
#define MP make_pair
#define REP(i, n) for(int i = 0; i < n; ++i)
#define REPR(i, n) for(int i = n - 1; i >= 0; --i)
#define FOR(i, a, b) for(int i = a; i < b; ++i)
#define FORR(i, a, b) for(int i = b - 1; i >= a; --i)
#define EB emplace_back
#define ST first
#define ND second
#define S size
#define RS resize

template<class T> using V = vector<T>;

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    int n, m;
    cin >> n >> m;

    set<int> s1, s2;
    s1.insert(0);
    s2.insert(n + 1);

    for (int i = 0; i < m; i++) {
        char c;
        int a, b;
        cin >> c >> a >> b;
        if (c == '+') {
            if (a == 1) {
                s1.insert(b);
            }
            else {
                s2.insert(a);
            }
        }
        else {
            if (a == 1) {
                s1.erase(b);
            }
            else {
                s2.erase(a);
            }
        }
        cout << max(0, *(s2.begin()) - *(prev(s1.end())) - 1) << '\n';
    }
}
