#include <cstdio>
// Behaviour test for CNavQuadTree::IsActionPointReachable (0x00A77570): a missing leaf is unreachable, an unblocked leaf is
// reachable, and a blocked leaf is reachable only when an unblocked leaf sits on the ring a quarter unit outside its square.
struct C2DVector { float X, Y; };
struct CNavigationPoint { C2DVector Position; int Layer; };
struct CNavLeaf { C2DVector Centre; unsigned char Level; bool Blocked; };
static const float g_navNodeSizes[4] = { 1.0f, 2.0f, 4.0f, 8.0f };
struct CNavQuadTree {
    CNavLeaf home;      // the node at the point
    CNavLeaf open;      // returned for probes inside the open box
    int mode;           // 0: no node, 1: home only, 2: home plus an open box
    float boxX0, boxX1, boxY0, boxY1;
    int probes;
    CNavLeaf* GetNodeAt(const C2DVector* p, int layer)
    {
        if (mode == 0) return 0;
        if (p->X == 10.0f && p->Y == 20.0f) return &home;
        ++probes;
        if (mode == 2 && p->X >= boxX0 && p->X <= boxX1 && p->Y >= boxY0 && p->Y <= boxY1) return &open;
        return &home;
    }
    bool IsActionPointReachable(const CNavigationPoint& point)
    {
        CNavLeaf* node = GetNodeAt(&point.Position, point.Layer);
        if (!node) return false;
        if (!node->Blocked) return true;
        C2DVector centre = node->Centre;
        float halfSize = g_navNodeSizes[node->Level] * 0.5f;
        float reach = halfSize + 0.25f;
        float x = centre.X - reach;
        float endX = centre.X + reach;
        for (; x <= endX; x += 0.5f)
            for (int side = -1; side <= 1; side += 2) {
                C2DVector probe; probe.X = x; probe.Y = side * reach + centre.Y;
                CNavLeaf* leaf = GetNodeAt(&probe, point.Layer);
                if (leaf && !leaf->Blocked) return true;
            }
        float y = centre.Y - halfSize + 0.25f;
        float endY = centre.Y + reach;
        for (; y < endY; y += 0.5f)
            for (int side = -1; side <= 1; side += 2) {
                C2DVector probe; probe.Y = y; probe.X = side * reach + centre.X;
                CNavLeaf* leaf = GetNodeAt(&probe, point.Layer);
                if (leaf && !leaf->Blocked) return true;
            }
        return false;
    }
};
int main()
{
    CNavigationPoint pt; pt.Position.X = 10.0f; pt.Position.Y = 20.0f; pt.Layer = 0;
    CNavQuadTree t;
    t.home.Centre.X = 10.0f; t.home.Centre.Y = 20.0f; t.home.Level = 1; t.home.Blocked = true;
    t.open.Blocked = false; t.probes = 0;
    int bad = 0;
    t.mode = 0; bad |= t.IsActionPointReachable(pt) != false;
    t.mode = 1; t.home.Blocked = false; bad |= t.IsActionPointReachable(pt) != true;
    // blocked everywhere: halfSize 1, reach 1.25 -> x in [8.75,11.25] step .5 = 6 columns x2, y in [19.25,21.25) = 4 rows x2
    t.home.Blocked = true; t.probes = 0; bad |= t.IsActionPointReachable(pt) != false; bad |= t.probes != 20;
    // an open leaf on the right-hand side (x = 11.25) of the rows only
    t.mode = 2; t.boxX0 = 11.0f; t.boxX1 = 12.0f; t.boxY0 = 19.5f; t.boxY1 = 20.0f; t.probes = 0;
    bad |= t.IsActionPointReachable(pt) != true;
    // the top-left ring corner column (x = 8.75, y = 18.75) hits on the very first probe
    t.boxX0 = 8.0f; t.boxX1 = 9.0f; t.boxY0 = 18.0f; t.boxY1 = 19.0f; t.probes = 0;
    bad |= t.IsActionPointReachable(pt) != true; bad |= t.probes != 1;
    if (bad) { std::printf("00a77570_TEST FAIL\n"); return 1; }
    std::printf("00a77570_TEST PASS\n");
    return 0;
}