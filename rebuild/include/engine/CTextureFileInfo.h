#ifndef FABLE_ENGINE_CTEXTUREFILEINFO_H
#define FABLE_ENGINE_CTEXTUREFILEINFO_H
// New shared header (egod e13, row 0x009FB750 CTexture::LoadFromDataStreamToPreallocatedSurface): the 19-byte packed texture file header.
// Members: Ego_d.pdb class record types.txt 0x6FE97/0x6FE98 (public Width, Height, NoLevels, PixelFormatInit, UseageFlags, Pool;
// sizeof 19), Ego_r.pdb struct_layouts_egor.tsv (same names / offsets). Retail reads it with one 19-byte CDataInputStream::Read
// (0x009FB765 `lea ecx,[eax+13h]`) and hands +5 to CPixelFormat::Initialise (0x009FB7DC `lea ecx,[esp+41h]`).
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CPixelFormatInit.h"
#include "engine/ESurfacePool.h"
#pragma pack(push, 1)
class CTextureFileInfo {
public:
    unsigned short   Width;                                              // +0x00
    unsigned short   Height;                                             // +0x02
    unsigned char    NoLevels;                                           // +0x04
    CPixelFormatInit PixelFormatInit;                                    // +0x05
    unsigned long    UseageFlags;                                        // +0x0B
    ESurfacePool     Pool;                                               // +0x0F
};
#ifdef FABLE_RETAIL_VOLUME_SERIALIZATION_PASS30
// The volume texture file uses the same one-byte file layout as the surface texture.
class CVolumeTextureFileInfo {
public:
    unsigned short Width;
    unsigned short Height;
    unsigned short Depth;
    unsigned char NoLevels;
    CPixelFormatInit PixelFormatInit;
    unsigned long UseageFlags;
    EVolumePool Pool;
};
FABLE_STATIC_ASSERT(sizeof(CVolumeTextureFileInfo) == 21);
FABLE_STATIC_ASSERT(offsetof(CVolumeTextureFileInfo, UseageFlags) == 13);
FABLE_STATIC_ASSERT(offsetof(CVolumeTextureFileInfo, Pool) == 17);
#endif
#pragma pack(pop)
FABLE_STATIC_ASSERT_AT(22, sizeof(CTextureFileInfo) == 19);
FABLE_STATIC_ASSERT_AT(23, offsetof(CTextureFileInfo, UseageFlags) == 0x0b);
#endif
