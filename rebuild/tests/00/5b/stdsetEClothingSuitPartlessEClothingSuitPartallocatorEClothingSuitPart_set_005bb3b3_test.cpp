// Behaviour test for 005bb3b3: std::set<EClothingSuitPart>::set() -- an empty set (one header, no elements)
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
    Set s;
    int ok = s.t.count == 0 && s.t.header->left == s.t.header && s.t.header->right == s.t.header && s.t.header->parent == 0 &&
             g_allocs == 1 && sizeof(Set) == 12;
    std::free(s.t.header);
    std::printf(ok ? "005bb3b3_TEST PASS\n" : "005bb3b3_TEST FAIL\n");
    return ok ? 0 : 1;
}
