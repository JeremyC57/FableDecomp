#ifndef FABLE_ENGINE_CVERTEXSHADERCONSTANT_H
#define FABLE_ENGINE_CVERTEXSHADERCONSTANT_H
// CVertexShaderConstant (chunk 723b): Ego_r.pdb class (V-keyed, 0x10 bytes: float X, Y, Z, W). Element of
// CVertexShaderConstantLayout::ReservedPresets (retail divides the byte span by 16: `sar ebx,4`, 0x00B709CE).
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
