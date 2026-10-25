// Behaviour test for 005bb025: std::_Rb_tree<EClothingSuitPart,..>::_Rb_tree(comp, a) -- empty tree: count 0, red header, no root, header self-linked
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
    Less c; Alloc a; Tree t(c, a);
    int ok = t.count == 0 && t.header->color == false && t.header->parent == 0 && t.header->left == t.header &&
             t.header->right == t.header && g_allocs == 1;
    std::free(t.header);
    std::printf(ok ? "005bb025_TEST PASS\n" : "005bb025_TEST FAIL\n");
    return ok ? 0 : 1;
}
