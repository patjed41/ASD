#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    int n, m;
    cin >> n >> m;

    vector<long long> players(n);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            long long player;
            cin >> player;
            if (j < n / 2) {
                players[player - 1] |= 1LL << i;;
            }
        }
    }

    sort(players.begin(), players.end());

    for (int i = 0; i < n - 1; i++) {
        if (players[i] == players[i + 1]) {
            cout << "NIE";
            return 0;
        }
    }

    cout << "TAK";
}