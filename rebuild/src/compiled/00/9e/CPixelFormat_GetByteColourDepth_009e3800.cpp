// Leaf: ?CPixelFormat_GetByteColourDepth@@YIKPAVCPixelFormat@@@Z
#include "engine/CPixelFormat.h"  // retyped onto the PDB layout; byte parity re-verified
struct PixFmtEntry { int bits; int pad[8]; };
extern PixFmtEntry g_pixFmtTable[];
unsigned long __fastcall CPixelFormat_GetByteColourDepth(CPixelFormat* format)
{
    int bits = g_pixFmtTable[format->FormatIndex].bits;
    return (unsigned long)(bits / 8);
}