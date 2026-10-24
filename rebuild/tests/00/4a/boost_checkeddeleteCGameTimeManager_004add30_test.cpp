#include <cstdio>
#include <cstdlib>

// Behaviour model of boost::checked_delete<CGameTimeManager> (0x004ADD30): null-tested delete whose implicit destructor
// clears the set<long> (erase from the root when non-empty, header self-linked, count 0) and frees its header node.
struct NodeBase { bool color; NodeBase* parent; NodeBase* left; NodeBase* right; };
static int g_erased = 0, g_freedHeaders = 0, g_deleted = 0;
static void Erase(NodeBase* x) { while (x) { Erase(x->right); NodeBase* l = x->left; std::free(x); ++g_erased; x = l; } }
struct LongSet {
    NodeBase* header; unsigned int count;
    LongSet() : count(0) { header = (NodeBase*)std::malloc(sizeof(NodeBase)); header->color = false; header->parent = 0; header->left = header->right = header; }
    void clear() { if (count != 0) { Erase(header->parent); header->left = header; header->parent = 0; header->right = header; count = 0; } }
    ~LongSet() { clear(); if (header) { std::free(header); ++g_freedHeaders; } }
};
struct CGameTimeManager {
    void* World; float Time; LongSet StopTimeIndexes; long NextStopTimeIndex;
    static void operator delete(void* p) { ++g_deleted; ::operator delete(p); }
};
template <class T> void checked_delete(T* x) { delete x; }

static NodeBase* MakeNode(NodeBase* parent) { NodeBase* n = (NodeBase*)std::malloc(sizeof(NodeBase)); n->color = true; n->parent = parent; n->left = n->right = 0; return n; }

int main()
{
    checked_delete<CGameTimeManager>(0);
    if (g_deleted != 0) { std::printf("004add30_TEST FAIL\n"); return 1; }
    CGameTimeManager* m = new CGameTimeManager;
    NodeBase* root = MakeNode(m->StopTimeIndexes.header);
    root->left = MakeNode(root); root->right = MakeNode(root);
    m->StopTimeIndexes.header->parent = root; m->StopTimeIndexes.count = 3;
    checked_delete(m);
    if (g_erased != 3 || g_freedHeaders != 1 || g_deleted != 1) { std::printf("004add30_TEST FAIL\n"); return 1; }
    CGameTimeManager* e = new CGameTimeManager;                    // empty set: no erase walk
    checked_delete(e);
    if (g_erased != 3 || g_freedHeaders != 2 || g_deleted != 2) { std::printf("004add30_TEST FAIL\n"); return 1; }
    std::printf("004add30_TEST PASS\n");
    return 0;
}