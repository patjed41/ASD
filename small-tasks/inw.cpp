// Patryk Jędrzejczak

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>

using namespace std;

template<class T> using V = vector<T>;

struct Frag {
    int l, r, maximum;

    Frag(int l, int r, int maximum) : l(l), r(r), maximum(maximum) {}
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    V<int> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    V<Frag> frags;
    for (int i = 0; i < n; i++) {
        frags.emplace_back(Frag(i, i, a[i]));
        while (frags.size() > 1 && a[i] < frags[frags.size() - 2].maximum) {
            frags[frags.size() - 2].r = i;
            frags[frags.size() - 2].maximum = max(frags[frags.size() - 2].maximum, frags.back().maximum);
            frags.pop_back();
        }
    }

    cout << frags.size() << '\n';

    for (Frag frag : frags) {
        cout << frag.r - frag.l + 1 << ' ';
        V<int> vertices = V<int>(a.begin() + frag.l, a.begin() + frag.r + 1);
        sort(vertices.begin(), vertices.end());
        for (int i = 0; i <= frag.r - frag.l; i++) {
            cout << vertices[i] << ' ';
        }
        cout << '\n';
    }
}