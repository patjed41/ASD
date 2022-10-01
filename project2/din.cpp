// Patryk Jędrzejczak

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

template<class T> using V = vector<T>;

struct Node {
  int prior; // random priority
  char letter;
  int subtree_size;
  int max_frag; // length of maximal fragment of same letters
  int max_prefix; // length of maximal prefix of same letters
  int max_suffix; // length of maximal suffix of same letters
  char prefix_letter; // leftmost letter in subtree
  char suffix_letter; // rightmost letter in subtree
  bool is_reversed; // this will work like lazy propagation
  Node* left;
  Node* right;

  Node(int prior, char letter) : prior(prior), letter(letter) {
    subtree_size = 1;
    max_frag = max_prefix = max_suffix = 1;
    prefix_letter = suffix_letter = letter;
    is_reversed = false;
    left = right = NULL;
  }
};

int get_subtree_size(Node* node) {
  return node == NULL ? 0 : node->subtree_size;
}

int get_max_frag(Node* node) {
  return node == NULL ? 0 : node->max_frag;
}

int get_max_prefix(Node* node) {
  return node == NULL ? 0 : node->max_prefix;
}

int get_max_suffix(Node* node) {
  return node == NULL ? 0 : node->max_suffix;
}

char get_prefix_letter(Node* node) {
  return node == NULL ? '@' : node->prefix_letter;
}

char get_suffix_letter(Node* node) {
  return node == NULL ? '@' : node->suffix_letter;
}

// Node update based on sons.
void update(Node* node) {
  if (node != NULL) {
    Node* l = node->left;
    Node* r = node->right;

    node->subtree_size = 1 + get_subtree_size(l) + get_subtree_size(r);

    node->max_frag = max(get_max_frag(l), get_max_frag(r));
    if (get_suffix_letter(l) == node->letter) {
      node->max_frag = max(node->max_frag, get_max_suffix(l) + 1);
      if (get_prefix_letter(r) == node->letter) {
        node->max_frag = max(node->max_frag, get_max_suffix(l) + get_max_prefix(r) + 1);
      }
    }
    if (get_prefix_letter(r) == node->letter) {
      node->max_frag = max(node->max_frag, get_max_prefix(r) + 1);
    }
    node->max_frag = max(node->max_frag, 1);

    if (l == NULL) {
      node->prefix_letter = node->letter;
      node->max_prefix = 1;
      if (node->prefix_letter == get_prefix_letter(r)) {
        node->max_prefix += get_max_prefix(r);
      }
    }
    else {
      node->prefix_letter = get_prefix_letter(l);
      node->max_prefix = get_max_prefix(l);
      if (get_max_prefix(l) == get_subtree_size(l) && get_suffix_letter(l) == node->letter) {
        node->max_prefix++;
        if (node->letter == get_prefix_letter(r)) {
          node->max_prefix += get_max_prefix(r);
        }
      }
    }

    if (r == NULL) {
      node->suffix_letter = node->letter;
      node->max_suffix = 1;
      if (node->suffix_letter == get_suffix_letter(l)) {
        node->max_suffix += get_max_suffix(l);
      }
    }
    else {
      node->suffix_letter = get_suffix_letter(r);
      node->max_suffix = get_max_suffix(r);
      if (get_max_suffix(r) == get_subtree_size(r) && get_prefix_letter(r) == node->letter) {
        node->max_suffix++;
        if (node->letter == get_suffix_letter(l)) {
          node->max_suffix += get_max_suffix(l);
        }
      }
    }
  }
}

// Pushing reverse to sons (lazy propagation).
void push(Node* node) {
  if (node != NULL && node->is_reversed) {
    node->is_reversed = false;

    swap(node->prefix_letter, node->suffix_letter);
    swap(node->max_prefix, node->max_suffix);
    swap(node->left, node->right);

    if (node->left != NULL) {
      node->left->is_reversed ^= true;
      swap(node->left->max_prefix, node->left->max_suffix);
      swap(node->left->prefix_letter, node->left->suffix_letter);
    }
    if (node->right != NULL) {
      node->right->is_reversed ^= true;
      swap(node->right->max_prefix, node->right->max_suffix);
      swap(node->right->prefix_letter, node->right->suffix_letter);
    }
  }
}

// Correcting priorities so our binary tree has the heap property
// and becomes legit treap.
void correct_priority(Node* node) {
  if (node == NULL) {
    return;
  }

  Node* highest = node;
  if (node->left != NULL && node->left->prior > highest->prior) {
    highest = node->left;
  }
  if (node->right != NULL && node->right->prior > highest->prior) {
    highest = node->right;
  }

  if (highest != node) {
    swap(node->prior, highest->prior);
    correct_priority(highest);
  }
}

// Building treap recursively in O(n).
Node* build(const string& s, int l, int r, const V<int>& priority) {
  if (l > r) {
    return NULL;
  }
  else if (l == r) {
    return new Node(priority[l], s[l]);
  }

  int mid = (l + r) / 2;
  Node* node = new Node(priority[mid], s[mid]);
  node->left = build(s, l, mid - 1, priority);
  node->right = build(s, mid + 1, r, priority);

  correct_priority(node); // Node is also a heap now.
  update(node); // Node has correct members now.

  return node;
}

// Merging treaps l and r into result.
void merge(Node*& result, Node* l, Node* r) {
  push(l);
  push(r);

  if (l == NULL || r == NULL) {
    result = l == NULL ? r : l;
  }
  else if (l->prior > r->prior) {
    merge(l->right, l->right, r);
    result = l;
  }
  else {
    merge(r->left, l, r->left);
    result = r;
  }

  update(result);
}

// Spliting treap node into treaps l and r by x.
void split(Node* node, Node*& l, Node*& r, int x, int x_shift = 0) {
  if (node == NULL) {
    l = r = NULL;
    return;
  }

  push(node);

  int current_x = x_shift + get_subtree_size(node->left);
  if (x <= current_x) {
    split(node->left, l, node->left, x, x_shift);
    r = node;
  }
  else {
    split(node->right, node->right, r, x, current_x + 1);
    l = node;
  }

  update(node);
}

// Operation O j k.
void reverse(Node* node, int j, int k) {
  Node* n1;
  Node* n2;
  Node* n3;

  split(node, n1, n2, j);
  split(n2, n2, n3, k - j + 1);

  n2->is_reversed ^= true;

  merge(node, n1, n2);
  merge(node, node, n3);
}

// Operation P j k l.
void replace(Node* node, int j, int k, int l) {
  Node* n1;
  Node* n2;
  Node* n3;

  split(node, n1, n2, j);
  split(n2, n2, n3, k - j + 1);
  merge(node, n1, n3);
  split(node, n1, n3, l);
  merge(node, n1, n2);
  merge(node, node, n3);
}

// Operation N j k.
int query(Node* node, int j, int k) {
  Node* n1;
  Node* n2;
  Node* n3;

  split(node, n1, n2, j);
  split(n2, n2, n3, k - j + 1);

  int result = get_max_frag(n2);

  merge(node, n1, n2);
  merge(node, node, n3);

  return result;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  int n, m;
  string s;
  cin >> n >> m >> s;

  V<int> priority(n);
  for (int i = 0; i < n; i++) {
    priority[i] = i;
  }
  random_shuffle(priority.begin(), priority.end());

  Node* BST = build(s, 0, n - 1, priority);

  for (int i = 0; i < m; i++) {
    char operation;
    int j, k;
    cin >> operation >> j >> k;

    if (operation == 'O') {
      reverse(BST, j - 1, k - 1);
    }
    else if (operation == 'P') {
      int l;
      cin >> l;
      replace(BST, j - 1, k - 1, l - 1);
    }
    else {
      cout << query(BST, j - 1, k - 1) << '\n';
    }
  }
}
