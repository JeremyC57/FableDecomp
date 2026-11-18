#ifndef FABLE_ENGINE_CCIRCLE_H
#define FABLE_ENGINE_CCIRCLE_H
#include "engine/C2DVector.h"
// Ego_r.pdb CCircle (12 bytes, struct_layouts_egor.tsv): C2DVector Centre +0, float Radius +8.
// Row 0x00724630: FableWin.pdb ?GetRadius@CCircle@@QBEMXZ (inline accessor). No out-of-line methods modelled.
class CCircle {
public:
    float GetRadius() const { return Radius; }
#ifdef FABLE_RETAIL_CCIRCLE_SET
    // Retail opt-in (egod2a, row 0x00A6C640): FableWin.pdb ?Set@CCircle@@QAEXABVC2DVector@@M@Z (38b/16i at /Od; the COMDAT right after
    // CPhysicsMesh::CalculateBoundingInfo), inline in retail (0x00A6C82B..0x00A6C83A). Body only.
    void Set(const C2DVector& centre, float radius) { Centre = centre; Radius = radius; }
#endif
public:
    C2DVector Centre;                                   //0x0
    float     Radius;                                   //0x8
};
#endif
