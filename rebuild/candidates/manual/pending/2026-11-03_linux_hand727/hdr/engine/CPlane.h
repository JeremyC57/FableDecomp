#ifndef FABLE_ENGINE_CPLANE_H
#define FABLE_ENGINE_CPLANE_H
// CPlane (chunk 727): Ego_r.pdb class, 0x10 bytes -- Normal (C3DVector) +0x00, Distance +0x0C (struct_layouts_egor.tsv; retail
// agrees: CEngineShadowRenderer::CalcClipVolume 0x00B4BCC0 copies planes as four dwords and dots words 0..2 with the light).
// Initialise(const C3DVector&, const C3DVector&, const C3DVector&): Ego_r/FableWin ?Initialise@CPlane@@QAE_NABVC3DVector@@00@Z,
// out of line in retail at 0x00A42170 (bound in reloc_bindings). The ctors are FableWin.pdb's (see below). GetNormal: FableWin ?GetNormal@CPlane@@QBEABVC3DVector@@XZ,
// inline in retail.
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/C3DVector.h"
class CPlane {
public:
    C3DVector Normal;                                                    // +0x00
    float     Distance;                                                  // +0x0c
    // FableWin.pdb ??0CPlane@@QAE@XZ and ??0CPlane@@QAE@ABVC3DVector@@00@Z, inline in retail (0x00B4BF81: a frame temporary built
    // by the out-of-line Initialise, then copied by AddPlane).
    CPlane() { }
    CPlane(const C3DVector& p0, const C3DVector& p1, const C3DVector& p2) { Initialise(p0, p1, p2); }
    bool Initialise(const C3DVector& p0, const C3DVector& p1, const C3DVector& p2);   // 0x00A42170
    const C3DVector& GetNormal() const { return Normal; }
};
FABLE_STATIC_ASSERT(sizeof(CPlane) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CPlane, Distance) == 0xc);
#endif // FABLE_ENGINE_CPLANE_H
