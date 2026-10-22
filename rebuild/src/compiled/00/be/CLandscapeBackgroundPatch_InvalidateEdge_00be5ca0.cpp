// CLandscapeBackgroundPatch::InvalidateEdge(long edge) -- clear the Tesselated flag of one of the patch's four edge strips (an out-of-range edge does nothing)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BE5CA0, 68 bytes incl. the 4-entry jump table, `ret 4`. Called four times from 0x00BE6450..0x00BE6580.
// Callees: none.
// Shape: a 4-way switch on the edge (`cmp eax,3 ; ja` + table) whose every case stores `Tesselated = false`
//   (CPatchTesselationEdgeStrip +0x28) at its own constant offset (+0x30, +0x5C, +0x88, +0xB4) and returns; the
//   default jumps straight to the `ret 4`. Written through an inlined GetEdgeStrip (the out-of-line 0x00BE5C60 is the
//   same switch returning the strip or 0) VC7.1 does not fold the null test into the cases.
// Layout: EdgeStrips is CPatchTesselationEdgeStrip[4] (0x2C each, Ego_r.pdb) at +0x08; the engine header keeps it as raw
//   bytes, so the strip is modelled here (StartPos +4, Size +6, the four vertex arrays +8..+0x27, Tesselated +0x28).
#include "engine/CLandscapeBackgroundPatch.h"

struct CPatchTesselationEdgeStrip {
    void*          _vertexOwner;                  // +0x00
    unsigned short StartPos;                      // +0x04
    unsigned short Size;                          // +0x06
    unsigned char  VertexArrays[0x20];            // +0x08 Verts, FanBaseVerts, WaterVerts, WaterFanBaseVerts
    bool           Tesselated;                    // +0x28
    bool           Vertical;                      // +0x29
    unsigned char  _pad2a[2];
};

struct CLandscapeBackgroundPatchMethods : CLandscapeBackgroundPatch {
    CPatchTesselationEdgeStrip* Strip(long index) { return (CPatchTesselationEdgeStrip*)EdgeStrips + index; }
    void InvalidateEdge(long edge);
};

void CLandscapeBackgroundPatchMethods::InvalidateEdge(long edge)
{
    switch (edge) {
    case 0: Strip(0)->Tesselated = false; break;
    case 1: Strip(1)->Tesselated = false; break;
    case 2: Strip(2)->Tesselated = false; break;
    case 3: Strip(3)->Tesselated = false; break;
    }
}