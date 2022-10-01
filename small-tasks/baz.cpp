#include <iostream>
#include <vector>
#include <algorithm>

#define LL long long

using namespace std;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  int n;
  cin >> n;

  vector<LL> price(n);
  for (int i = 0; i < n; i++)
    cin >> price[n - i - 1];

  vector<LL> pref_price(n + 1);
  for (int i = 1; i <= n; i++)
    pref_price[i] = pref_price[i - 1] + price[i - 1];

  vector<LL> min_even_pref(n), min_odd_pref(n);
  if (price[0] % 2 == 0)
    min_even_pref[0] = price[0];
  else
    min_odd_pref[0] = price[0];
  for (int i = 1; i < n; i++) {
    if (price[i] % 2 == 0) {
      min_even_pref[i] = price[i];
      min_odd_pref[i] = min_odd_pref[i - 1];
    }
    else {
      min_odd_pref[i] = price[i];
      min_even_pref[i] = min_even_pref[i - 1];
    }
  }

  vector<LL> max_even_suf(n + 1), max_odd_suf(n + 1);
  if (price[n - 1] % 2 == 0)
    max_even_suf[0] = price[n - 1];
  else
    max_odd_suf[n - 1] = price[n - 1];
  for (int i = n - 2; i >= 0; i--) {
    if (price[i] % 2 == 0) {
      max_even_suf[i] = price[i];
      max_odd_suf[i] = max_odd_suf[i + 1];
    }
    else {
      max_odd_suf[i] = price[i];
      max_even_suf[i] = max_even_suf[i + 1];
    }
  }

  int m;
  cin >> m;
  for (int i = 0; i < m; i++) {
    int k;
    cin >> k;
    if (max_odd_suf[0] == 0 || (k == n && pref_price[k] % 2 == 0)) {
      cout << -1 << '\n';
    }
    else if (pref_price[k] % 2 == 1) {
      cout << pref_price[k] << '\n';
    }
    else {
      LL res = -1;
      if (min_odd_pref[k - 1] != 0 && max_even_suf[k] != 0) {
        res = pref_price[k] - min_odd_pref[k - 1] + max_even_suf[k];
      }
      if (min_even_pref[k - 1] != 0 && max_odd_suf[k] != 0) {
        res = max(res, pref_price[k] - min_even_pref[k - 1] + max_odd_suf[k]);
      }
      cout << res << '\n';
    }
  }
}
