#ifndef FABLE_ENGINE_C3DVERTEXBLEND2_H
#define FABLE_ENGINE_C3DVERTEXBLEND2_H
// New header (chunk 851, row 0x00A9BBE0): C3DVertexBlend2 (FableWin.pdb; Codex pass 12 model), sizeof 8: VertexIndex +0, BlendValue +4.
// Element type of C3DMeshFileGroupChunk::VertexList (CArray<C3DVertexBlend2>).
#include "rebuild_abi.h"
class C3DVertexBlend2 {
#ifdef FABLE_RETAIL_QUADRIC_COPY_TO_PASS30
public:
    C3DVertexBlend2(unsigned long vertex,float weight) : VertexIndex(vertex),BlendValue(weight) {}
#endif
#ifdef FABLE_RETAIL_C3DVERTEXBLEND2_GETTERS
public:
    // Opt-in (wfA3, row 0x00ACDB80): FableWin.pdb inline ?GetVertexIndex@C3DVertexBlend2@@QBEKXZ / ?GetBlendValue@C3DVertexBlend2@@QBEMXZ
    // (absent from Ego_r = always inlined); the weight loop reads both words of each blend (0x00ACE1C0..).
    unsigned long GetVertexIndex() const { return VertexIndex; }
    float GetBlendValue() const { return BlendValue; }
#endif
private:
    unsigned long VertexIndex;   // +0x00
    float BlendValue;            // +0x04
};
FABLE_STATIC_ASSERT_AT(11, sizeof(C3DVertexBlend2) == 8);
#endif
