#include <iostream>
#include <cmath>

#include <vector>

class CompleteBinaryTree {
private:
    std::vector<int> tree;
public:
    CompleteBinaryTree() {
        // Do not use the 0th index
        tree.push_back(0);
    }

    // Constructor according the leaf nodes
    // The value of non-leaf nodes is the average of its children
    CompleteBinaryTree(std::vector<int> leaf_nodes) {
        int TreeSize = 2 * leaf_nodes.size()-1;
        tree.resize( TreeSize + 1, 0);
        tree[0] = 0;// Do not use the 0th index

        // Calculate the depth of the tree
        int Depth = log2(leaf_nodes.size()) + 1;

        int Index_1 = pow(2, Depth);// The left-most index of the last level
        int Index_2 = Index_1 - (leaf_nodes.size() - (TreeSize - Index_1))+1; // The left-most index of the leaf node in the last full level

#if 0
        std::cout << "TreeSize: " << TreeSize << std::endl;
        std::cout << "tree.size()" << tree.size() << std::endl;
        std::cout << "Depth: " << Depth << std::endl;
        std::cout << "Index_1: " << Index_1 << std::endl;
        std::cout << "Index_2: " << Index_2 << std::endl;
#endif

        // Assign the leaf nodes
        for(int i = Index_2; i < tree.size(); i++) {
            if(i < Index_1)
                tree[i] = leaf_nodes[ (TreeSize - Index_1) + 1 +  i - Index_2];
            else
                tree[i] = leaf_nodes[i - Index_1];
        }


        // Assign the non-leaf nodes
        for(int i = Index_2 - 1; i > 0; i--) {
            tree[i] = (tree[2*i] + tree[2*i + 1]) / 2;
        }

    }

    ~CompleteBinaryTree() {}

#if 0
    // Get the parent of the node at index i
    int parent(int i) {
        return i / 2;
    }

    // Get the left child of the node at index i
    int left(int i) {
        return 2 * i;
    }

    // Get the right child of the node at index i
    int right(int i) {
        return 2 * i + 1;
    }
#endif

    // Traverse the tree
    void traverse() {
        for (int i = 1; i < tree.size(); i++) {
            std::cout << " In " << i << " : " << tree[i] << std::endl;
        }
    }
};