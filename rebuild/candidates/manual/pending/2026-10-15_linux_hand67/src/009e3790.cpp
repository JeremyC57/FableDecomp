// CPixelFormat::Initialise(long rBits, long gBits, long bBits, long aBits, bool linear) -- pick the first RGB-type (PFType 2) D3D format descriptor whose R/G/B/alpha bit counts match and store its index; leave FormatIndex unchanged when none does
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E3790, length 87 bytes.
// Identity: Ego_r.pdb `?Initialise@CPixelFormat@@QAEXJJJJ_N@Z` (`ret 14h` = five arguments; the
// `linear` flag is not read by this build).
// Data: the CD3DFormatInfo table @ 0x0129BA40 (36-byte records: D3DFormat +0, PFType +4, BitDepth +8,
// AlphaBits +0xC, RBits +0x10, GBits +0x14, BBits +0x18, StencilBits +0x1C, Linear +0x20 -- the PDB
// layout, confirmed by the table bytes), terminated by a record whose BitDepth is -1 (entry 46).
// The landed CPixelFormat::IsCompressed/IsSigned (0x009E3940/70) read PFType at 0x0129BA44.
// Shape: the walk is a do/while (no entry test before the first record) over a pointer to the
// record's RBits (`mov eax,offset table+10h`) with a parallel index in edx; the terminator test
// reads the NEXT record's BitDepth (`cmp [eax-8],-1` after `add eax,24h`); `this` is spilled to the
// `push ecx` slot and only written on the found path. `this` from engine/CPixelFormat.h.
#include "engine/CPixelFormat.h"

struct CD3DFormatInfo {
    long          D3DFormat;                       // +0x00 _D3DFORMAT
    long          PFType;                          // +0x04 EPixelFormatType (2 = RGB)
    long          BitDepth;                        // +0x08 (-1 terminates the table)
    long          AlphaBits;                       // +0x0c
    long          RBits;                           // +0x10
    long          GBits;                           // +0x14
    long          BBits;                           // +0x18
    long          StencilBits;                     // +0x1c
    bool          Linear;                          // +0x20
};

extern const CD3DFormatInfo g_D3DFormatInfo[];     // 0x0129BA40

struct CPixelFormat_Methods : CPixelFormat {
    void Initialise(long rBits, long gBits, long bBits, long aBits, bool linear);
};

void CPixelFormat_Methods::Initialise(long rBits, long gBits, long bBits, long aBits, bool linear)
{
    long index = 0;
    const CD3DFormatInfo* info = g_D3DFormatInfo;
    do {
        if (info->PFType == 2 && info->RBits == rBits && info->GBits == gBits && info->BBits == bBits && info->AlphaBits == aBits) {
            FormatIndex = index;
            return;
        }
        ++info;
        ++index;
    } while (info->BitDepth != -1);
}
