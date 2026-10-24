// Behaviour test for 0x006DBC00: the {long, tree} default constructor zeroes the long and builds an empty,
// self-linked tree head (colour 0, no parent) with a zero node count.
#include <cstdio>
#include <cstdlib>
#include <new>

struct SRetailTree_node0x18 {
    struct node { unsigned char colour; unsigned char _pad[3]; node* parent; node* left; node* right; unsigned char value[8]; };
    node*         head;
    unsigned long size;
    SRetailTree_node0x18() : head(0)
    {
        head = (node*)std::malloc(sizeof(node));
        size = 0;
        head->colour = 0;
        head->parent = 0;
        head->left   = head;
        head->right  = head;
    }
};

struct SLongAndTree {
    long first;
    SRetailTree_node0x18 second;
    SLongAndTree() : first(0) {}
};

int main()
{
    unsigned char raw[sizeof(SLongAndTree)];
    for (unsigned int i = 0; i < sizeof(raw); ++i) raw[i] = 0xAB;
    SLongAndTree* p = new (raw) SLongAndTree;
    SRetailTree_node0x18::node* h = p->second.head;
    if (p->first != 0 || p->second.size != 0 || h == 0 || h->colour != 0 || h->parent != 0 || h->left != h || h->right != h) {
        std::printf("006dbc00_TEST FAIL\n"); return 1;
    }
    std::free(h);
    std::printf("006dbc00_TEST PASS\n");
    return 0;
}