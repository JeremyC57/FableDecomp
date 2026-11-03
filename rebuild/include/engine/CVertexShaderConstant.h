#ifndef FABLE_ENGINE_CVERTEXSHADERCONSTANT_H
#define FABLE_ENGINE_CVERTEXSHADERCONSTANT_H
// CVertexShaderConstant: one retail model (merged 2026-11-03 from chunk 723b and Codex pass 13). Ego_r.pdb class (V-keyed,
// 0x10 bytes: float X, Y, Z, W). Element of CVertexShaderConstantLayout::ReservedPresets (retail divides the byte span by 16:
// `sar ebx,4`, 0x00B709CE). Ego_r.pdb also names ??0CVertexShaderConstant@@QAE@MMMM@Z; no
// landed row needs it yet and no out-of-line copy is bound in retail, so it is not declared here.
#include "rebuild_abi.h"
class CVertexShaderConstant {
public:
    float X;                                                             // +0x00
    float Y;                                                             // +0x04
    float Z;                                                             // +0x08
    float W;                                                             // +0x0c
};
FABLE_STATIC_ASSERT(sizeof(CVertexShaderConstant) == 0x10);
#endif // FABLE_ENGINE_CVERTEXSHADERCONSTANT_H
