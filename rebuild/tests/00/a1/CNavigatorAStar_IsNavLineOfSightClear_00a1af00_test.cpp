// Behaviour test for CNavigatorAStar::IsNavLineOfSightClear (0x00A1AF00): both nodes must exist and be unblocked, then the
// route between way points built from the 2D positions, the node layers and Removable=true decides.
#include <cstdio>
class C3DVector { public: float X, Y, Z; };
class C2DVector { public:
    float X, Y;
    C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) { }
    C2DVector(float x, float y) : X(x), Y(y) { }
};
class CANavQuadTreeNode { public: C2DVector Position; unsigned char NavLayer; bool Blocked; CANavQuadTreeNode() : Position(0.0f, 0.0f), NavLayer(0), Blocked(false) { } };
class CNavWayPoint { public:
    C2DVector Position; unsigned char NavLayer; bool Removable;
    CNavWayPoint(const C2DVector& p, unsigned char layer, bool removable) : Position(p), NavLayer(layer), Removable(removable) { }
};
static CANavQuadTreeNode g_nodes[2];
static bool g_present[2];
static bool g_routeClear;
static int g_routeCalls;
static float g_lastFromX, g_lastToY; static unsigned char g_lastFromLayer, g_lastToLayer; static bool g_lastRem, g_lastFlag;
class CNavigatorManager { public:
    const CANavQuadTreeNode* GetNodeAt(const C2DVector& p, unsigned char level) const
    { int i = p.X < 0.0f ? 0 : 1; (void)level; return g_present[i] ? &g_nodes[i] : 0; }
};
class CNavigatorExternalInterface { public: long GetNavigationLevelAt(const C3DVector& p) const { return p.Z > 0.0f ? 1 : 0; } };
class CNavigatorAStar {
public:
    CNavigatorExternalInterface* PInterface;
    CNavigatorManager*           NavigationManager;
    bool IsRouteClear(const CNavWayPoint& a, const CNavWayPoint& b, bool f) const
    { ++g_routeCalls; g_lastFromX = a.Position.X; g_lastToY = b.Position.Y; g_lastFromLayer = a.NavLayer; g_lastToLayer = b.NavLayer;
      g_lastRem = a.Removable && b.Removable; g_lastFlag = f; return g_routeClear; }
    const CANavQuadTreeNode* GetNodeAt(const C3DVector& pos) const
    { return NavigationManager->GetNodeAt(C2DVector(pos), (unsigned char)PInterface->GetNavigationLevelAt(pos)); }
    bool IsNavLineOfSightClear(const C3DVector& from, const C3DVector& to) const
    {
        const CANavQuadTreeNode* fromNode = GetNodeAt(from);
        if (!fromNode || fromNode->Blocked) return false;
        const CANavQuadTreeNode* toNode = GetNodeAt(to);
        if (!toNode || toNode->Blocked) return false;
        CNavWayPoint start(C2DVector(from.X, from.Y), fromNode->NavLayer, true);
        CNavWayPoint end(C2DVector(to.X, to.Y), toNode->NavLayer, true);
        return IsRouteClear(start, end, false);
    }
};
int main()
{
    CNavigatorManager m; CNavigatorExternalInterface ifc; CNavigatorAStar nav; nav.PInterface = &ifc; nav.NavigationManager = &m;
    C3DVector a = { -1.0f, 2.0f, 0.0f }, b = { 3.0f, 4.0f, 1.0f };
    g_nodes[0].NavLayer = 5; g_nodes[1].NavLayer = 7;
    int bad = 0;
    g_present[0] = false; g_present[1] = true; g_routeClear = true;
    if (nav.IsNavLineOfSightClear(a, b) || g_routeCalls) bad = 1;
    g_present[0] = true; g_nodes[0].Blocked = true;
    if (nav.IsNavLineOfSightClear(a, b) || g_routeCalls) bad = 2;
    g_nodes[0].Blocked = false; g_present[1] = false;
    if (nav.IsNavLineOfSightClear(a, b) || g_routeCalls) bad = 3;
    g_present[1] = true; g_nodes[1].Blocked = true;
    if (nav.IsNavLineOfSightClear(a, b) || g_routeCalls) bad = 4;
    g_nodes[1].Blocked = false;
    if (!nav.IsNavLineOfSightClear(a, b) || g_routeCalls != 1) bad = 5;
    if (g_lastFromX != -1.0f || g_lastToY != 4.0f || g_lastFromLayer != 5 || g_lastToLayer != 7 || !g_lastRem || g_lastFlag) bad = 6;
    g_routeClear = false;
    if (nav.IsNavLineOfSightClear(a, b)) bad = 7;
    if (bad) { std::printf("00a1af00_TEST FAIL\n"); return 1; }
    std::printf("00a1af00_TEST PASS\n");
    return 0;
}