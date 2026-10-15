// Behaviour test for 0x00BF6750 (CLandscapeBackgroundTreeNode::InvalidateEdgeTesellation): four edge invalidations around the node rectangle.
#include <cstdio>
struct Call { long x, y, len, side; };
struct Map { unsigned short OriginX, OriginY; Call calls[4]; int n; void InvalidateEdge(long x, long y, long l, long s) { Call c = { x, y, l, s }; calls[n++] = c; } };
struct Node {
    Map* map; unsigned short MapX, MapY, Width, Height; bool TesselationValid;
    void Invalidate()
    {
        long x = MapX + map->OriginX;
        long y = MapY + map->OriginY;
        map->InvalidateEdge(x + Width, y, Height, 0);
        map->InvalidateEdge(x, y, Height, 1);
        map->InvalidateEdge(x, y + Height, Width, 2);
        map->InvalidateEdge(x, y, Width, 3);
        TesselationValid = false;
    }
};
int main()
{
    Map m; m.n = 0; m.OriginX = 100; m.OriginY = 200;
    Node n; n.map = &m; n.MapX = 10; n.MapY = 20; n.Width = 4; n.Height = 8; n.TesselationValid = true;
    n.Invalidate();
    int ok = m.n == 4 && !n.TesselationValid
        && m.calls[0].x == 114 && m.calls[0].y == 220 && m.calls[0].len == 8 && m.calls[0].side == 0
        && m.calls[1].x == 110 && m.calls[1].y == 220 && m.calls[1].len == 8 && m.calls[1].side == 1
        && m.calls[2].x == 110 && m.calls[2].y == 228 && m.calls[2].len == 4 && m.calls[2].side == 2
        && m.calls[3].x == 110 && m.calls[3].y == 220 && m.calls[3].len == 4 && m.calls[3].side == 3;
    if (!ok) { std::printf("00bf6750_TEST FAIL\n"); return 1; }
    std::printf("00bf6750_TEST PASS\n");
    return 0;
}