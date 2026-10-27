// behaviour test for 0x00797A60 std::map<CThingCreatureBase*, CTCUnholyPower::PerCreatureInfo>::erase(iterator) (model; the rebalance and value destructor are stubs)
#include <cstdio>
#include <cstdlib>
struct Node { bool color; Node* parent; Node* left; Node* right; int key; int value[6]; };
static Node* g_z = 0; static int g_rebalance = 0, g_dtor = 0;
static Node* Rebalance_for_erase(Node* z, Node*& root, Node*& leftmost, Node*& rightmost)
{
    ++g_rebalance; g_z = z;
    if (root == z) root = 0;
    if (leftmost == z) leftmost = (Node*)&root;
    if (rightmost == z) rightmost = (Node*)&root;
    return z;
}
static void DestroyValue(int*) { ++g_dtor; }
struct Tree {
    Node* header; unsigned count; int comp;
    void erase(Node* pos)
    {
        Node* y = Rebalance_for_erase(pos, header->parent, header->left, header->right);
        DestroyValue(y->value);
        if (y != 0) std::free(y);
        --count;
    }
};
int main()
{
    Node hdr = { false, 0, 0, 0, 0, { 0 } };
    Node* n = (Node*)std::malloc(sizeof(Node));
    n->parent = &hdr; n->left = n->right = 0;
    hdr.parent = n; hdr.left = n; hdr.right = n;
    Tree t; t.header = &hdr; t.count = 1; t.comp = 0;
    t.erase(n);
    int ok = g_rebalance == 1 && g_dtor == 1 && t.count == 0 && hdr.parent == 0;
    if (!ok) { std::printf("00797a60_TEST FAIL\n"); return 1; }
    std::printf("00797a60_TEST PASS\n");
    return 0;
}