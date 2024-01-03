#include "old_helpers.h"

template<typename T>
struct BSTNode {
    typedef std::unique_ptr<BSTNode<T>> node_ptr;

    static int created;

    T val;
    node_ptr left;
    node_ptr right;

    BSTNode(T val) : val(val), left(nullptr), right(nullptr) { created++; }
    BSTNode(T val, node_ptr left, node_ptr right) : val(val), left(std::move(left)), right(std::move(right)) { created++; }

    ~BSTNode() {
        created--;
    }

    bool find(T x, int (*comp)(T a, T b)) {
        int c = comp(x, val);
        if (c < 0) {
            if (left) return left->find(x, comp);
            return false;
        } else if (c > 0) {
            if (right) return right->find(x, comp);
            return false;
        } else return true;
    }

    bool insert(T x, int (*comp)(T a, T b)) {
        int c = comp(x, val);
        if (c <= 0) {
            if (left) left->insert(x, comp);
            else left = node_ptr(new BSTNode<T>(x));

            return c == 0;
        } else {
            if (right) return right->insert(x, comp);
            else right = node_ptr(new BSTNode<T>(x));

            return false;
        }
    }

    T removeRightmost(BSTNode<T>* parent) {
        if (right) return right->removeRightmost(this);
        else {
            T temp = val;
            if (parent->right.get() == this) {
                parent->right = std::move(left);
            } else if (parent->left.get() == this) {
                parent->left = std::move(left);
            }
            return temp;
        }
    }

    T removeLeftmost(BSTNode<T>* parent) {
        if (left) return left->removeLeftmost(this);
        else {
            T temp = val;
            if (parent->left.get() == this) {
                parent->left = std::move(left);
            } else if (parent->right.get() == this) {
                parent->right = std::move(left);
            }
            return temp;
        }
    }

    bool remove(T x, int (*comp)(T a, T b), BSTNode<T>* parent) {
        int c = comp(x, val);
        if (c < 0) {
            if (left) return left->remove(x, comp, this);
            return false;
        } else if (c > 0) {
            if (right) return right->remove(x, comp, this);
            return false;
        } else {
            if (left) {
                val = left->removeRightmost(this);
                return true;
            }
            if (right) {
                val = right->removeLeftmost(this);
                return true;
            }

            if (parent->left.get() == this) {
                parent->left.reset();
            } else if (parent->right.get() == this) {
                parent->right.reset();
            }
            return true;
        }
    }

    void printHelp(std::string topIndent, std::string botIndent, bool isTop, std::ostream& stream) {
        if (right) {
            right->printHelp(topIndent + "    ", topIndent + "|   ", true, stream);
        }
        stream << topIndent.substr(0, topIndent.length() - 4) << (isTop ? "/---" : "\\---") << val << '\n';
        if (left) {
            left->printHelp(botIndent + "|   ", botIndent + "    ", false, stream);
        }
    }

    void print(std::ostream& stream) {
        printHelp("    ", "    ", true, stream);
    }
};

int BSTNode<int>::created = 0;

template<typename T, int (*comp)(T a, T b)>
class BST {
public:
    typedef std::unique_ptr<BSTNode<T>> node_ptr;

    node_ptr root;

    BST() : root(nullptr) {}
    BST(node_ptr root) : root(root) {}

    bool find(T x) {
        if (root) return root->find(x, comp);
        return false;
    }

    bool insert(T x) {
        if (root) return root->insert(x, comp);
        
        root = node_ptr(new BSTNode<T>(x));
        return false;
    }

    bool remove(T x) {
        if (root) {
            if (!root->left && !root->right && comp(x, root->val) == 0) {
                root.reset();
                return true;
            }
            return root->remove(x, comp, nullptr);
        }
        return false;
    }

    bool clear() {
        if (root) {
            root.reset();
            return true;
        }
        return false;
    }

    void print(std::ostream& stream) {
        if (root) {
            root->print(stream);
        } else {
            stream << "Empty tree\n";
        }
    }
};

#define SPLIT cout << "Enter to continue" << flush; cin.ignore(1000, '\n'); system("cls")
int bst_main() {
    using namespace std;

    // BSTNode testTree{
    //     8,
    //     new BSTNode(4,
    //         new BSTNode(2,
    //             new BSTNode(1),
    //             new BSTNode(3)),
    //         new BSTNode(6,
    //             new BSTNode(5),
    //             new BSTNode(7))),
    //     new BSTNode(11,
    //         new BSTNode(9,
    //             nullptr,
    //             new BSTNode(10)),
    //         nullptr)
    // };

    // testTree.print(cout);
    // testTree.insert(23);
    // testTree.print(cout);

    //            8
    //           / \ 
    //          /   \ 
    //         /     \ 
    //        /       \ 
    //       /         \ 
    //      4           11
    //     / \         / 
    //    /   \       /
    //   2     6     9
    //  / \   / \     \ 
    // 1   3 5   7     10

    BST<int, int_less> tree;

    SPLIT;
    cout << "Initial list:\n";
    tree.print(cout);
    SPLIT;
    cout << "Insert 20:\n";
    tree.insert(20);
    tree.print(cout);
    SPLIT;
    cout << "Insert 40:\n";
    tree.insert(40);
    tree.print(cout);
    SPLIT;
    cout << "Insert 10:\n";
    tree.insert(10);
    tree.print(cout);
    SPLIT;
    cout << "Insert 20:\n";
    tree.insert(20);
    tree.print(cout);
    SPLIT;
    cout << "Insert 5:\n";
    tree.insert(5);
    tree.print(cout);
    SPLIT;
    cout << "Insert 80:\n";
    tree.insert(80);
    tree.print(cout);
    SPLIT;
    cout << "Delete 20:\n";
    tree.remove(20);
    tree.print(cout);
    SPLIT;
    cout << "Insert 100:\n";
    tree.insert(100);
    tree.print(cout);
    SPLIT;
    cout << "Insert 20:\n";
    tree.insert(20);
    tree.print(cout);
    SPLIT;
    cout << "Insert 30:\n";
    tree.insert(30);
    tree.print(cout);
    SPLIT;
    cout << "Delete 5:\n";
    tree.remove(5);
    tree.print(cout);
    SPLIT;
    cout << "Insert 50:\n";
    tree.insert(50);
    tree.print(cout);
    SPLIT;
    cout << "Lookup 80: ";
    cout << (tree.find(80) ? "found" : "not found") << '\n';
    tree.print(cout);
    SPLIT;
    cout << "Random Trial, 40 inserts:\n";
    for (int i = 0; i < 40; i++) {
        tree.insert(rand0toN(50));
    }
    tree.print(cout);
    cout << "\nRandom Trial, 10 deletions:\n";
    for (int i = 0; i < 10; i++) {
        tree.remove(rand0toN(50));
    }
    tree.print(cout);
    SPLIT;

    tree.clear();
    cout << "Undestroyed Nodes: " << BSTNode<int>::created << '\n';
    return 0;
}