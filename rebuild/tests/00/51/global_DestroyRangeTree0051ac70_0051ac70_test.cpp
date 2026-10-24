#include <stdio.h>
#include <stdlib.h>
static int g_nodesFreed = 0;
static void FreeBlock(void* p) { ++g_nodesFreed; free(p); }
struct CTree {
    struct node { unsigned char colour; node* parent; node* left; node* right; };
    node*         head;
    unsigned long size;
    unsigned long _comp;
    void Erase(node* x)
    {
        while (x != 0) {
            Erase(x->right);
            node* y = x->left;
            FreeBlock(x);
            x = y;
        }
    }
    void clear()
    {
        if (size) {
            Erase(head->parent);
            head->left = head;
            head->parent = 0;
            head->right = head;
            size = 0;
        }
    }
    ~CTree()
    {
        clear();
        if (head)
            FreeBlock(head);
    }
};
static CTree::node* NewNode(CTree::node* left, CTree::node* right)
{
    CTree::node* n = (CTree::node*)malloc(sizeof(CTree::node));
    n->colour = 0; n->parent = 0; n->left = left; n->right = right;
    return n;
}
static void InitTree(CTree& t, int three)
{
    t.head = NewNode(0, 0);
    if (three) {
        CTree::node* root = NewNode(NewNode(0, 0), NewNode(0, 0));
        root->parent = t.head; t.head->parent = root; t.head->left = root->left; t.head->right = root->right;
        t.size = 3;
    } else {
        t.head->parent = 0; t.head->left = t.head; t.head->right = t.head; t.size = 0;
    }
    t._comp = 0;
}
struct CRetailAllocator { };
void DestroyRange_Tree_0051ac70(CTree* first, CTree* last, CRetailAllocator allocator)
{
    for (; first != last; ++first)
        first->~CTree();
}
int main()
{
    int ok = 1;
    CTree* trees = (CTree*)malloc(3 * sizeof(CTree));
    InitTree(trees[0], 1);
    InitTree(trees[1], 0);
    InitTree(trees[2], 1);
    CRetailAllocator alloc;
    DestroyRange_Tree_0051ac70(trees, trees + 3, alloc);
    if (g_nodesFreed != 9) ok = 0;
    DestroyRange_Tree_0051ac70(trees, trees, alloc);
    if (g_nodesFreed != 9) ok = 0;
    free(trees);
    printf(ok ? "0051ac70_TEST PASS\n" : "0051ac70_TEST FAIL\n");
    return ok ? 0 : 1;
}