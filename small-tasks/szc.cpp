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

map<string, string> change;

string get_string(const string& s, int pos) {
    string res = "";
    res.push_back(s[pos - 2]);
    res.push_back(s[pos - 1]);
    res.push_back(s[pos]);
    return res;
}

int new_pairs(const string& s, int pos, int swap_pos) {
    char c = s[pos];
    if (pos <= swap_pos) {
        c = change[get_string(s, swap_pos)][2 - swap_pos + pos];
    }
    if (c != 'G') {
        return 0;
    }

    char c1 = 'A';
    if (pos > 0) {
        c1 = s[pos - 1];
    }
    if (pos == swap_pos - 1) {
        c1 = change[get_string(s, swap_pos)][0];
    }
    else if (pos == swap_pos) {
        c1 = change[get_string(s, swap_pos)][1];
    }
    else if (pos == swap_pos + 1) {
        c1 = change[get_string(s, swap_pos)][2];
    }

    char c2 = 'A';
    if (pos > 1) {
        c2 = s[pos - 2];
    } 
    if (pos == swap_pos) {
        c2 = change[get_string(s, swap_pos)][0];
    }
    else if (pos == swap_pos + 1) {
        c2 = change[get_string(s, swap_pos)][1];
    }
    else if (pos == swap_pos + 2) {
        c2 = change[get_string(s, swap_pos)][2];
    }

    char c3 = 'A';
    if (pos > 2) {
        c3 = s[pos - 3];
    }
    if (pos == swap_pos + 1) {
        c3 = change[get_string(s, swap_pos)][0];
    }
    else if (pos == swap_pos + 2) {
        c3 = change[get_string(s, swap_pos)][1];
    }
    else if (pos == swap_pos + 3) {
        c3 = change[get_string(s, swap_pos)][2];
    }

    return (c1 == 'G') + (c2 == 'G') + (c3 == 'G');
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    int n, m;
    string s;
    cin >> n >> m >> s;

    for (int i = 0; i < m; i++) {
        string x, y;
        cin >> x >> y;
        change[x] = y;
    }

    int inf = 10000000;
    V<V<int>> dp(n, V<int>(4, inf));
    for (int i = 0; i < n; i++) {
        dp[i][3] = 0;
    }
    if (n > 1 && s[0] == 'G' && s[1] == 'G') {
        dp[1][3] = 1;
    }
    for (int i = 2; i < n; i++) {
        if (change.find(get_string(s, i)) != change.end()) {
            dp[i][0] = (i > 2 ? dp[i - 3][3] : 0) + new_pairs(s, i, i) + new_pairs(s, i - 1, i) + new_pairs(s, i - 2, i);
            string old_s = get_string(s, i);
            string new_s = change[old_s];
            s[i] = new_s[2], s[i - 1] = new_s[1], s[i - 2] = new_s[0]; 
            if (i > 2) {
                for (int j = 0; j < 3; j++) {
                    if (dp[i - 3][j] < inf) {
                        dp[i][0] = min(dp[i][0], dp[i - 3][j]
                            + new_pairs(s, i, i - 3 - j) + new_pairs(s, i - 1, i - 3 - j) + new_pairs(s, i - 2, i - 3 - j));
                    }
                }
            }
            s[i] = old_s[2], s[i - 1] = old_s[1], s[i - 2] = old_s[0];
        }
        if (dp[i - 1][0] < inf) {
           dp[i][1] = dp[i - 1][0] + new_pairs(s, i, i - 1); 
        }
        if (dp[i - 1][1] < inf) {
            dp[i][2] = dp[i - 1][1] + new_pairs(s, i, i - 2);
        }
        dp[i][3] = dp[i - 1][3] + new_pairs(s, i, -10);
        if (dp[i - 1][2] < inf) {
            dp[i][3] = min(dp[i][3], dp[i - 1][2] + new_pairs(s, i, i - 3));
        }
    }

    /*for (int i = 0; i < n; i++) {
        cout << dp[i][1] << ' ';
    }
    cout << '\n';*/

    int res = inf;
    for (int i = 0; i < 4; i++) {
        res = min(res, dp[n - 1][i]);
    }

    cout << res; 
}
