#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  string s;
  cin >> s;

  int n = s.length();
  
  pair<char, int> last1 = make_pair('*', -n), last2 = make_pair('*', -n);

  int smallest_dist = n;
  for (int i = 0; i < n; i++) {
    if (s[i] == '*')
      continue;

    if (s[i] == last1.first) {
      smallest_dist = min(smallest_dist, i - last2.second);
      last1.second = i;
    }
    else {
      smallest_dist = min(smallest_dist, i - last1.second);
      last2 = last1;
      last1 = make_pair(s[i], i);
    }
  }

  cout << n - smallest_dist + 1;
}
