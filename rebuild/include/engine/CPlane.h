#ifndef FABLE_ENGINE_CPLANE_H
#define FABLE_ENGINE_CPLANE_H
// New header (chunk 851, row 0x00A9BBE0): CPlane, Ego_r.pdb (struct_layouts_egor.tsv) sizeof 0x10: Normal (C3DVector) +0, Distance +0x0C.
// Element type of C3DMeshFileConvexVolumeChunk::Planes (CArray<CPlane>).
#include "rebuild_abi.h"
#include "engine/C3DVector.h"
class CPlane {
public:
    C3DVector Normal;       // +0x00
    float Distance;         // +0x0C
#ifdef FABLE_RETAIL_CPLANE_LINE
    // Retail opt-in (chunk 1013, rows 0x0063D0A0 / 0x0063D4A0): Ego_r.pdb ?Initialise@CPlane@@QAE_NABVC3DVector@@00@Z (Ego_r 0x00B9DB30;
    // retail 0x00A42170, out of line), ?GetIntersectionWithLine@CPlane@@QBE_NABVC3DVector@@0PAV2@@Z (Ego_r 0x00B9DC90; retail
    // 0x00A422C0, out of line) and FableWin.pdb ?GetNormal@CPlane@@QBEABVC3DVector@@XZ (0x024AA120; inline in retail). No layout change.
    bool Initialise(const C3DVector& a, const C3DVector& b, const C3DVector& c);                          // 0x00A42170
    bool GetIntersectionWithLine(const C3DVector& point, const C3DVector& direction, C3DVector* out) const; // 0x00A422C0
    const C3DVector& GetNormal() const { return Normal; }
#endif
};
FABLE_STATIC_ASSERT(sizeof(CPlane) == 0x10);
#endif
