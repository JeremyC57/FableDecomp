// Behaviour test for 005babe1: std::_Rb_tree_base<EClothingSuitPart,..>::_Rb_tree_base -- one 0x14-byte header node allocated
#include <cstdio>
#include <cstdlib>
enum EClothingSuitPart { PART_0, PART_1, PART_2 };
struct NodeBase { bool color; NodeBase* parent; NodeBase* left; NodeBase* right; };
struct Node : NodeBase { EClothingSuitPart value; };
struct Less { bool operator()(const EClothingSuitPart& a, const EClothingSuitPart& b) const { return a < b; } };
struct Alloc { };
static int g_allocs = 0;
struct TreeBase {
    Node* header;
    explicit TreeBase(const Alloc&) : header(0) { header = (Node*)std::malloc(sizeof(Node)); ++g_allocs; }
};
struct Tree : TreeBase {
    unsigned int count; Less comp;
    Tree(const Less& c, const Alloc& a) : TreeBase(a), count(0), comp(c)
    { header->color = false; header->parent = 0; header->left = header; header->right = header; }
};
struct Set { Tree t; Set() : t(Less(), Alloc()) { } };

int main()
{
    Alloc a; TreeBase b(a);
    int ok = b.header != 0 && g_allocs == 1 && sizeof(Node) == 0x14;
    std::free(b.header);
    std::printf(ok ? "005babe1_TEST PASS\n" : "005babe1_TEST FAIL\n");
    return ok ? 0 : 1;
}
