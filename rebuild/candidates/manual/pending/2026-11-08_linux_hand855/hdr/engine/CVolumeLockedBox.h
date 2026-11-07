#ifndef FABLE_ENGINE_CVOLUMELOCKEDBOX_H
#define FABLE_ENGINE_CVOLUMELOCKEDBOX_H
// CVolumeLockedBox (chunk 855, row 0x00A68DE0). Ego_r.pdb: 24 bytes; returned by value from CVolumeTexture::Lock 0x00A685E0
// (hidden result pointer) and read member-wise (Dimensions X/Y/Z, RowPitch, SlicePitch, PMem).
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/C3DExtentsI.h"
class CVolumeLockedBox {
public:
    C3DExtentsI    Dimensions;                                           // +0x00
    long           RowPitch;                                             // +0x0c
    long           SlicePitch;                                           // +0x10
    unsigned char* PMem;                                                 // +0x14
};
FABLE_STATIC_ASSERT(sizeof(CVolumeLockedBox) == 0x18);
#endif
