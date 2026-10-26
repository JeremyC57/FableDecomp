#include <cstdio>
// Behaviour test for 00a1bee0 CNavigatorAStar::UpdateLastPosition: no lookup while the last node still covers the position;
// an open node becomes the last node (with its layer); a blocked node only seeds the nearest-node search (radius 4 cells)
// when there is no last node yet.
class CANavQuadTreeNode;
static float g_radius = 0; static const CANavQuadTreeNode* g_from = 0; static const CANavQuadTreeNode* g_blocked = 0;
static CANavQuadTreeNode* g_nearest = 0;
class C3DVector { public: float X, Y, Z; };
class C2DVector { public:
    float X, Y;
    C2DVector() { }
    C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) { }   // the conversion temporary: two dwords copied into the frame
};
class C2DBoxF { public:
    float MinX, MinY, MaxX, MaxY;
    C2DBoxF(float minX, float minY, float maxX, float maxY) : MinX(minX), MinY(minY), MaxX(maxX), MaxY(maxY) {}
    bool ContainsPoint(const C2DVector& p) const { return p.X >= MinX && p.X < MaxX && p.Y >= MinY && p.Y < MaxY; }
};
class CANavQuadTreeNode {
public:
    virtual void v00() {}
    C2DVector     Position;                        // +0x04
    unsigned char QuadTreeLevel;                   // +0x0c
    unsigned char NavLayer;                        // +0x0d
    bool          LeafNode;                        // +0x0e
    bool          Blocked;                         // +0x0f
    C2DBoxF GetAreaCovering() const { return C2DBoxF(Position.X - 1, Position.Y - 1, Position.X + 1, Position.Y + 1); }
    float GetSize() const { return 2.0f; }
};
class CNavigatorManager {
public:
    CANavQuadTreeNode* result; int calls;
    CANavQuadTreeNode* GetNodeAt(const C2DVector& position, bool layer) { ++calls; return result; }
};
class CNavigatorExternalInterface {
public:
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {}
    virtual float FindPosZAt(const C3DVector& pos, bool useNavigation) const { return 0; }
    virtual void v05() {} virtual void v06() {} virtual void v07() {} virtual void v08() {} virtual void v09() {} virtual void v10() {} virtual void v11() {}
    virtual void v12() {} virtual void v13() {} virtual void v14() {} virtual void v15() {}
    virtual bool GetLayerAt(const C3DVector& position) const { return false; }
};
class CNavigatorBase {
public:
    virtual void v00() {}                            // +0x00 vptr
    unsigned char                _pad04[0x10];     // CClass registration
    CNavigatorExternalInterface* PInterface;       // +0x14
    bool                         OnRoute;          // +0x18
    bool                         Interrupted;      // +0x19
    CNavigatorManager*           NavigationManager;   // +0x1c
    void*                        SteeringAgent;    // +0x20
    CANavQuadTreeNode* FindNearestNavigableNode(const CANavQuadTreeNode* from, const CANavQuadTreeNode* blocked, const C3DVector& pos, float radius, CNavigatorManager* manager) const
    { g_radius = radius; g_from = from; g_blocked = blocked; return g_nearest; }
};
class CNavigatorAStar : public CNavigatorBase {
public:
    unsigned char      _pad24[0x64];
    CANavQuadTreeNode* LastNavigableNodeVisited;   // +0x88
    unsigned char      _pad8c[0x2c];
    long               LastKnownNavLayer;          // +0xb8
    void UpdateLastPosition(const C3DVector& pos);
};

void CNavigatorAStar::UpdateLastPosition(const C3DVector& pos)
{
    if (!Interrupted && LastNavigableNodeVisited && LastNavigableNodeVisited->GetAreaCovering().ContainsPoint(pos))
        return;
    CANavQuadTreeNode* node = NavigationManager->GetNodeAt(pos, PInterface->GetLayerAt(pos));
    if (node && node != LastNavigableNodeVisited) {
        if (!node->Blocked) {
            LastNavigableNodeVisited = node;
            LastKnownNavLayer = node->NavLayer;
            return;
        }
        if (!LastNavigableNodeVisited) {
            CANavQuadTreeNode* nearest = FindNearestNavigableNode(node, node, pos, node->GetSize() * 4.0f, NavigationManager);
            if (nearest) {
                LastNavigableNodeVisited = nearest;
                LastKnownNavLayer = nearest->NavLayer;
            }
        }
    }
}

static C3DVector V(float x, float y) { C3DVector v; v.X = x; v.Y = y; v.Z = 0; return v; }
static void Node(CANavQuadTreeNode& n, float x, float y, unsigned char layer, bool blocked) { n.Position.X = x; n.Position.Y = y; n.NavLayer = layer; n.Blocked = blocked; }
int main()
{
    static CNavigatorAStar nav;
    CNavigatorExternalInterface iface; CNavigatorManager mgr; mgr.calls = 0; mgr.result = 0;
    nav.PInterface = &iface; nav.NavigationManager = &mgr; nav.Interrupted = false;
    CANavQuadTreeNode last, open, blocked, nearest;
    Node(last, 0, 0, 1, false); Node(open, 10, 10, 2, false); Node(blocked, 20, 20, 3, true); Node(nearest, 30, 30, 4, false);
    nav.LastNavigableNodeVisited = &last; nav.LastKnownNavLayer = 1;
    nav.UpdateLastPosition(V(0.5f, 0.5f));                       // still inside the last node: no lookup
    bool ok = mgr.calls == 0 && nav.LastNavigableNodeVisited == &last;
    mgr.result = &open;
    nav.UpdateLastPosition(V(10.0f, 10.0f));                     // outside: the open node is taken
    ok = ok && mgr.calls == 1 && nav.LastNavigableNodeVisited == &open && nav.LastKnownNavLayer == 2;
    nav.Interrupted = true; mgr.result = &open;
    nav.UpdateLastPosition(V(10.0f, 10.0f));                     // interrupted: always looks up; same node: no change
    ok = ok && mgr.calls == 2 && nav.LastNavigableNodeVisited == &open;
    nav.Interrupted = false; mgr.result = &blocked; g_nearest = &nearest;
    nav.UpdateLastPosition(V(20.0f, 20.0f));                     // blocked with a last node: kept
    ok = ok && nav.LastNavigableNodeVisited == &open && g_from == 0;
    nav.LastNavigableNodeVisited = 0;
    nav.UpdateLastPosition(V(20.0f, 20.0f));                     // blocked, no last node: nearest search
    ok = ok && nav.LastNavigableNodeVisited == &nearest && nav.LastKnownNavLayer == 4 && g_radius == 8.0f && g_from == &blocked && g_blocked == &blocked;
    if (!ok) { std::printf("00a1bee0_TEST FAIL\n"); return 1; }
    std::printf("00a1bee0_TEST PASS\n");
    return 0;
}