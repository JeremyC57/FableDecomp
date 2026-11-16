// CRenderManager2D::AddQuadToDrawBuffer(const CTVertexRHWColSpecTex1& v0, v1, v2, v3, unsigned long blend_mode, CTexture* texture) -- split a 2D quad into the triangles (v0,v1,v2) and (v0,v2,v3): opaque triangles for blend mode 1, alpha triangles otherwise
// Leaf: ?AddQuadToDrawBuffer@CRenderManager2D@@QAEXABVCTVertexRHWColSpecTex1@@000KPAVCTexture@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009DC500, length 119 bytes (`ret 18h`). Ego_r.pdb 0x00B30140
//   ?AddQuadToDrawBuffer@CRenderManager2D@@QAEXABVCTVertexRHWColSpecTex1@@000KPAVCTexture@@@Z.
// Callees: CRenderManager2D::AddTri 0x009DB810, CRenderManager2D::AddAlphaTri 0x009DBD10 (both private, Ego_r.pdb / FableWin.pdb
//   (v0, v1, v2, long blend_mode, CTexture*)).
// Shape: `if (blend_mode == 1)` passes the literal 1; the first call of each arm reuses the incoming ecx (`this`
//   untouched since `mov esi,ecx`), the second reloads it from esi.
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGER2D_ADDLINE
#include "fable_compiler.h"
#include "engine/CTVertexRHWColSpecTex1.h"
#include "engine/CRenderManager2D.h"

void CRenderManager2D::AddQuadToDrawBuffer(const CTVertexRHWColSpecTex1& v0, const CTVertexRHWColSpecTex1& v1,
                                           const CTVertexRHWColSpecTex1& v2, const CTVertexRHWColSpecTex1& v3,
                                           unsigned long blend_mode, CTexture* texture)
{
    if (blend_mode == 1) {
        AddTri(v0, v1, v2, 1, texture);
        AddTri(v0, v2, v3, 1, texture);
    } else {
        AddAlphaTri(v0, v1, v2, blend_mode, texture);
        AddAlphaTri(v0, v2, v3, blend_mode, texture);
    }
}
