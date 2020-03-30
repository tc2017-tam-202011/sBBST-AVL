/*
 * This code implements a self-Balanced Binary Search Tree (sBBST).  A sBBST
 * is "a node-based binary search tree that automatically keeps its height (the
 * maximal number of levels below the root node) small in the face of arbitra-
 * ry item insertions and deletions." (C++ Program to Create a Balanced Binary
 * Tree of the Incoming Data, 2014)
 *
 * This implementation uses the AVL (Adelson, Velsky & Landis) tree. For more
 * information on AVL trees, check these resources:
 *
 * https://www.tutorialspoint.com/data_structures_algorithms/avl_tree_algorithm.htm
 * https://en.wikipedia.org/wiki/AVL_tree
 *
 * Code based almost entirely on:
 * https://www.tutorialspoint.com/cplusplus-program-to-implement-self-balancing-binary-search-tree
 */

#include <iostream>
#include <stdexcept>

using namespace std;

// This is the tree_node structure
struct tree_node {
    int value;
    struct tree_node *left;
    struct tree_node *right;
} *root;

// Type declaration of node, to ease the implementation
typedef struct tree_node node;

// Declaration of the AVL Tree class.  This class implements all the methods
// needed for a AVL sBBST.
class avl_tree {
    int elements;
public:
    int height(node *);
    int difference(node *);
    int numNodes(node *);
    int numNodesSmallerThan(node *, int);
    int numNodesGreaterThan(node *, int);
    int getNumElements();
    int kSmallest(node *, int);
    node *rr_rotation(node *);
    node *ll_rotation(node *);
    node *lr_rotation(node *);
    node *rl_rotation(node *);
    node *balance(node *);
    node *insert(node *, int);
    node *deleteNode(node *, int);
    node *minValueNode(node *);
    node *search(node *, int);
    void show(node *, int);
    void inorder(node *);
    void preorder(node *);
    void postorder(node *);

    // Constructor
    avl_tree() {
        root = nullptr;
        this->elements = 0;
    }
};

/*
 * int avl_tree::height(node *)
 * This method computes and returns the height of a tree, receiving a node.
 * If the node is NULL, its height is 0.  Otherwise, the height of the node
 * is 1 plus the max height amongst its children.  This is a recursive method.
 */
int avl_tree::height(node *tree) {
    int h = 0;
    if (tree != NULL) {
        int left_height = height(tree->left);
        int right_height = height(tree->right);
        int max_height = max(left_height, right_height);
        h = max_height + 1;
    }
    return h;
}

/*
 * int avl_tree::difference(node *)
 * This method computes and returns the difference in heights between a node's
 * left and right children.  It uses the avl_tree::height(node *) method.
 */
int avl_tree::difference(node *tree) {
    int left_height = height(tree->left);
    int right_height = height(tree->right);
    int balance_factor = left_height - right_height;
    return balance_factor;
}

/*
 * int avl_tree::numNodes(node *)
 * This method counts and returns the number of nodes in a given tree.
 */
int avl_tree::numNodes(node *tree) {
    if (tree == nullptr) {
        return 0;
    } else {
        return 1 + numNodes(tree->left) + numNodes(tree->right);
    }
}

/*
 * int avl_tree::numNodesSmallerThan(node *, int)
 * This method counts and returns the number of nodes in a given tree that
 * have a value smaller than a particular given value.
 */
int avl_tree::numNodesSmallerThan(node *tree, int x) {
    if (tree == nullptr) {
        return 0;
    }
    if (tree->value == x) {
        return numNodes(tree->left);
    } else if (tree->value < x) {
        return 1 + numNodes(tree->left) + numNodesSmallerThan(tree->right, x);
    } else {
        return numNodesSmallerThan(tree->left, x);
    }
}

/*
 * int avl_tree::numNodesGreaterThan(node *, int)
 * This method counts and returns the number of nodes in a given tree that
 * have a value greater than a particular fiven value.
 */
int avl_tree::numNodesGreaterThan(node *tree, int x) {
    if (tree == nullptr) {
        return 0;
    }
    if (tree->value == x) {
        return numNodes(tree->right);
    } else if (tree->value > x) {
        return 1 + numNodes(tree->right) + numNodesGreaterThan(tree->left, x);
    } else {
        return numNodesGreaterThan(tree->right, x);
    }
}

/*
 * int avl_tree::getNumElements()
 * Getter for the elements property.
 */
int avl_tree::getNumElements() {
    return this->elements;
}

/*
 * int avl_tree::kSmallest(node *, int);
 * This method searches for and returns the kth smallest value within a tree.
 * If k is less than 1 or greater than the total amounts of nodes within the
 * tree, the function raises an exception.
 * This algorithm uses the Morris Traversal
 */
int avl_tree::kSmallest(node *rootNode, int k) {
    if (k < 1 || k > this->elements) {
        throw invalid_argument("impossible value for k");
    }
    int count = 0;
    int ksmall = INT_MIN;
    node *curr = rootNode;

    while (curr != nullptr) {
        if (curr->left == nullptr) {
            count++;
            if (count == k) {
                ksmall = curr->value;
            }
            curr = curr->right;
        } else {
            node *pre = curr->left;
            while (pre->right != nullptr && pre->right != curr) {
                pre = pre->right;
            }

            if (pre->right == nullptr) {
                pre->right = curr;
                curr = curr->left;
            } else {
                pre->right = nullptr;
                count++;
                if (count == k) {
                    ksmall = curr->value;
                }
                curr = curr->right;
            }
        }
    }
    return ksmall;
}
/*
 * node *avl_tree::rr_rotation(node *)
 * This method performs a right right rotation on a node to balance it.
 */
node *avl_tree::rr_rotation(node *parent) {
    node *temp;
    temp = parent->right;
    parent->right = temp->left;
    temp->left = parent;
    return temp;
}

/*
 * node *avl_tree::ll_rotation(node *)
 * This method performs a left left rotation on a node to balance it.
 */
node *avl_tree::ll_rotation(node *parent) {
    node *temp;
    temp = parent->left;
    parent->left = temp->right;
    temp->right = parent;
    return temp;
}

/*
 * node *avl_tree::lr_rotation(node *)
 * This method performs a left right rotation on a node to balance it. It uses
 * the *avl_tree::ll_rotation(node *) and *avl_tree::rr_rotation(node *)
 * methods
 */
node *avl_tree::lr_rotation(node *parent) {
    node *temp;
    temp = parent->left;
    parent->left = rr_rotation(temp);
    return ll_rotation(parent);
}

/*
 * node *avl_tree::rl_rotation(node *)
 * This method performs a right left rotation on a node to balance it. It uses
 * the *avl_tree::ll_rotation(node *) and *avl_tree::rr_rotation(node *)
 * methods
 */
node *avl_tree::rl_rotation(node *parent) {
    node *temp;
    temp = parent->right;
    parent->right = ll_rotation(temp);
    return rr_rotation(parent);
}

/*
 * node *avl_tree::balance(node *)
 * This method balance the tree of a given node.  It gets the difference in
 * height of the node's children.  If the balance factor is less than -1 or
 * if it is greater than 1, the method rotates nodes until the tree is
 * balanced.
 */
node *avl_tree::balance(node *tree) {
    int balance_factor = difference(tree);
    if (balance_factor > 1) {
        if (difference(tree->left) > 0) {
            tree = ll_rotation(tree);
        } else {
            tree = lr_rotation(tree);
        }
    } else if (balance_factor < -1) {
        if (difference(tree->right) > 0) {
            tree = rl_rotation(tree);
        } else {
            tree = rr_rotation(tree);
        }
    }
    return tree;
}

/*
 * node *avl_tree::insert(node *, int)
 * This method inserts a value into the given tree.  If the value is already
 * within the given tree, the method does nothing.
 */
node *avl_tree::insert(node *rootNode, int value) {
    if (rootNode == nullptr) {
        rootNode = new node;
        rootNode->value = value;
        rootNode->left = nullptr;
        rootNode->right = nullptr;
        this->elements += 1;
    } else if (value < rootNode->value) {
        rootNode->left = insert(rootNode->left, value);
        rootNode = balance(rootNode);
    } else if (value > rootNode->value) {
        rootNode->right = insert(rootNode->right, value);
        rootNode = balance(rootNode);
    }
    return rootNode;
}

/*
 * node *avl_tree::deleteNode(node *, int)
 * This method removes a value into the given tree.  If the value is not
 * within the given tree, the method does nothing.
 */
node *avl_tree::deleteNode(node *rootNode, int value) {
    if (root == nullptr) {
        return root;
    }

    if (value < rootNode->value) {
        rootNode->left = deleteNode(rootNode->left, value);
    } else {
        if (value > rootNode->value) {
            rootNode->right = deleteNode(rootNode->right, value);
        } else {
            // If the node has one or no child
            if (rootNode->left == nullptr) {
                node *temp = root->right;
                free(rootNode);
                return temp;
            } else if (rootNode->right == nullptr) {
                    node *temp = root->left;
                    free(rootNode);
                    return temp;
            }
            // if the node has two children, then we search for the
            // inorder successor in the right children tree.
            node *temp = minValueNode(rootNode->right);
            rootNode->value = temp->value;
            rootNode->right = deleteNode(rootNode->right, temp->value);
            this->elements -= 1;
        }
    }
    balance(rootNode);
    return rootNode;
}

/*
 * node *avl_tree::minValueNode(node *)
 * This method finds and returns the node with the smallest value within the
 * given tree
 */
node *avl_tree::minValueNode(node *rootNode) {
    node *current = rootNode;
    while (current && current->left != nullptr) {
        current = current->left;
    }
    return current;
}

/*
 * node *avl_tree::search(node *, int)
 * This method search for a key within the tree, and returns the node that
 * holds that value if it finds it.
 */
node *avl_tree::search(node *tree, int value) {
    if (tree == nullptr) {
        return nullptr;
    }
    if (tree->value == value) {
        return tree;
    } else if (tree->value > value) {
        return search(tree->left, value);
    } else {
        return search(tree->right, value);
    }
}

/*
 * void avl_tree::show(node *, int)
 * Shows the balanced tree.
 */
void avl_tree::show(node *position, int level) {
    int i;
    if (position != nullptr) {
        show(position->right, level + 1);
        cout << " ";
        if (position == root) {
            cout << "Root -> ";
        }
        for (i = 0; (i < level) && (position != root); i++) {
            cout << " ";
        }
        cout << position->value;
        show(position->left, level + 1);
    }
}

/*
 * void avl_tree::inorder(node *)
 * Shows the tree inorder traversal.
 */
void avl_tree::inorder(node *tree) {
    if (tree == nullptr) {
        return;
    }
    inorder(tree->left);
    cout << tree->value << " ";
    inorder(tree->right);
}

/*
 * void avl_tree::preorder(node *)
 * Shows the tree preorder traversal
 */
void avl_tree::preorder(node *tree) {
    if (tree == nullptr) {
        return;
    }
    cout << tree->value << " ";
    preorder(tree->left);
    preorder(tree->right);
}

/*
 * void avl_tree::postorder(node *)
 * Shows the tree postorder traversal
 */
void avl_tree::postorder(node *tree) {
    if (tree == nullptr) {
        return;
    }
    postorder(tree->left);
    postorder(tree->right);
    cout << tree->value << " ";
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
