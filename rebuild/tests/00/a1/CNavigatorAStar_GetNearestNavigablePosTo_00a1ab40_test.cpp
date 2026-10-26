#include <stdio.h>
// Behaviour test for CNavigatorAStar::GetNearestNavigablePosTo (0x00A1AB40).
class C3DVector { public: float X, Y, Z; void SetXY(float x, float y) { X = x; Y = y; } };
class C2DVector { public:
    float X, Y;
    C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) { }
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) { }
    C2DVector() {}
};
class CANavQuadTreeNode { public: virtual void v00() {} C2DVector Position; unsigned char QuadTreeLevel, NavLayer; bool LeafNode, Blocked; };
static CANavQuadTreeNode* g_nodeAt = 0; static float g_gx = 0; static bool g_glayer = false;
class CNavigatorManager { public:
    CANavQuadTreeNode* GetNodeAt(const C2DVector& p, bool layer) { g_gx = p.X; g_glayer = layer; return g_nodeAt; } };
class CNavigatorExternalInterface { public:
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {}
    virtual float FindPosZAt(const C3DVector& pos, bool) const { return pos.X + pos.Y; }
    virtual void v05() {} virtual void v06() {} virtual void v07() {} virtual void v08() {} virtual void v09() {} virtual void v10() {} virtual void v11() {}
    virtual void v12() {} virtual void v13() {} virtual void v14() {} virtual void v15() {}
    virtual bool GetLayerAt(const C3DVector&) const { return true; } };
static CANavQuadTreeNode* g_nearest = 0; static const CANavQuadTreeNode* g_from = 0; static float g_radius = 0;
class CNavigatorBase { public:
    virtual void v00() {}
    unsigned char _pad04[0x10]; CNavigatorExternalInterface* PInterface; bool OnRoute, Interrupted;
    CNavigatorManager* NavigationManager; void* SteeringAgent;
    CANavQuadTreeNode* FindNearestNavigableNode(const CANavQuadTreeNode* from, const CANavQuadTreeNode*, const C3DVector&, float radius, CNavigatorManager*) const
    { g_from = from; g_radius = radius; return g_nearest; } };
class CNavigatorAStar : public CNavigatorBase { public:
    unsigned char _pad24[0x64]; CANavQuadTreeNode* LastNavigableNodeVisited;
    virtual bool GetNearestNavigablePosTo(const C3DVector& pos, float radius, C3DVector& out) const; };
bool CNavigatorAStar::GetNearestNavigablePosTo(const C3DVector& pos, float radius, C3DVector& out) const
{
    CANavQuadTreeNode* node = NavigationManager->GetNodeAt(pos, PInterface->GetLayerAt(pos));
    if (node) {
        if (!node->Blocked) { out = pos; return true; }
        CANavQuadTreeNode* nearest = FindNearestNavigableNode(LastNavigableNodeVisited, node, pos, radius, NavigationManager);
        if (nearest) {
            C2DVector p = nearest->Position;
            out.SetXY(p.X, p.Y);
            out.Z = PInterface->FindPosZAt(out, true);
            return true;
        }
    }
    return false;
}
int main()
{
    int ok = 1;
    CNavigatorExternalInterface iface; CNavigatorManager mgr; CNavigatorAStar a;
    a.PInterface = &iface; a.NavigationManager = &mgr; CANavQuadTreeNode last; a.LastNavigableNodeVisited = &last;
    C3DVector pos = { 1.0f, 2.0f, 3.0f }, out = { 0, 0, 0 };
    g_nodeAt = 0;
    if (a.GetNearestNavigablePosTo(pos, 5.0f, out) || g_gx != 1.0f || !g_glayer) ok = 0;
    CANavQuadTreeNode n; n.Blocked = false; g_nodeAt = &n;
    if (!a.GetNearestNavigablePosTo(pos, 5.0f, out) || out.X != 1.0f || out.Z != 3.0f) ok = 0;
    n.Blocked = true; g_nearest = 0;
    if (a.GetNearestNavigablePosTo(pos, 5.0f, out) || g_from != &last || g_radius != 5.0f) ok = 0;
    CANavQuadTreeNode m; m.Position.X = 10.0f; m.Position.Y = 20.0f; g_nearest = &m;
    if (!a.GetNearestNavigablePosTo(pos, 5.0f, out) || out.X != 10.0f || out.Y != 20.0f || out.Z != 30.0f) ok = 0;
    if (ok) { printf("00a1ab40_TEST PASS\n"); return 0; }
    printf("00a1ab40_TEST FAIL\n"); return 1;
}