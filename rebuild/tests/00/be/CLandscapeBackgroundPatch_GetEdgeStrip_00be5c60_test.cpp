// Behaviour test for 0x00BE5C60 CLandscapeBackgroundPatch::GetEdgeStrip(LANDSCAPE_EDGE_STRIP_SIDE): sides 0..3 give the four
// EdgeStrips elements in order, any other side value gives 0.
#include <cstdio>

enum LANDSCAPE_EDGE_STRIP_SIDE { };

class CPatchTesselationEdgeStrip {
public:
    virtual ~CPatchTesselationEdgeStrip() { }
    unsigned short StartPos;
    unsigned short Size;
    bool Tesselated;
};

class CLandscapeBackgroundPatch {
public:
    virtual ~CLandscapeBackgroundPatch() { }
    CPatchTesselationEdgeStrip* GetEdgeStrip(LANDSCAPE_EDGE_STRIP_SIDE side);
    void* Parent;
    CPatchTesselationEdgeStrip EdgeStrips[4];
};

CPatchTesselationEdgeStrip* CLandscapeBackgroundPatch::GetEdgeStrip(LANDSCAPE_EDGE_STRIP_SIDE side)
{
    switch (side) {
    case 0: return &EdgeStrips[0];
    case 1: return &EdgeStrips[1];
    case 2: return &EdgeStrips[2];
    case 3: return &EdgeStrips[3];
    }
    return 0;
}

int main()
{
    CLandscapeBackgroundPatch p;
    for (int i = 0; i < 4; ++i) {
        if (p.GetEdgeStrip((LANDSCAPE_EDGE_STRIP_SIDE)i) != &p.EdgeStrips[i]) { std::printf("00be5c60_TEST FAIL\n"); return 1; }
    }
    if (p.GetEdgeStrip((LANDSCAPE_EDGE_STRIP_SIDE)4) != 0 || p.GetEdgeStrip((LANDSCAPE_EDGE_STRIP_SIDE)-1) != 0 ||
        p.GetEdgeStrip((LANDSCAPE_EDGE_STRIP_SIDE)100) != 0) {
        std::printf("00be5c60_TEST FAIL\n"); return 1;
    }
    std::printf("00be5c60_TEST PASS\n");
    return 0;
}