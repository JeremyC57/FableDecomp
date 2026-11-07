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
};
FABLE_STATIC_ASSERT(sizeof(CPlane) == 0x10);
#endif
