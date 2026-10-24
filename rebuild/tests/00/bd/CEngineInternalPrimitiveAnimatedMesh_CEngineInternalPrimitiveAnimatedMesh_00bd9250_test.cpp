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
#include <string.h>
static char g_order[16];
static int g_n = 0;
static void Log(char c) { g_order[g_n++] = c; g_order[g_n] = 0; }
class CEngineInternalPrimitiveMeshBase {
public:
    CEngineInternalPrimitiveMeshBase() { }
    virtual ~CEngineInternalPrimitiveMeshBase() { Log('B'); }
};
class CBaseClass {
public:
    char tag;
    CBaseClass() : tag('x') { }
    virtual void v00() { }
    ~CBaseClass() { Log(tag); }
};
class C3DAnimationInterpolator {
public:
    ~C3DAnimationInterpolator() { Log('A'); }
};
class C3DPositionInterpolator : public CBaseClass { public: C3DPositionInterpolator() { tag = 'P'; } };
class C3DOrientationInterpolator : public CBaseClass { public: C3DOrientationInterpolator() { tag = 'O'; } };
class CCache {
public:
    CTree BoneScalings;
    CCache() { InitTree(BoneScalings, 1); }
    ~CCache() { Log('C'); }
};
class CEngineInternalPrimitiveAnimatedMesh : public CEngineInternalPrimitiveMeshBase {
public:
    CEngineInternalPrimitiveAnimatedMesh() { }
    C3DAnimationInterpolator   Anim;
    C3DPositionInterpolator    WorldPos;
    C3DOrientationInterpolator Orientation;
    CCache                     Cache;
    unsigned char              RenderInfo[0x48];
};
int main()
{
    int ok = 1;
    CEngineInternalPrimitiveMeshBase* mesh = new CEngineInternalPrimitiveAnimatedMesh;
    delete mesh;
    if (strcmp(g_order, "COPAB") != 0) ok = 0;
    if (g_nodesFreed != 4) ok = 0;
    printf(ok ? "00bd9250_TEST PASS\n" : "00bd9250_TEST FAIL\n");
    return ok ? 0 : 1;
}