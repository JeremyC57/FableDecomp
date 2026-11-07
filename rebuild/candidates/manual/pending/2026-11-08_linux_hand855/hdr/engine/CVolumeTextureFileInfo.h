#ifndef FABLE_ENGINE_CVOLUMETEXTUREFILEINFO_H
#define FABLE_ENGINE_CVOLUMETEXTUREFILEINFO_H
// CVolumeTextureFileInfo (chunk 855, row 0x00A68DE0). Ego_r.pdb: 21 bytes, byte-packed -- the header CVolumeTexture::SaveToDataStream
// 0x00A68AE0 writes and LoadFromDataStream 0x00A68DE0 reads (`lea ecx,[eax+15h]`: one 0x15-byte read).
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CPixelFormatInit.h"
#include "engine/EVolumePool.h"
#pragma pack(push, 1)
class CVolumeTextureFileInfo {
public:
    unsigned short   Width;                                              // +0x00
    unsigned short   Height;                                             // +0x02
    unsigned short   Depth;                                              // +0x04
    unsigned char    NoLevels;                                           // +0x06
    CPixelFormatInit PixelFormatInit;                                    // +0x07
    unsigned long    UseageFlags;                                        // +0x0d (PDB spelling)
    EVolumePool      Pool;                                               // +0x11
};
#pragma pack(pop)
FABLE_STATIC_ASSERT(sizeof(CVolumeTextureFileInfo) == 0x15);
FABLE_STATIC_ASSERT(offsetof(CVolumeTextureFileInfo, UseageFlags) == 0xd);
FABLE_STATIC_ASSERT(offsetof(CVolumeTextureFileInfo, Pool) == 0x11);
#endif
