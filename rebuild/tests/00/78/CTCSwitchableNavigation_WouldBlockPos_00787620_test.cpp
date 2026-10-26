// Behaviour test for 00787620: src/00787620.cpp copied verbatim; the world, navigator and node stubbed so the node
// lookup (XY of the position, layer from the world map) and the blocked query with the parent's 64-bit UID are checked.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCSwitchableNavigation::WouldBlockPos(const C3DVector& pos) -- find the navigation node under the position on its navigation layer and ask it whether this owner's thing blocks it
// Retry 10b (accessor sweep, FIXES B-4/B-5): the parent's UID is read through an inline accessor (`GetParentUID()`), which gives retail's 64-bit argument loads; the plain `ParentThing->UID` read gives the parked difference.
// Leaf: WouldBlockPos
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00787620, length 107 bytes (`ret 4`).
// Callees: CTCBase::GetWorld @ 0x00686A70 (navigator manager at CWorld+0x48), CTCBase::GetWorldMap @ 0x00686D20,
// CWorldMap::GetNavigationLayerAt(const C3DVector&, CThing*) @ 0x0063E590 (null thing), CNavigatorManager::GetNodeAt(const C2DVector&,
// unsigned char layer) @ 0x00A14F70, the node's virtual slot 10 (+0x28) taking the owner's 64-bit UID (CThing+0x18/+0x1C).
// Layout: CTCSwitchableNavigation MapIndex +0x0C (Ego_r.pdb, unused here). CThing UID __int64 at +0x18.
// Shape: the 2D position is a named local built first (`C2DVector p(pos.X, pos.Y)`), the manager fetched before the layer call (kept in
// ebx); the node's answer is returned directly.
class C3DVector { public: float X, Y, Z; };
class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
};
class CThing { public:
    unsigned char _pad00[0x18];
    __int64 UID;                                // +0x18
};
class CANavQuadTreeNode { public:
    virtual void n00(); virtual void n01(); virtual void n02(); virtual void n03(); virtual void n04();
    virtual void n05(); virtual void n06(); virtual void n07(); virtual void n08(); virtual void n09();
    virtual bool WouldBeBlockedBy(__int64 thingUID);   // slot 10 (+0x28)
};
class CNavigatorManager { public:
    CANavQuadTreeNode* GetNodeAt(const C2DVector& position, unsigned char layer);   // 0x00A14F70
};
class CWorld { public:
    unsigned char _pad00[0x48];
    CNavigatorManager* NavigatorManager;        // +0x48
};
class CWorldMap { public:
    long GetNavigationLayerAt(const C3DVector& pos, CThing* thing);   // 0x0063E590
};
class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    __int64 GetParentUID() const { return ParentThing->UID; }
    bool ToBeRemoved;                           // +0x08
    bool IsInGlobalUpdateTCs;                   // +0x09
    CWorld* GetWorld();                         // 0x00686A70
    CWorldMap* GetWorldMap();                   // 0x00686D20
};
class CTCSwitchableNavigation : public CTCBase { public:
    long MapIndex;                              // +0x0c
    bool WouldBlockPos(const C3DVector& pos);
};

bool CTCSwitchableNavigation::WouldBlockPos(const C3DVector& pos)
{
    C2DVector p(pos.X, pos.Y);
    CNavigatorManager* navigator = GetWorld()->NavigatorManager;
    CANavQuadTreeNode* node = navigator->GetNodeAt(p, (unsigned char)GetWorldMap()->GetNavigationLayerAt(pos, 0));
    if (node)
        return node->WouldBeBlockedBy(GetParentUID());
    return false;
}
// ---- end source ----
void CTCBase::v00() {}
void CANavQuadTreeNode::n00() {} void CANavQuadTreeNode::n01() {} void CANavQuadTreeNode::n02() {} void CANavQuadTreeNode::n03() {} void CANavQuadTreeNode::n04() {}
void CANavQuadTreeNode::n05() {} void CANavQuadTreeNode::n06() {} void CANavQuadTreeNode::n07() {} void CANavQuadTreeNode::n08() {} void CANavQuadTreeNode::n09() {}
static __int64 g_askedUID = 0; static bool g_blocked = true;
bool CANavQuadTreeNode::WouldBeBlockedBy(__int64 uid) { g_askedUID = uid; return g_blocked; }
static CANavQuadTreeNode g_node; static bool g_haveNode = true; static float g_px = 0, g_py = 0; static unsigned char g_layer = 0;
CANavQuadTreeNode* CNavigatorManager::GetNodeAt(const C2DVector& p, unsigned char layer) { g_px = p.X; g_py = p.Y; g_layer = layer; return g_haveNode ? &g_node : 0; }
static const C3DVector* g_layerPos = 0; static CThing* g_layerThing = (CThing*)1;
long CWorldMap::GetNavigationLayerAt(const C3DVector& pos, CThing* thing) { g_layerPos = &pos; g_layerThing = thing; return 0x102; }
static CNavigatorManager g_nav; static CWorld g_world; static CWorldMap g_map;
CWorld* CTCBase::GetWorld() { return &g_world; }
CWorldMap* CTCBase::GetWorldMap() { return &g_map; }
int main()
{
    g_world.NavigatorManager = &g_nav;
    static CThing parent; memset(&parent, 0, sizeof(parent)); parent.UID = 0x123456789ABCDEFLL;
    CTCSwitchableNavigation n; memset(&n, 0, sizeof(n)); n.ParentThing = &parent;
    C3DVector pos = { 1.5f, 2.5f, 3.5f };
    CHECK(n.WouldBlockPos(pos) == true);
    CHECK(g_px == 1.5f && g_py == 2.5f && g_layer == 2 && g_layerPos == &pos && g_layerThing == 0 && g_askedUID == 0x123456789ABCDEFLL);
    g_blocked = false; CHECK(n.WouldBlockPos(pos) == false);
    g_haveNode = false; g_blocked = true; g_askedUID = 0;
    CHECK(n.WouldBlockPos(pos) == false && g_askedUID == 0);
    if (fails == 0) printf("00787620_TEST PASS\n"); else printf("00787620_TEST FAIL\n");
    return fails;
}