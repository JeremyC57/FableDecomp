#ifndef FABLE_ENGINE_C3DHELPERPOINT_H
#define FABLE_ENGINE_C3DHELPERPOINT_H
#ifdef FABLE_RETAIL_C3DHELPERPOINT_LAYOUT_PASS27
// Ego_r: Position +0x0 and BoneIndex +0xC, sizeof 0x10. FableWin and retail pass this class
// to the two C3DMesh2 helper-point overloads. The 0x00A898D0 body reads both fields.
#include "engine/C3DVector.h"
#include "rebuild_abi.h"

class C3DHelperPoint
{
public:
#ifdef FABLE_RETAIL_C3DMESHSTATS_FILE_LOADER_PASS28
    // FableWin ctor; retail creates Position and BoneIndex together before the map insertion.
    C3DHelperPoint() { }
    C3DHelperPoint(const C3DVector& position,unsigned long bone_index) : Position(position), BoneIndex(bone_index) { }
#endif
#if defined(FABLE_RETAIL_PHYSICS_FILE_COPY_CTORS_PASS29) && !defined(FABLE_RETAIL_C3DMESHSTATS_FILE_LOADER_PASS28)
    // The file reader calls this constructor after the by-value getter.
    C3DHelperPoint(const C3DVector&, unsigned long);
#endif
    C3DVector Position;
    long BoneIndex;
};
FABLE_STATIC_ASSERT_AT(15, sizeof(C3DHelperPoint)==0x10);
#endif
#endif
