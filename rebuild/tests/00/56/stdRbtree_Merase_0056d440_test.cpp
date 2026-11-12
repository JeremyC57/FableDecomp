// Behaviour test for 0x0056D440 std::_Rb_tree<NUISystem::CMenuEntry*, pair<..., CQuestManager::CBoast>, ...>::_M_erase (STLport 4.6): a test-only
// model of the erase walk (TestTree / TestValue are test-only stand-ins; agentNF23 respelled the old CRetailTree_erase0056d440 names).
#include <stdio.h>
#include <stdlib.h>
static int g_valuesDestroyed = 0;
static int g_nodesFreed = 0;
struct CTrackedBuffer {
    void* data;
    ~CTrackedBuffer() { ++g_valuesDestroyed; if (data) free(data); }
};
struct TestValue {
    unsigned long  key;
    CTrackedBuffer second;
};
static void FreeNode(void* p) { ++g_nodesFreed; free(p); }
struct TestTree {
    struct node {
        unsigned char colour;
        node*         parent;
        node*         left;
        node*         right;
        TestValue value;
    };
    node*         head;
    unsigned long size;
    void Erase(node* root);
};
void TestTree::Erase(node* x)
{
    while (x != 0) {
        Erase(x->right);
        node* y = x->left;
        x->value.~TestValue();
        FreeNode(x);
        x = y;
    }
}
typedef TestTree::node TNode;
static TNode* MakeNode(unsigned long key, TNode* left, TNode* right)
{
    TNode* n = (TNode*)malloc(sizeof(TNode));
    n->colour = 0; n->parent = 0; n->left = left; n->right = right;
    n->value.key = key;
    n->value.second.data = (key & 1) ? malloc(8) : 0;
    if (left) left->parent = n;
    if (right) right->parent = n;
    return n;
}
int main()
{
    int ok = 1;
    TNode* root = MakeNode(4, MakeNode(2, MakeNode(1, 0, 0), MakeNode(3, 0, 0)), MakeNode(6, MakeNode(5, 0, 0), MakeNode(7, 0, 0)));
    TestTree tree;
    tree.head = 0; tree.size = 7;
    tree.Erase(root);
    if (g_valuesDestroyed != 7 || g_nodesFreed != 7) ok = 0;
    tree.Erase(0);
    if (g_valuesDestroyed != 7 || g_nodesFreed != 7) ok = 0;
    printf(ok ? "0056d440_TEST PASS\n" : "0056d440_TEST FAIL\n");
    return ok ? 0 : 1;
}