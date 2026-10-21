// Behaviour test for CNavQuadTreeNode::RemoveRegionZero @ 0x00A76060.
#include <cstdio>

static int g_deleted = 0;
class CANavQuadTreeNode {
public:
    bool LeafNode;
    CANavQuadTreeNode(bool leaf) : LeafNode(leaf) {}
    virtual ~CANavQuadTreeNode() {}
    virtual const void* GetRegion() const { return 0; }
    virtual long GetNodeType() const { return 2; }
    virtual bool RemoveRegionZero() { return false; }
};
class CNavBlockedQuadTreeNode : public CANavQuadTreeNode {
public:
    CNavBlockedQuadTreeNode() : CANavQuadTreeNode(true) {}
};
static CNavBlockedQuadTreeNode g_BlockedNavNode;

class CLeaf : public CANavQuadTreeNode {
public:
    const void* region; long type;
    CLeaf(const void* r, long t) : CANavQuadTreeNode(true), region(r), type(t) {}
    virtual const void* GetRegion() const { return region; }
    virtual long GetNodeType() const { return type; }
};
class CBranch : public CANavQuadTreeNode {
public:
    bool allBlocked;
    CBranch(bool b) : CANavQuadTreeNode(false), allBlocked(b) {}
    ~CBranch() { ++g_deleted; }
    virtual long GetNodeType() const { return 0; }
    virtual bool RemoveRegionZero() { return allBlocked; }
};

class CNavQuadTreeNode : public CANavQuadTreeNode {
public:
    CANavQuadTreeNode* TLNode; CANavQuadTreeNode* TRNode; CANavQuadTreeNode* BLNode; CANavQuadTreeNode* BRNode;
    int sets;
    CNavQuadTreeNode() : CANavQuadTreeNode(false), sets(0) {}
    void SetTLNode(CANavQuadTreeNode* n) { TLNode = n; ++sets; }
    void SetTRNode(CANavQuadTreeNode* n) { TRNode = n; ++sets; }
    void SetBLNode(CANavQuadTreeNode* n) { BLNode = n; ++sets; }
    void SetBRNode(CANavQuadTreeNode* n) { BRNode = n; ++sets; }
    virtual bool RemoveRegionZero();
};

bool CNavQuadTreeNode::RemoveRegionZero()
{
    if (TLNode->LeafNode) {
        if (TLNode->GetRegion() == 0 && TLNode->GetNodeType() == 1) SetTLNode(&g_BlockedNavNode);
    } else if (TLNode->RemoveRegionZero()) {
        if (TLNode != &g_BlockedNavNode) delete TLNode;
        TLNode = &g_BlockedNavNode;
    }
    if (TRNode->LeafNode) {
        if (TRNode->GetRegion() == 0 && TRNode->GetNodeType() == 1) SetTRNode(&g_BlockedNavNode);
    } else if (TRNode->RemoveRegionZero()) {
        if (TRNode != &g_BlockedNavNode) delete TRNode;
        TRNode = &g_BlockedNavNode;
    }
    if (BLNode->LeafNode) {
        if (BLNode->GetRegion() == 0 && BLNode->GetNodeType() == 1) SetBLNode(&g_BlockedNavNode);
    } else if (BLNode->RemoveRegionZero()) {
        if (BLNode != &g_BlockedNavNode) delete BLNode;
        BLNode = &g_BlockedNavNode;
    }
    if (BRNode->LeafNode) {
        if (BRNode->GetRegion() == 0 && BRNode->GetNodeType() == 1) SetBRNode(&g_BlockedNavNode);
    } else if (BRNode->RemoveRegionZero()) {
        if (BRNode != &g_BlockedNavNode) delete BRNode;
        BRNode = &g_BlockedNavNode;
    }
    if (TLNode->GetNodeType() == 2 && TRNode->GetNodeType() == 2 && BRNode->GetNodeType() == 2 && BLNode->GetNodeType() == 2)
        return true;
    return false;
}

int main()
{
    int region = 0;
    CLeaf noRegion(0, 1), withRegion(&region, 1), switchable(0, 3);
    CNavQuadTreeNode n;
    n.TLNode = &noRegion; n.TRNode = new CBranch(true); n.BLNode = &g_BlockedNavNode; n.BRNode = &withRegion;
    bool r1 = n.RemoveRegionZero();
    int ok = !r1 && n.TLNode == &g_BlockedNavNode && n.TRNode == &g_BlockedNavNode && g_deleted == 1
          && n.BRNode == &withRegion && n.sets == 1;
    CNavQuadTreeNode m;
    m.TLNode = &noRegion; m.TRNode = &g_BlockedNavNode; m.BLNode = &g_BlockedNavNode; m.BRNode = &noRegion;
    ok = ok && m.RemoveRegionZero() && m.sets == 2;
    CNavQuadTreeNode k;
    CBranch keep(false);
    k.TLNode = &switchable; k.TRNode = &keep; k.BLNode = &g_BlockedNavNode; k.BRNode = &g_BlockedNavNode;
    ok = ok && !k.RemoveRegionZero() && k.TLNode == &switchable && k.TRNode == &keep && g_deleted == 1;
    if (!ok) { std::printf("00a76060_TEST FAIL\n"); return 1; }
    std::printf("00a76060_TEST PASS\n");
    return 0;
}
