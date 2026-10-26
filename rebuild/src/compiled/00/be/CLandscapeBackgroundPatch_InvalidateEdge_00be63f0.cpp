// CLandscapeBackgroundPatch::InvalidateEdge(long x, long y, long length, LANDSCAPE_EDGE_STRIP_SIDE side) -- if the world-space strip (x, y, length) lies on the given edge of this patch (the patch rectangle offset by the landscape map origin) and overlaps it, invalidate that edge strip
// Leaf: ?InvalidateEdge@CPatchEdges
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE63F0, length 432 bytes (jump table of 4 at 0x00BE6588 inside the row; `ret 0x10`).
// Callees: CLandscapeBackgroundPatch::InvalidateEdge(LANDSCAPE_EDGE_STRIP_SIDE) @ 0x00BE5CA0.
// Layout: `this` is engine/CLandscapeBackgroundPatch.h (Parent +0x04, MapX/MapY/Width/Height words +0x10c..+0x112). The parent
//   tree node in retail is smaller than the PDB view (engine/CLandscapeBackgroundTreeNode.h puts Map at +0xac): retail reads the
//   map pointer at +0xa4 and the map's origin block at +0x14 (origin words at +0x60/+0x62), so the node and map are local models.
// Shape: a 4-way switch over the side (left, right, top, bottom); the origin is read through the full `Parent->Map->PSection` chain in
//   every operand (a named pointer local moves the block into edx and reorders the adds), and VC7.1 CSEs the chain as retail does.
#include "engine/CLandscapeBackgroundPatch.h"

enum LANDSCAPE_EDGE_STRIP_SIDE { LESS_LEFT = 0, LESS_RIGHT = 1, LESS_TOP = 2, LESS_BOTTOM = 3 };
struct CLandscapeMapOrigin {
    unsigned char  _pad00[0x60];
    unsigned short OriginX;                          // +0x60
    unsigned short OriginY;                          // +0x62
};
class CEngineLandscapeMap { public:
    unsigned char        _pad00[0x14];
    CLandscapeMapOrigin* PSection;                   // +0x14
};
class CLandscapeBackgroundTreeNode { public:
    unsigned char        _pad00[0xa4];
    CEngineLandscapeMap* Map;                        // +0xa4 (retail)
};
struct CPatchEdges : CLandscapeBackgroundPatch {
    void InvalidateEdge(LANDSCAPE_EDGE_STRIP_SIDE side);                                   // 0x00BE5CA0
    void InvalidateEdge(long x, long y, long length, LANDSCAPE_EDGE_STRIP_SIDE side);
};

void CPatchEdges::InvalidateEdge(long x, long y, long length, LANDSCAPE_EDGE_STRIP_SIDE side)
{
    switch (side) {
    case LESS_LEFT: {
        if (x == MapX + Parent->Map->PSection->OriginX && y < MapY + Height + Parent->Map->PSection->OriginY && y + length > MapY + Parent->Map->PSection->OriginY)
            InvalidateEdge(LESS_LEFT);
        break;
    }
    case LESS_RIGHT: {
        if (x == MapX + Width + Parent->Map->PSection->OriginX && y < MapY + Height + Parent->Map->PSection->OriginY && y + length > MapY + Parent->Map->PSection->OriginY)
            InvalidateEdge(LESS_RIGHT);
        break;
    }
    case LESS_TOP: {
        if (y == MapY + Parent->Map->PSection->OriginY && x < MapX + Width + Parent->Map->PSection->OriginX && x + length > MapX + Parent->Map->PSection->OriginX)
            InvalidateEdge(LESS_TOP);
        break;
    }
    case LESS_BOTTOM: {
        if (y == MapY + Height + Parent->Map->PSection->OriginY && x < MapX + Width + Parent->Map->PSection->OriginX && x + length > MapX + Parent->Map->PSection->OriginX)
            InvalidateEdge(LESS_BOTTOM);
        break;
    }
    }
}