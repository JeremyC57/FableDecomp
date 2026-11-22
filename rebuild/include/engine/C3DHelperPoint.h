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
    C3DVector Position;
    long BoneIndex;
};
FABLE_STATIC_ASSERT(sizeof(C3DHelperPoint)==0x10);
#endif
#endif
