// Behaviour test for 00be5ca0 CLandscapeBackgroundPatch::InvalidateEdge: clears the Tesselated flag of edge strip 0..3,
// ignores any other index.
#include <cstdio>
struct CPatchTesselationEdgeStrip { unsigned short StartPos, Size; bool Tesselated; bool Vertical; };
struct CLandscapeBackgroundPatch {
    CPatchTesselationEdgeStrip EdgeStrips[4];
    CPatchTesselationEdgeStrip* Strip(long index) { return EdgeStrips + index; }
    void InvalidateEdge(long edge)
    {
        switch (edge) {
        case 0: Strip(0)->Tesselated = false; break;
        case 1: Strip(1)->Tesselated = false; break;
        case 2: Strip(2)->Tesselated = false; break;
        case 3: Strip(3)->Tesselated = false; break;
        }
    }
};
int main()
{
    CLandscapeBackgroundPatch p;
    for (int i = 0; i < 4; ++i) { p.EdgeStrips[i].Tesselated = true; p.EdgeStrips[i].Vertical = true; }
    p.InvalidateEdge(4); p.InvalidateEdge(-1);
    for (int i = 0; i < 4; ++i) if (!p.EdgeStrips[i].Tesselated) { std::printf("00be5ca0_TEST FAIL\n"); return 1; }
    p.InvalidateEdge(2);
    if (p.EdgeStrips[2].Tesselated || !p.EdgeStrips[1].Tesselated || !p.EdgeStrips[3].Tesselated || !p.EdgeStrips[2].Vertical) { std::printf("00be5ca0_TEST FAIL\n"); return 1; }
    p.InvalidateEdge(0); p.InvalidateEdge(1); p.InvalidateEdge(3);
    for (int i = 0; i < 4; ++i) if (p.EdgeStrips[i].Tesselated) { std::printf("00be5ca0_TEST FAIL\n"); return 1; }
    std::printf("00be5ca0_TEST PASS\n");
    return 0;
}