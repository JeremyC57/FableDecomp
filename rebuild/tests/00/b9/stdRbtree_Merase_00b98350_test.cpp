// behaviour test for 0x00B98350 (STLport red-black tree model)
#include <cstdio>
#include <cstdlib>

struct NodeBase { bool color; NodeBase* parent; NodeBase* left; NodeBase* right; };
static int g_freed = 0, g_innerErased = 0;
static void Free(void* p) { ++g_freed; std::free(p); }

struct InnerTree {                               // map<long, CCountedPointer<CEngineSceneMap> >
    NodeBase* header; unsigned count;
    void Erase(NodeBase* root) { if (root) ++g_innerErased; }   // stand-in for 0x00B97580
    ~InnerTree()
    {
        if (count != 0) { Erase(header->parent); header->left = header; header->parent = 0; header->right = header; count = 0; }
        if (header) Free(header);
    }
};
struct OuterNode : NodeBase { long key; InnerTree maps; };

static void EraseOuter(OuterNode* x)
{
    while (x != 0) {
        EraseOuter((OuterNode*)x->right);
        OuterNode* y = (OuterNode*)x->left;
        x->maps.~InnerTree();
        Free(x);
        x = y;
    }
}

static OuterNode* MakeNode(long key, unsigned innerCount)
{
    OuterNode* n = (OuterNode*)std::malloc(sizeof(OuterNode));
    n->key = key; n->left = n->right = n->parent = 0;
    n->maps.header = (NodeBase*)std::malloc(sizeof(NodeBase));
    n->maps.header->parent = innerCount ? n->maps.header : 0;
    n->maps.count = innerCount;
    return n;
}

int main()
{
    OuterNode* a = MakeNode(1, 2);
    OuterNode* b = MakeNode(0, 0);
    OuterNode* c = MakeNode(2, 1);
    a->left = b; a->right = c; b->parent = a; c->parent = a;
    EraseOuter(a);
    if (g_freed != 6 || g_innerErased != 2) { std::printf("00b98350_TEST FAIL\n"); return 1; }
    EraseOuter(0);
    if (g_freed != 6) { std::printf("00b98350_TEST FAIL\n"); return 1; }
    std::printf("00b98350_TEST PASS\n");
    return 0;
}