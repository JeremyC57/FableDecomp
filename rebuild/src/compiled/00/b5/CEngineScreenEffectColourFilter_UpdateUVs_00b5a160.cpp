// CEngineScreenEffectColourFilter::UpdateUVs() -- recompute the texture coordinates of the four texture-to-back-buffer vertices with a quarter-texel offset for the current back-buffer size
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B5A160, length 108 bytes (+ int3 padding). No callees.
// Globals: the display manager @ 0x01436E1C (back-buffer width +0x18c, height +0x190); float pool 0.25f @ 0x0123078C, 1.0f @ 0x0122DED8.
// Layout: engine/CEngineScreenEffectColourFilter.h -- TextureToBackBufferVerts (CTVertexColTex1[4], 0x18-byte vertices with
// U at +0x10 and V at +0x14) at +0x48, so the UVs land at +0x58/+0x5c, +0x70/+0x74, +0x88/+0x8c, +0xa0/+0xa4.
// Shape: the size is copied as one 8-byte C2DCoordI local into the frame (both words loaded, then `fild` from the frame), `0.25f / (float)w` gives `fild ; fdivr`;
// the two offsets stay on the x87 stack and `1.0f + dv` / `du + 1.0f` are each computed once (CSE) and stored twice.
#include "engine/CEngineScreenEffectColourFilter.h"

class C2DCoordI { public:
    long X, Y;
};

struct CDisplayManager_01436e1c {
    unsigned char _pad[0x18c];
    C2DCoordI     m_backBufferSize;                // +0x18c (width, height)
};
extern CDisplayManager_01436e1c* g_pDisplayManager_01436e1c;   // 0x01436E1C

class CTVertexColTex1 { public:
    float         X, Y, Z;
    unsigned long Colour;
    float         U, V;                            // +0x10, +0x14
};

struct CEngineScreenEffectColourFilter_UpdateUVs : CEngineScreenEffectColourFilter {
    void UpdateUVs();
};

void CEngineScreenEffectColourFilter_UpdateUVs::UpdateUVs()
{
    C2DCoordI size = g_pDisplayManager_01436e1c->m_backBufferSize;
    float du = 0.25f / (float)size.X;
    float dv = 0.25f / (float)size.Y;
    CTVertexColTex1* verts = (CTVertexColTex1*)TextureToBackBufferVerts;
    verts[0].U = du;
    verts[0].V = 1.0f + dv;
    verts[1].U = du + 1.0f;
    verts[1].V = 1.0f + dv;
    verts[2].U = du + 1.0f;
    verts[2].V = dv;
    verts[3].U = du;
    verts[3].V = dv;
}