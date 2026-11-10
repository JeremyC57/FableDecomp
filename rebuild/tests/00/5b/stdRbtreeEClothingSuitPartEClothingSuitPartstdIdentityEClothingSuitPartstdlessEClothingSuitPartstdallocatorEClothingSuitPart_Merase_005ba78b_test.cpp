// Portable test for 0x005BA78B: std::_Rb_tree<EClothingSuitPart,...>::_M_erase(node) (STLport 4.6 _tree.h / _tree.c).
#include <stdio.h>
#include <stdlib.h>

enum EClothingSuitPart { CLOTHING_SUIT_PART_A, CLOTHING_SUIT_PART_B };
struct NodeBase { bool color; NodeBase* parent; NodeBase* left; NodeBase* right; };
struct Node : NodeBase { EClothingSuitPart value; };
static int g_freed = 0;

struct Tree {
    Node*        header;      // +0x00 _M_header._M_data
    unsigned int node_count;  // +0x04
    void _M_erase(Node* x) {                                       // 0x005BA78B
        while (x != 0) {
            _M_erase((Node*)x->right);
            Node* y = (Node*)x->left;
            free(x); ++g_freed;
            x = y;
        }
    }
    void clear() {                                                 // 0x005BABB8
        if (node_count != 0) {
            _M_erase((Node*)header->parent);
            header->left = header; header->parent = 0; header->right = header;
            node_count = 0;
        }
    }
};

static Node* mk(NodeBase* parent) { Node* n = (Node*)malloc(sizeof(Node)); n->parent = parent; n->left = n->right = 0; n->value = CLOTHING_SUIT_PART_A; return n; }

int main() {
    Node header; header.color = false;
    Tree t; t.header = &header; t.node_count = 4;
    Node* root = mk(&header); header.parent = root;
    root->left = mk(root); root->right = mk(root); root->right->right = mk(root->right);
    header.left = (Node*)root->left; header.right = (Node*)root->right->right;
    t.clear();
    if (g_freed != 4 || t.node_count != 0 || header.parent != 0 || header.left != &header || header.right != &header) { printf("FAIL clear\n"); return 1; }
    t.clear();                                                     // empty: nothing freed, header untouched
    if (g_freed != 4) { printf("FAIL empty\n"); return 1; }
    Node* lone = mk(0); t._M_erase(lone); t._M_erase(0);
    if (g_freed != 5) { printf("FAIL erase\n"); return 1; }
    printf("005ba78b_TEST PASS\n");
    return 0;
}
