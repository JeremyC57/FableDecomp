#include <cstdio>
#include <cstdlib>

// Behaviour model of boost::checked_delete<CBulletTimeManager> (0x004AE080): null-tested delete whose implicit destructor
// clears the map<void*, CBulletTimeInfo> (erase from the root when non-empty, header self-linked, count 0) and frees its header node.
struct NodeBase { bool color; NodeBase* parent; NodeBase* left; NodeBase* right; };
static int g_erased = 0, g_freedHeaders = 0, g_deleted = 0;
static void Erase(NodeBase* x) { while (x) { Erase(x->right); NodeBase* l = x->left; std::free(x); ++g_erased; x = l; } }
struct LongSet {
    NodeBase* header; unsigned int count;
    LongSet() : count(0) { header = (NodeBase*)std::malloc(sizeof(NodeBase)); header->color = false; header->parent = 0; header->left = header->right = header; }
    void clear() { if (count != 0) { Erase(header->parent); header->left = header; header->parent = 0; header->right = header; count = 0; } }
    ~LongSet() { clear(); if (header) { std::free(header); ++g_freedHeaders; } }
};
class CBulletTimeManager { public:
    LongSet BulletTimeInfoMap; bool Override; long LastStrikeActionTick;
    static void operator delete(void* p) { ++g_deleted; ::operator delete(p); }
};
template <class T> void checked_delete(T* x) { delete x; }

static NodeBase* MakeNode(NodeBase* parent) { NodeBase* n = (NodeBase*)std::malloc(sizeof(NodeBase)); n->color = true; n->parent = parent; n->left = n->right = 0; return n; }

int main()
{
    checked_delete<CBulletTimeManager>(0);
    if (g_deleted != 0) { std::printf("004ae080_TEST FAIL\n"); return 1; }
    CBulletTimeManager* m = new CBulletTimeManager;
    NodeBase* root = MakeNode(m->BulletTimeInfoMap.header);
    root->left = MakeNode(root); root->right = MakeNode(root);
    m->BulletTimeInfoMap.header->parent = root; m->BulletTimeInfoMap.count = 3;
    checked_delete(m);
    if (g_erased != 3 || g_freedHeaders != 1 || g_deleted != 1) { std::printf("004ae080_TEST FAIL\n"); return 1; }
    CBulletTimeManager* e = new CBulletTimeManager;                    // empty set: no erase walk
    checked_delete(e);
    if (g_erased != 3 || g_freedHeaders != 2 || g_deleted != 2) { std::printf("004ae080_TEST FAIL\n"); return 1; }
    std::printf("004ae080_TEST PASS\n");
    return 0;
}