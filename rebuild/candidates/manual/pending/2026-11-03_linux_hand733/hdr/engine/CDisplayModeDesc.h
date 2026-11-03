#ifndef FABLE_ENGINE_CDISPLAYMODEDESC_H
#define FABLE_ENGINE_CDISPLAYMODEDESC_H
// Retail model (chunk 733): Ego_r.pdb CDisplayModeDesc (class key V, ghidra_out/pdb_class_keys.tsv), 16 bytes: Dimensions
// (C2DExtentsI) +0x00, BitsPerPixel +0x08, RefreshRate +0x0C (struct_layouts_egor.tsv). Retail agrees: CUserProfileManager
// walks its vector<CDisplayModeDesc> 0x10 at a time comparing +0 / +4 / +8 (SetDefaultValues 0x0040C972..0x0040C97B) and
// ApplyVideoValues 0x0040A7F0 reads RefreshRate +0x0C.
#include "rebuild_abi.h"
#include "engine/C2DExtentsI.h"
class CDisplayModeDesc {
public:
    C2DExtentsI Dimensions;                                           // +0x00
    long        BitsPerPixel;                                         // +0x08
    long        RefreshRate;                                          // +0x0C
};
FABLE_STATIC_ASSERT(sizeof(CDisplayModeDesc) == 0x10);
#endif
