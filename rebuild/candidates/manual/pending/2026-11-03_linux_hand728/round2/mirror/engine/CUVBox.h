#ifndef FABLE_ENGINE_CUVBOX_H
#define FABLE_ENGINE_CUVBOX_H
// CUVBox (Ego_r.pdb, class V, 0x20 bytes): float U[4] +0x00, float V[4] +0x10. Neither PDB name set holds a CUVBox
// member function (no ??0CUVBox in FableWin.pdb either): an aggregate with the implicit constructor. 0x006286F0 fills a
// stack CUVBox member by member (U[0], V[0], U[1], V[1], ...) before CRenderManager2D::Draw2DTexture.
#include "rebuild_abi.h"
class CUVBox {
public:
    float U[4];                                                          // +0x00
    float V[4];                                                          // +0x10
};
FABLE_STATIC_ASSERT(sizeof(CUVBox) == 0x20);
#endif // FABLE_ENGINE_CUVBOX_H
