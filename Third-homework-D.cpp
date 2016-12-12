#include<iostream>
#include<vector>
#include<utility>
#include<algorithm>

using std::cin;
using std::cout;
using std::vector;
using std::max;
using std::min;
using std::pair;
using std::swap;
using std::ostream;
using std::istream;

typedef long long ll;

// node of segment tree
struct node {
    node * left = nullptr;
    node * right = nullptr;
    int value = 0;

    explicit node (int n_value) {
        value = n_value;
    }

    node (node * n_left, node * n_right) : left(n_left), right(n_right) {
        if (left) {
            value += left->value;
        }
        if (right) {
            value += right->value;
        }
    }
};

// segment tree that we need to get answer
struct PersistentSegmentTree {
    // dynamic array of version roots
    vector<node *> roots;
    // first(initial) version
    node * first;
    // size of base array
    int size;
    PersistentSegmentTree(int ar_size) {
        size = ar_size;
        first = firstCreate(0, size - 1);
        roots.resize(static_cast<size_t>(size));
    }

    node * firstCreate (int cur_l, int cur_r) {
        if (cur_l == cur_r) {
            return new node(0);
        }

        int cur_m = (cur_r + cur_l) >> 1;

        return new node (firstCreate(cur_l, cur_m),
                         firstCreate(cur_m + 1, cur_r));
    }

    // function that add new_version to roots array
    void addToRoots(node * version, int index) {
        roots[index] = version;
    }

    // update function that return new version node
    node * update(int pos, int new_value, node * version, int index = -1) {
        node * root = nullptr;
        if (version == nullptr) {
            root = first;
        } else {
            root = version;
        }

        auto res = update(root, 0, size - 1, pos, new_value);
        return res;
    }

    node * update(node * cur, int cur_l, int cur_r, int pos, int new_value) {
        if (cur_l == cur_r) {
            return new node(new_value);
        }
        int cur_m = (cur_r + cur_l) >> 1;
        if (pos <= cur_m) {
            return new node(
                    update(cur->left, cur_l, cur_m, pos, new_value),
                    cur->right);
        } else {
            return new node(
                    cur->left,
                    update(cur->right, cur_m + 1, cur_r, pos, new_value));
        }
    }

    // searching needed r in segment tree
    int search(int version, int k_value) {

        int res = search(roots[version], 0, size - 1, version, k_value);
        if (k_value > 0) {
            return -1;
        }
        return res;
    }

    int search(node * cur, int cur_l, int cur_r, int l_value, int & k_value) {
        if (cur_r < l_value) {
            return -1;
        }
        if ((l_value <= cur_l && (cur->value < k_value || cur_l == cur_r)) || cur_l == cur_r) {
            k_value -= cur->value;
            return cur_r;
        }

        int cur_m = (cur_r + cur_l) >> 1;
        int res = search(cur->left, cur_l, cur_m, l_value, k_value);
        if (k_value > 0) {
            res = search(cur->right, cur_m + 1, cur_r, l_value, k_value);
        }
        return res;
    }
};

// input function
void readArray(istream & in, int & array_size, int & range, vector<int> &  array) {
    cin >> array_size >> range;
    array.resize(static_cast<size_t>(array_size));
    for (auto&& item: array) {
        cin >> item;
    }
    return;
}

//build all needed version of segment tree
PersistentSegmentTree * buildPresistentSegmentTree(
        const vector<int> & array, int ar_size, int range) {

    vector<vector <int>> needed_occurrence;
    vector<int> last_by_number;

    // positions in array by number
    needed_occurrence.resize(static_cast<size_t>(range) + 1);

    // last index in needed_occurrence array
    last_by_number.resize(static_cast<size_t>(range) + 1);


    for (int i = 0; i < ar_size; ++i) {
        needed_occurrence[array[i]].push_back(i);
    }

    for (int i = 1; i <= range; ++i) {
        last_by_number[i] = static_cast<int>(needed_occurrence[i].size());
    }
    PersistentSegmentTree * tree = new PersistentSegmentTree(ar_size);
    node * cur_version = nullptr;
    for (int i = ar_size - 1; i >= 0; --i) {
        cur_version = tree->update(i, 1, cur_version);
        int c_num = array[i];
        if (--last_by_number[c_num] < needed_occurrence[c_num].size() - 1) {
            int next = needed_occurrence[c_num][last_by_number[c_num] + 1];
            cur_version = tree->update(next, 0, cur_version, i);
        }
        tree->addToRoots(cur_version, i);
    }
    return tree;
}

// function that perform query processing (read query and answer)
void queryProcessing(istream & in, ostream & out,
                     PersistentSegmentTree * tree, int ar_size, int range) {
    int number_of_queries;
    in >> number_of_queries;
    int x_value, y_value, p_value = 0;
    int l_value, k_value;
    while (number_of_queries--) {
        in >> x_value >> y_value;
        l_value = ((x_value + p_value) % ar_size) + 1;
        k_value = ((y_value + p_value) % range) + 1;
        int res = tree->search(l_value - 1, k_value);
        ++res;
        out << (p_value = res) << "\n";
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int ar_size;
    int range;
    vector<int> array;

    readArray(cin, ar_size, range, array);
    PersistentSegmentTree * tree = buildPresistentSegmentTree(array, ar_size, range);

    queryProcessing(cin, cout, tree, ar_size, range);

    return 0;
}
