#ifndef FABLE_ENGINE_CVOLUMELOCKEDBOX_H
#define FABLE_ENGINE_CVOLUMELOCKEDBOX_H
#ifdef FABLE_RETAIL_VOLUME_SERIALIZATION_PASS30
#include "engine/C3DExtentsI.h"
// Ego_r layout and the Lock result agree on the dimensions followed by the two pitches and data pointer.
class CVolumeLockedBox {
public:
#ifdef FABLE_RETAIL_VOLUME_LOCKED_BOX_CTORS_PASS30
    CVolumeLockedBox() {}
    CVolumeLockedBox(const CVolumeLockedBox& value)
        : Dimensions(value.Dimensions), RowPitch(value.RowPitch), SlicePitch(value.SlicePitch), PMem(value.PMem) {}
#endif
    C3DExtentsI Dimensions;
    long RowPitch;
    long SlicePitch;
    unsigned char* PMem;
};
FABLE_STATIC_ASSERT(sizeof(CVolumeLockedBox) == 24);
#endif

#endif
