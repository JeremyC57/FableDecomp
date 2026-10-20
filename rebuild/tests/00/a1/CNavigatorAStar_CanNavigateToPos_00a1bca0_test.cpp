// Behaviour test for 0x00A1BCA0 CNavigatorAStar::CanNavigateToPos: false without nodes; blocked start replaced by the
// last navigable node (when asked) or the nearest navigable one; blocked end replaced by the nearest one that still
// reaches the radius; true only for unblocked nodes in connected regions.
#include <cstdio>

struct C2DVector { float X, Y; C2DVector() {} C2DVector(float x, float y) : X(x), Y(y) {} };
struct C3DVector { float X, Y, Z; };
struct CNavRegion { int group; bool IsConnectedTo(const CNavRegion* o) const { return group == o->group; } };
struct CNode { bool Blocked; CNavRegion* region; CNavRegion* GetRegion() const { return region; } };
static CNode* g_nodeAt[4]; static CNode* g_nearest; static bool g_reaches; static int g_findCalls; static float g_lastFindDist;
struct CNavigatorAStar {
    CNode* LastNavigableNodeVisited;
    CNode* GetNodeAt(const C2DVector& p) const { return g_nodeAt[(int)p.X]; }
    CNode* FindNearestNavigableNode(CNode*, CNode*, const C3DVector&, float d) const { ++g_findCalls; g_lastFindDist = d; return g_nearest; }
    bool IsAnyPartOfNodeWithinDistOfPos(CNode*, float, const C3DVector&, C2DVector*) const { return g_reaches; }
    bool CanNavigateToPos(const C3DVector& from, const C3DVector& to, float radius, bool useLastNavigable) const
    {
        C2DVector from2d(from.X, from.Y);
        CNode* startNode = GetNodeAt(from2d);
        if (!startNode) return false;
        if (startNode->Blocked) {
            if (useLastNavigable && LastNavigableNodeVisited) startNode = LastNavigableNodeVisited;
            else startNode = FindNearestNavigableNode(startNode, startNode, from, 4.0f);
            if (!startNode) return false;
        }
        C2DVector to2d(to.X, to.Y);
        CNode* endNode = GetNodeAt(to2d);
        if (!endNode) return false;
        if (endNode->Blocked) {
            C2DVector nearest;
            endNode = FindNearestNavigableNode(startNode, endNode, to, radius);
            if (!endNode) return false;
            if (!IsAnyPartOfNodeWithinDistOfPos(endNode, radius, to, &nearest)) return false;
        }
        return !endNode->Blocked && !startNode->Blocked && startNode->GetRegion()->IsConnectedTo(endNode->GetRegion());
    }
};
static int Fail(int n) { std::printf("00a1bca0_TEST FAIL %d\n", n); return 1; }
int main()
{
    CNavRegion a = { 1 }, b = { 1 }, c = { 2 };
    CNode open1 = { false, &a }, open2 = { false, &b }, island = { false, &c }, blocked = { true, &a };
    CNavigatorAStar nav; nav.LastNavigableNodeVisited = 0;
    C3DVector p0 = { 0, 0, 0 }, p1 = { 1, 0, 0 }, p2 = { 2, 0, 0 }, p3 = { 3, 0, 0 };
    g_nodeAt[0] = &open1; g_nodeAt[1] = &open2; g_nodeAt[2] = &island; g_nodeAt[3] = &blocked;
    if (!nav.CanNavigateToPos(p0, p1, 1.0f, false)) return Fail(1);
    if (nav.CanNavigateToPos(p0, p2, 1.0f, false)) return Fail(2);             // other region
    g_nearest = &open2; if (!nav.CanNavigateToPos(p3, p1, 1.0f, false) || g_findCalls != 1 || g_lastFindDist != 4.0f) return Fail(3);
    nav.LastNavigableNodeVisited = &open1; if (!nav.CanNavigateToPos(p3, p1, 1.0f, true) || g_findCalls != 1) return Fail(4);
    g_reaches = false; if (nav.CanNavigateToPos(p0, p3, 2.0f, false) || g_lastFindDist != 2.0f) return Fail(5);
    g_reaches = true; if (!nav.CanNavigateToPos(p0, p3, 2.0f, false)) return Fail(6);
    g_nearest = 0; if (nav.CanNavigateToPos(p3, p1, 1.0f, false)) return Fail(7);
    g_nodeAt[1] = 0; if (nav.CanNavigateToPos(p0, p1, 1.0f, false)) return Fail(8);
    std::printf("00a1bca0_TEST PASS\n");
    return 0;
}