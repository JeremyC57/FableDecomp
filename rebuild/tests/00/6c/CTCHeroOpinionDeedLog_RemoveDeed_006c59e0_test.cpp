// Behaviour test for 0x006C59E0 CTCHeroOpinionDeedLog::RemoveDeed: a present handle deletes its deed (running the
// PTarget destructor), unlinks the node through the rebalance hook, frees it and drops the count; an absent handle
// leaves the map untouched.
#include <cstdio>
#include <cstdlib>

static int g_targetDtors = 0;
static int g_rebalanceCalls = 0;

struct CBaseIntelligentPointer { void* m_pItem; ~CBaseIntelligentPointer() { ++g_targetDtors; } };
class COpinionDeed { public: long DeedType; long DeedHandle; float Pos[3]; long FrameHappened; CBaseIntelligentPointer PTarget; long ExtraData; };

struct NodeBase { unsigned char colour; NodeBase* parent; NodeBase* left; NodeBase* right; };
struct Node : NodeBase { long first; COpinionDeed* second; };

// Test rebalance: the tree only ever holds a root with no children here.
static NodeBase* Rebalance(NodeBase* z, NodeBase*& root, NodeBase*& leftmost, NodeBase*& rightmost)
{
    ++g_rebalanceCalls;
    root = 0; leftmost = 0; rightmost = 0;
    return z;
}

struct Map {
    Node* header; unsigned long count;
    Node* find(const long& key)
    {
        Node* r = header; Node* n = (Node*)header->parent;
        while (n) { if (!(n->first < key)) { r = n; n = (Node*)n->left; } else n = (Node*)n->right; }
        if (r == header || key < r->first) r = header;
        return r;
    }
};

struct DeedLog {
    Map OpinionDeedMap;
    void RemoveDeed(long handle)
    {
        Node* it = OpinionDeedMap.find(handle);
        if (it != OpinionDeedMap.header) {
            delete it->second;
            Node* y = (Node*)Rebalance(it, OpinionDeedMap.header->parent, OpinionDeedMap.header->left, OpinionDeedMap.header->right);
            if (y) std::free(y);
            --OpinionDeedMap.count;
        }
    }
};

int main()
{
    Node header; header.colour = 0; header.parent = 0; header.left = header.right = &header;
    Node* n = (Node*)std::malloc(sizeof(Node));
    n->colour = 1; n->parent = &header; n->left = n->right = 0; n->first = 7; n->second = new COpinionDeed;
    header.parent = n; header.left = header.right = n;
    DeedLog log; log.OpinionDeedMap.header = &header; log.OpinionDeedMap.count = 1;

    log.RemoveDeed(3);                                   // absent
    if (g_targetDtors != 0 || g_rebalanceCalls != 0 || log.OpinionDeedMap.count != 1) { std::printf("006c59e0_TEST FAIL\n"); return 1; }
    log.RemoveDeed(7);                                   // present
    if (g_targetDtors != 1 || g_rebalanceCalls != 1 || log.OpinionDeedMap.count != 0 || header.parent != 0) { std::printf("006c59e0_TEST FAIL\n"); return 1; }
    std::printf("006c59e0_TEST PASS\n");
    return 0;
}