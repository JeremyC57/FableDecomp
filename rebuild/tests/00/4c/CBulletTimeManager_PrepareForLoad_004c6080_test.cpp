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
class CBulletTimeManager {
public:
    CTree BulletTimeInfoMap;
    bool  Override;
    bool  UseDefaultMultiplier;
    long  LastStrikeActionTick;
    void PrepareForLoad();
};
void CBulletTimeManager::PrepareForLoad()
{
    Override = false;
    UseDefaultMultiplier = true;
    LastStrikeActionTick = 0;
    BulletTimeInfoMap.clear();
}
int main()
{
    int ok = 1;
    CBulletTimeManager* m = new CBulletTimeManager;
    InitTree(m->BulletTimeInfoMap, 1);
    m->Override = true; m->UseDefaultMultiplier = false; m->LastStrikeActionTick = 99;
    m->PrepareForLoad();
    if (m->Override != false || m->UseDefaultMultiplier != true || m->LastStrikeActionTick != 0) ok = 0;
    if (m->BulletTimeInfoMap.size != 0 || g_nodesFreed != 3) ok = 0;
    if (m->BulletTimeInfoMap.head->left != m->BulletTimeInfoMap.head || m->BulletTimeInfoMap.head->parent != 0) ok = 0;
    m->PrepareForLoad();
    if (g_nodesFreed != 3) ok = 0;
    delete m;
    if (g_nodesFreed != 4) ok = 0;
    printf(ok ? "004c6080_TEST PASS\n" : "004c6080_TEST FAIL\n");
    return ok ? 0 : 1;
}