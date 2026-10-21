#include <cstdio>
// Behaviour test for CLandscapeBackgroundPatch::InvalidateEdge(x, y, length, side) (0x00BE63F0).
enum LANDSCAPE_EDGE_STRIP_SIDE { LESS_LEFT = 0, LESS_RIGHT = 1, LESS_TOP = 2, LESS_BOTTOM = 3 };
struct CLandscapeMapOrigin { unsigned short OriginX, OriginY; };
struct CEngineLandscapeMap { CLandscapeMapOrigin* PSection; };
struct CLandscapeBackgroundTreeNode { CEngineLandscapeMap* Map; };
struct CPatchEdges {
    CLandscapeBackgroundTreeNode* Parent;
    unsigned short MapX, MapY, Width, Height;
    int lastInvalidated;
    void InvalidateEdge(LANDSCAPE_EDGE_STRIP_SIDE side) { lastInvalidated = side; }
    void InvalidateEdge(long x, long y, long length, LANDSCAPE_EDGE_STRIP_SIDE side)
    {
        switch (side) {
        case LESS_LEFT:
            if (x == MapX + Parent->Map->PSection->OriginX && y < MapY + Height + Parent->Map->PSection->OriginY && y + length > MapY + Parent->Map->PSection->OriginY)
                InvalidateEdge(LESS_LEFT);
            break;
        case LESS_RIGHT:
            if (x == MapX + Width + Parent->Map->PSection->OriginX && y < MapY + Height + Parent->Map->PSection->OriginY && y + length > MapY + Parent->Map->PSection->OriginY)
                InvalidateEdge(LESS_RIGHT);
            break;
        case LESS_TOP:
            if (y == MapY + Parent->Map->PSection->OriginY && x < MapX + Width + Parent->Map->PSection->OriginX && x + length > MapX + Parent->Map->PSection->OriginX)
                InvalidateEdge(LESS_TOP);
            break;
        case LESS_BOTTOM:
            if (y == MapY + Height + Parent->Map->PSection->OriginY && x < MapX + Width + Parent->Map->PSection->OriginX && x + length > MapX + Parent->Map->PSection->OriginX)
                InvalidateEdge(LESS_BOTTOM);
            break;
        }
    }
};
static int Run(CPatchEdges& p, long x, long y, long len, int side)
{
    p.lastInvalidated = -1;
    p.InvalidateEdge(x, y, len, (LANDSCAPE_EDGE_STRIP_SIDE)side);
    return p.lastInvalidated;
}
int main()
{
    CLandscapeMapOrigin o; o.OriginX = 100; o.OriginY = 200;
    CEngineLandscapeMap m; m.PSection = &o;
    CLandscapeBackgroundTreeNode n; n.Map = &m;
    CPatchEdges p; p.Parent = &n; p.MapX = 10; p.MapY = 20; p.Width = 16; p.Height = 8;
    // patch spans x 110..126, y 220..228
    if (Run(p, 110, 221, 2, 0) != 0) { std::printf("00be63f0_TEST FAIL\n"); return 1; }
    if (Run(p, 111, 221, 2, 0) != -1) { std::printf("00be63f0_TEST FAIL\n"); return 1; }
    if (Run(p, 110, 228, 2, 0) != -1) { std::printf("00be63f0_TEST FAIL\n"); return 1; }
    if (Run(p, 110, 210, 10, 0) != -1) { std::printf("00be63f0_TEST FAIL\n"); return 1; }
    if (Run(p, 110, 210, 11, 0) != 0) { std::printf("00be63f0_TEST FAIL\n"); return 1; }
    if (Run(p, 126, 225, 1, 1) != 1) { std::printf("00be63f0_TEST FAIL\n"); return 1; }
    if (Run(p, 115, 220, 1, 2) != 2) { std::printf("00be63f0_TEST FAIL\n"); return 1; }
    if (Run(p, 126, 220, 1, 2) != -1) { std::printf("00be63f0_TEST FAIL\n"); return 1; }
    if (Run(p, 100, 228, 11, 3) != 3) { std::printf("00be63f0_TEST FAIL\n"); return 1; }
    if (Run(p, 100, 228, 11, 4) != -1) { std::printf("00be63f0_TEST FAIL\n"); return 1; }
    std::printf("00be63f0_TEST PASS\n");
    return 0;
}
