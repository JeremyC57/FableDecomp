// CRenderManagerCore::DrawAsIndexedTris(const CTexture** textures, unsigned long noTextures, const void* vertices, long stride, long vertexCount, void* indices, long triangleCount) -- attach every texture of the array to its stage, realise the render states, then draw user-memory vertices/16-bit indices as an indexed triangle list
// Leaf: ?DrawAsIndexedTris@CRenderManagerCore@@QAEXPAPBVCTexture@@KPBXJJPAXJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0AEA0, length 146 bytes (+14 int3 padding).
// Shape: the overload taking a CTexture array; the loop `for (i = 0; i < noTextures; ++i) AttachTextureToStage(i, textures[i])`
// is the inlined AttachTextureToStage (ebx walks CurrentlySelectedTextures +0x3D00, esi the stage, `inc esi` is the
// stage + 1 of the TextureStagesInUse grow). Then the same tail as the landed four-argument overload.
// Callees: CRenderStateManager::RealiseRenderState @ 0x00A058C0; IDirect3DDevice9::SetTexture (slot 65, +0x104);
// IDirect3DDevice9::DrawIndexedPrimitiveUP (slot 84, +0x150), D3DPT_TRIANGLELIST, D3DFMT_INDEX16 (0x65).
// NF150: ported onto the retail branch of engine/CRenderManagerCore.h (opt-in FABLE_RETAIL_CRENDERMANAGERCORE_DRAWASINDEXEDTRIS:
//   ?DrawAsIndexedTris@CRenderManagerCore@@QAEXPAPBVCTexture@@KPBXJJPAXJ@Z, Ego_r.pdb 0x00B74460 / FableWin.pdb 0x030EC200: const CTexture**,
//   long stride / count, non-const void* indices) with the header's expanded AttachTextureToStage (FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS)
//   and engine/IDirect3DDevice9.h; was a local CRenderManagerCore_Methods stand-in over a renamed-include model and a local device vtable.
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_CRENDERMANAGERCORE_DRAWASINDEXEDTRIS
#include "engine/CRenderManagerCore.h"

void CRenderManagerCore::DrawAsIndexedTris(const CTexture** textures, unsigned long noTextures, const void* vertices, long stride,
                                           long vertexCount, void* indices, long triangleCount)
{
    for (unsigned long i = 0; i < noTextures; ++i)
        AttachTextureToStage(i, textures[i]);
    RealiseRenderState();
    PD3DDevice->DrawIndexedPrimitiveUP(4 /* D3DPT_TRIANGLELIST */, 0, vertexCount, triangleCount, indices,
                                       0x65 /* D3DFMT_INDEX16 */, vertices, stride);
}
