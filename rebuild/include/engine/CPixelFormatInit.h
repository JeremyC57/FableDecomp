#ifndef FABLE_ENGINE_CPIXELFORMATINIT_H
#define FABLE_ENGINE_CPIXELFORMATINIT_H
// New shared header (egod e13, row 0x009FB750 CTexture::LoadFromDataStreamToPreallocatedSurface): the 6-byte packed pixel-format
// description stored in texture files (read as part of CTextureFileInfo, handed to CPixelFormat::Initialise(const CPixelFormatInit&)
// 0x009E3640). Members: Ego_r.pdb (struct_layouts_egor.tsv: Type uchar:4, Linear uchar:1 at +0, ColourDepth +1, RBits +2, GBits +3,
// BBits +4, ABits +5; sizeof 6), Ego_d.pdb class record 0x2784. Packed (owner convention: the class's own header packs it).
#include "rebuild_abi.h"
#pragma pack(push, 1)
class CPixelFormatInit {
public:
    unsigned char Type : 4;                                              // +0x00 bits 0..3
    unsigned char Linear : 1;                                            // +0x00 bit 4
    unsigned char ColourDepth;                                           // +0x01
    unsigned char RBits;                                                 // +0x02
    unsigned char GBits;                                                 // +0x03
    unsigned char BBits;                                                 // +0x04
    unsigned char ABits;                                                 // +0x05
};
#pragma pack(pop)
FABLE_STATIC_ASSERT(sizeof(CPixelFormatInit) == 6);
#endif
