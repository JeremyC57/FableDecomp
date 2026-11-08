#ifndef FABLE_ENGINE_CDISPLAYMODEDESC_H
#define FABLE_ENGINE_CDISPLAYMODEDESC_H
// CDisplayModeDesc (new, chunk 880, row 0x0058AF10 CTCInventory::OpenPCOptionsMenu): one display mode the profile offers. Ego_r.pdb
// (struct_layouts_egor.tsv, 16 bytes, class key V in ?GetDisplayModes@CUserProfileManager@@QBEABV?$vector@VCDisplayModeDesc@@..): C2DExtentsI
// Dimensions +0, long BitsPerPixel +8, long RefreshRate +0xC. Retail reads the same offsets (0x0058D791..0x0058D7AC: [esi+8] / [esi+4] /
// [esi], 16-byte stride `add esi,10h`).
#include "rebuild_abi.h"
#include "engine/C2DExtentsI.h"
class CDisplayModeDesc {
public:
    C2DExtentsI Dimensions;                                              // +0x00
    long BitsPerPixel;                                                   // +0x08
    long RefreshRate;                                                    // +0x0c
};
FABLE_STATIC_ASSERT(sizeof(CDisplayModeDesc) == 0x10);
#endif
