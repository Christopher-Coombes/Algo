#include <vector>
#include <cmath>
#include <iomanip>
#include "old_helpers.h"

template<typename T>
struct SLNode {
    typedef std::unique_ptr<SLNode<T>> node_ptr;
    typedef SLNode<T>* raw_node_ptr;

    static int created;

    T val;
    node_ptr right;
    raw_node_ptr down;

    SLNode(T val) : val(val), right(nullptr), down(nullptr) { created++; }
    SLNode(T val, node_ptr right, raw_node_ptr down) : val(val), right(std::move(right)), down(down) { created++; }

    ~SLNode() {
        created--;
    }
    
    bool find(T x, int (*comp)(T a, T b)) {
        if (right) {
            int c = comp(x, right->val);
            if (c == 0) return true;
            if (c > 0) return right->find(x, comp);
            // if c < 0, fall through
        }
        if (down) return down->find(x, comp);
        return false;
    }

    raw_node_ptr insertDown(T x, int (*comp)(T a, T b), int levels) {
        if (right) {
            int c = comp(x, right->val);
            if (c >= 0) return right->insertDown(x, comp, levels);
            // if c < 0, fall through
        }
        raw_node_ptr rest = nullptr;
        if (down) rest = down->insertDown(x, comp, levels - 1);

        if (levels <= 0) {
            right = node_ptr(new SLNode<T>(x, std::move(right), rest));
            return right.get();
        }
        return nullptr;
    }

    bool remove(T x, int (*comp)(T a, T b)) {
        if (right) {
            int c = comp(x, right->val);
            if (c > 0) return right->remove(x, comp);
            if (c == 0) {
                right = std::move(right->right);
                // fall through
            }
            // if c < 0, fall through
        }
        if (down) return down->remove(x, comp);
        return false;
    }
};
int SLNode<int>::created = 0;

template<typename T>
struct SLRoot {
    typedef std::unique_ptr<SLNode<T>> node_ptr;
    typedef SLNode<T>* raw_node_ptr;
    typedef std::unique_ptr<SLRoot<T>> root_ptr;
   
    static int created;

    int level;
    node_ptr right;
    root_ptr down;

    SLRoot(int level) : level(level), right(nullptr), down(nullptr) { created++; }
    SLRoot(int level, root_ptr down) : level(level), right(nullptr), down(std::move(down)) { created++; }

    ~SLRoot() {
        created--;
    }

    bool find(T x, int (*comp)(T a, T b)) {
        if (right) {
            int c = comp(x, right->val);
            if (c == 0) return true;
            if (c > 0) return right->find(x, comp);
            // if c < 0, fall through
        }
        if (down) return down->find(x, comp);
        return false;
    }

    raw_node_ptr insertDown(T x, int (*comp)(T a, T b), int levels) {
        if (right) {
            int c = comp(x, right->val);
            if (c >= 0) return right->insertDown(x, comp, levels);
            // if c < 0, fall through
        }
        raw_node_ptr rest = nullptr;
        if (down) rest = down->insertDown(x, comp, levels - 1);

        if (levels <= 0) {
            right = node_ptr(new SLNode<T>(x, std::move(right), rest));
            return right.get();
        }
        return nullptr;
    }

    bool remove(T x, int (*comp)(T a, T b)) {
        if (right) {
            int c = comp(x, right->val);
            if (c > 0) return right->remove(x, comp);
            if (c == 0) {
                right = std::move(right->right);
                // fall through
            }
            // if c < 0, fall through
        }
        if (down) return down->remove(x, comp);
        return false;
    }

    std::vector<std::pair<raw_node_ptr, int>> print(std::ostream& stream) {
        std::vector<std::pair<raw_node_ptr, int>> pos;
        int x = 0;
        raw_node_ptr node = right.get();

        if (down != nullptr) {
            auto ppos = down->print(stream);
            auto it = ppos.cbegin();
            stream << "[" << std::setw(3) << level << "]";
            while (node) {
                while (it != ppos.cend() && it->first != node->down) it++;
                stream << std::string(it->second - x, '-') << node->val;
                x = it->second;
                pos.emplace_back(node, x);
                x += numwidth(node->val);
                node = node->right.get();
            }
            stream << '\n';
        } else {
            stream << "[" << std::setw(3) << level << "]";
            while (node) {
                stream << '-' << node->val;
                x += 1;
                pos.emplace_back(node, x);
                x += numwidth(node->val);
                node = node->right.get();
            }
            stream << '\n';
        }

        return pos;
    }
};
int SLRoot<int>::created = 0;

template<typename T, int (*comp)(T a, T b)>
class SkipList {
    typedef std::unique_ptr<SLNode<T>> node_ptr;
    typedef SLNode<T>* raw_node_ptr;
    typedef std::unique_ptr<SLRoot<T>> root_ptr;
public:
    root_ptr root;

    SkipList() : root(nullptr) {}

    bool find(T x) {
        if (root) return root->find(x, comp);
        return false;
    }

    void insert(T x) {
        // int p = static_cast<int>(floor(-log2(rand0to1())));
        int p = 0;
        while (rand0to1() < 0.5) p++;
        insert(x, p < 25 ? p : 25);
    }

    void insert(T x, int depth) {
        if (!root) root = root_ptr(new SLRoot<T>(0));
        while (root->level < depth) {
            int level = root->level + 1;
            root = root_ptr(new SLRoot<T>(level, std::move(root)));
        }
        root->insertDown(x, comp, root->level - depth);
    }

    bool remove(T x) {
        if (!root) return false;

        bool out = root->remove(x, comp);
        while (root->down && !root->right) {
            root = std::move(root->down);
        }
        if (!root->right) root.reset();
        return out;
    }

    void rebalance() {
        if (!root) return;

        while (root->down) {
            root = std::move(root->down);
        }
        while (root->right && root->right->right) {
            raw_node_ptr ptr = root->right->right.get();
            int level = root->level + 1;
            root = root_ptr(new SLRoot<T>(level, std::move(root)));
            root->right = node_ptr(new SLNode<T>(ptr->val, nullptr, ptr));
            raw_node_ptr tail = root->right.get();

            while (ptr->right && ptr->right->right) {
                ptr = ptr->right->right.get();
                tail->right = node_ptr(new SLNode<T>(ptr->val, nullptr, ptr));
                tail = tail->right.get();
            }
        }
    }

    bool clear() {
        if (!root) return false;
        root.reset();
        return true;
    }

    void print(std::ostream& stream) {
        if (root) root->print(stream);
        else stream << "Empty list\n";
    }
};

#define SPLIT cout << "Enter to continue" << flush; cin.ignore(1000, '\n'); system("cls")
int skiplist_main() {
    using namespace std;
    
    SkipList<int, int_less> sl;

    SPLIT;
    cout << "Initial list:\n";
    sl.print(cout);
    SPLIT;
    cout << "Insert 20:\n";
    sl.insert(20, 2);
    sl.print(cout);
    SPLIT;
    cout << "Insert 40:\n";
    sl.insert(40);
    sl.print(cout);
    SPLIT;
    cout << "Insert 10:\n";
    sl.insert(10);
    sl.print(cout);
    SPLIT;
    cout << "Insert 20:\n";
    sl.insert(20);
    sl.print(cout);
    SPLIT;
    cout << "Insert 5:\n";
    sl.insert(5);
    sl.print(cout);
    SPLIT;
    cout << "Insert 80:\n";
    sl.insert(80);
    sl.print(cout);
    SPLIT;
    cout << "Delete 20:\n";
    sl.remove(20);
    sl.print(cout);
    SPLIT;
    cout << "Insert 100:\n";
    sl.insert(100);
    sl.print(cout);
    SPLIT;
    cout << "Insert 20:\n";
    sl.insert(20, 1);
    sl.print(cout);
    SPLIT;
    cout << "Insert 30:\n";
    sl.insert(30);
    sl.print(cout);
    SPLIT;
    cout << "Delete 5:\n";
    sl.remove(5);
    sl.print(cout);
    SPLIT;
    cout << "Insert 50:\n";
    sl.insert(50);
    sl.print(cout);
    SPLIT;
    cout << "Lookup 80: ";
    cout << (sl.find(80) ? "found" : "not found") << '\n';
    sl.print(cout);
    SPLIT;
    cout << "Rebalance:\n";
    sl.rebalance();
    sl.print(cout);
    SPLIT;
    cout << "Random Trial, 40 inserts:\n";
    for (int i = 0; i < 40; i++) {
        sl.insert(rand0toN(50));
    }
    sl.print(cout);
    cout << "\nRandom Trial, 10 deletions:\n";
    for (int i = 0; i < 10; i++) {
        sl.remove(rand0toN(50));
    }
    sl.print(cout);
    cout << "\nRebalance:\n";
    sl.rebalance();
    sl.print(cout);
    SPLIT;

    sl.clear();
    cout << "Undestroyed Nodes: " << SLNode<int>::created << '\n';
    cout << "Undestroyed Roots: " << SLRoot<int>::created << '\n';
    return 0;
}