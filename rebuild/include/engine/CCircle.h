#ifndef FABLE_ENGINE_CCIRCLE_H
#define FABLE_ENGINE_CCIRCLE_H
#include "engine/C2DVector.h"
// Ego_r.pdb CCircle (12 bytes, struct_layouts_egor.tsv): C2DVector Centre +0, float Radius +8.
// Row 0x00724630: FableWin.pdb ?GetRadius@CCircle@@QBEMXZ (inline accessor). No out-of-line methods modelled.
class CCircle {
public:
    float GetRadius() const { return Radius; }
public:
    C2DVector Centre;                                   //0x0
    float     Radius;                                   //0x8
};
#endif
