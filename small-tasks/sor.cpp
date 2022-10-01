#include <iostream>
#include <vector>

using namespace std;

using VI = vector<int>;

int main() {
    int n, m = int(1e9);
    cin >> n;

    VI a(n);
    for (int i = 0; i < n; i++)
        cin >> a[i];

    if (n == 1) {
        cout << 1;
        return 0;
    }

    vector<vector<VI>> dp(n, vector<VI> (n, VI(2)));

    for (int i = 0; i < n; i++)
        dp[i][i][0] = dp[i][i][1] = 1;

    for (int d = 1; d < n; d++) {
        for (int i = 0; i < n - d; i++) {
            if (a[i] < a[i + 1])
                dp[i][i + d][0] += dp[i + 1][i + d][0];
            if (a[i] < a[i + d] && d > 1)
                dp[i][i + d][0] += dp[i + 1][i + d][1];
            if (a[i + d] > a[i])
                dp[i][i + d][1] += dp[i][i + d - 1][0];
            if (a[i + d] > a[i + d - 1] && d > 1)
                dp[i][i + d][1] += dp[i][i + d - 1][1];
            dp[i][i + d][0] %= m;
            dp[i][i + d][1] %= m;
        }
    }

    cout << (dp[0][n - 1][0] + dp[0][n - 1][1]) % m;
}
