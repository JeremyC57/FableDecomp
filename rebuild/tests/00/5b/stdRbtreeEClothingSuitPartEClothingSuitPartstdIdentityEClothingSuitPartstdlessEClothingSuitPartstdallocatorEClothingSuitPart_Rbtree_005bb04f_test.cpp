// Portable test for 0x005BB04F std::_Rb_tree<EClothingSuitPart,...>::~_Rb_tree(): clear() every node, then free the header node.
#include <stdio.h>
#include <stdlib.h>

enum EClothingSuitPart { CLOTHING_SUIT_PART_NONE };

static int g_clear = 0;
static int g_freed = 0;

struct RbNode { bool color; RbNode* parent; RbNode* left; RbNode* right; EClothingSuitPart value; };

struct Tree {
    RbNode*      header;      // +0x00 (_M_header._M_data)
    unsigned int node_count;  // +0x04
    void clear() { ++g_clear; node_count = 0; }
    ~Tree() {
        clear();                                  // ~_Rb_tree() { clear(); }
        RbNode* h = header;                       // ~_Rb_tree_base(): _M_header.deallocate(_M_header._M_data, 1)
        if (h) { free(h); ++g_freed; }
    }
};

int main() {
    {
        Tree t; t.header = (RbNode*)malloc(sizeof(RbNode)); t.node_count = 3;
    }
    if (g_clear != 1 || g_freed != 1) { printf("FAIL header\n"); return 1; }
    {
        Tree t; t.header = 0; t.node_count = 0;
    }
    if (g_clear != 2 || g_freed != 1) { printf("FAIL null header\n"); return 1; }
    printf("005bb04f_TEST PASS\n");
    return 0;
}
