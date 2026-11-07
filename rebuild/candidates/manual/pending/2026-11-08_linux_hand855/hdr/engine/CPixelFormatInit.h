#ifndef FABLE_ENGINE_CPIXELFORMATINIT_H
#define FABLE_ENGINE_CPIXELFORMATINIT_H
// CPixelFormatInit (chunk 855, row 0x00A68DE0). Ego_r.pdb: 6 bytes, byte-packed (it sits at +7 of the 21-byte
// CVolumeTextureFileInfo): Type : 4 and Linear : 1 share byte 0, then ColourDepth, R/G/B/A bit counts.
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
FABLE_STATIC_ASSERT(sizeof(CPixelFormatInit) == 0x6);
#endif
