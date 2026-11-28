#ifndef FABLE_ENGINE_GFGETTOPBITINBYTE_H
#define FABLE_ENGINE_GFGETTOPBITINBYTE_H
#ifdef FABLE_RETAIL_MESH_MIPMAP_PASS30
#include "fable_compiler.h"
extern unsigned char GTopBitInByteTable[256];
// FableWin GFGetTopBitInByte and the retail byte-indexed table lookup.
inline unsigned long FABLE_FASTCALL GFGetTopBitInByte(unsigned char value)
{
    return GTopBitInByteTable[value];
}
#endif
#endif
