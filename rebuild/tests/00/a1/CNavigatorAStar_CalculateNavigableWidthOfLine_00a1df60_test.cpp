#include <stdio.h>
// Behaviour test for CNavigatorAStar::CalculateNavigableWidthOfLine(const C3DVector&, const C3DVector&, float&, float&) (0x00A1DF60).
class C3DVector { public: float X, Y, Z; };
class C2DVector { public:
    float X, Y;
    C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) { }
};
class CANavQuadTreeNode { public: virtual void v00() {} C2DVector Position; unsigned char QuadTreeLevel, NavLayer; bool LeafNode, Blocked;
    CANavQuadTreeNode() : Position(0, 0) {} };
static CANavQuadTreeNode* g_nodes[2]; static int g_call = 0;
class CNavigatorManager { public:
    CANavQuadTreeNode* GetNodeAt(const C2DVector& p, bool layer) { return (p.X < 5.0f) ? g_nodes[0] : g_nodes[1]; } };
class CNavigatorExternalInterface { public:
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {}
    virtual float FindPosZAt(const C3DVector&, bool) const { return 0; }
    virtual void v05() {} virtual void v06() {} virtual void v07() {} virtual void v08() {} virtual void v09() {} virtual void v10() {} virtual void v11() {}
    virtual void v12() {} virtual void v13() {} virtual void v14() {} virtual void v15() {}
    virtual bool GetLayerAt(const C3DVector&) const { return false; } };
class CNavWayPoint { public:
    C2DVector Position; unsigned char NavLayer; bool Removable;
    CNavWayPoint(const C2DVector& p, unsigned char layer, bool removable) : Position(p), NavLayer(layer), Removable(removable) { }
};
static CNavWayPoint g_a(C2DVector(0, 0), 0, false), g_b(C2DVector(0, 0), 0, false); static bool g_flag = false;
class CNavigatorBase { public:
    virtual void v00() {}
    unsigned char _pad04[0x10]; CNavigatorExternalInterface* PInterface; bool OnRoute, Interrupted;
    CNavigatorManager* NavigationManager; void* SteeringAgent; };
class CNavigatorAStar : public CNavigatorBase { public:
    void CalculateNavigableWidthOfLine(const CNavWayPoint& from, const CNavWayPoint& to, float& w1, float& w2, bool b)
    { g_a = from; g_b = to; g_flag = b; w1 = 1.5f; w2 = 2.5f; }
    virtual void CalculateNavigableWidthOfLine(const C3DVector& from, const C3DVector& to, float& width1, float& width2); };
void CNavigatorAStar::CalculateNavigableWidthOfLine(const C3DVector& from, const C3DVector& to, float& width1, float& width2)
{
    long fromLayer = 0;
    long toLayer = 0;
    CANavQuadTreeNode* fromNode = NavigationManager->GetNodeAt(from, PInterface->GetLayerAt(from));
    if (fromNode)
        fromLayer = fromNode->NavLayer;
    CANavQuadTreeNode* toNode = NavigationManager->GetNodeAt(to, PInterface->GetLayerAt(to));
    if (toNode)
        toLayer = toNode->NavLayer;
    {
    CNavWayPoint start(C2DVector(from.X, from.Y), (unsigned char)fromLayer, true);
    CNavWayPoint end(C2DVector(to.X, to.Y), (unsigned char)toLayer, true);
    CalculateNavigableWidthOfLine(start, end, width1, width2, true);
    }
}
int main()
{
    int ok = 1;
    CNavigatorExternalInterface iface; CNavigatorManager mgr; CNavigatorAStar a; a.PInterface = &iface; a.NavigationManager = &mgr;
    CANavQuadTreeNode n0, n1; n0.NavLayer = 3; n1.NavLayer = 7; g_nodes[0] = &n0; g_nodes[1] = &n1;
    C3DVector f = { 1.0f, 2.0f, 0 }, t = { 9.0f, 8.0f, 0 }; float w1 = 0, w2 = 0;
    a.CalculateNavigableWidthOfLine(f, t, w1, w2);
    if (g_a.Position.X != 1.0f || g_a.Position.Y != 2.0f || g_a.NavLayer != 3 || !g_a.Removable) ok = 0;
    if (g_b.Position.X != 9.0f || g_b.Position.Y != 8.0f || g_b.NavLayer != 7 || !g_b.Removable || !g_flag || w1 != 1.5f || w2 != 2.5f) ok = 0;
    g_nodes[1] = 0;
    a.CalculateNavigableWidthOfLine(f, t, w1, w2);
    if (g_b.NavLayer != 0 || g_a.NavLayer != 3) ok = 0;
    if (ok) { printf("00a1df60_TEST PASS\n"); return 0; }
    printf("00a1df60_TEST FAIL\n"); return 1;
}