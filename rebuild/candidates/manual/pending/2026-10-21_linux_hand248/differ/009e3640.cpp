// Grade: DIFFER(332v328) -- block layout only: retail places the shared found-store tail of cases 1/2 right after case 1 (so case 2's loop head is unaligned); ours places it after case 2 and pads case 2's loop head with `lea esp,[esp]` (4 bytes). Same instructions otherwise.
// CPixelFormat::Initialise(const CPixelFormatInit& init) -- pick the D3D format descriptor for a packed pixel-format description: RGB (type 1) and type-2 formats match on pixel-format type and the R/G/B/alpha/colour-depth bit counts, types 3..5 take the first descriptor of pixel-format type 5, 4 or 7; FormatIndex is -1 when nothing matches
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E3640, length 306 bytes (the 5-entry switch table at 0x009E3774 follows
// the code), `ret 4`.
// Identity: Ego_r.pdb `?Initialise@CPixelFormat@@QAEXABVCPixelFormatInit@@@Z`.
// Data: the CD3DFormatInfo table @ 0x0129BA40 (36-byte records, terminated by BitDepth -1), as the landed
// CPixelFormat::Initialise(long, long, long, long, bool) 0x009E3790.
// Layout: CPixelFormatInit (Ego_r.pdb, 6 bytes): Type:4 / Linear:1 bitfield byte +0, ColourDepth +1, RBits +2,
// GBits +3, BBits +4, ABits +5. `this` from engine/CPixelFormat.h.
// Shape: FormatIndex = -1 first; a switch on the 4-bit Type (cases 1..5, `dec ; cmp 4 ; ja`); each case is the landed
// do/while table walk (no entry test, the terminator read from the next record); the two compare walks share one
// found-store tail and the three type-only walks another.
#include "engine/CPixelFormat.h"

struct CD3DFormatInfo {
    long          D3DFormat;                       // +0x00 _D3DFORMAT
    long          PFType;                          // +0x04 EPixelFormatType
    long          BitDepth;                        // +0x08 (-1 terminates the table)
    long          AlphaBits;                       // +0x0c
    long          RBits;                           // +0x10
    long          GBits;                           // +0x14
    long          BBits;                           // +0x18
    long          StencilBits;                     // +0x1c
    bool          Linear;                          // +0x20
};

extern const CD3DFormatInfo g_D3DFormatInfo[];     // 0x0129BA40

struct CPixelFormatInit {                          // 6 bytes
    unsigned char Type : 4;                        // +0 bits 0..3
    unsigned char Linear : 1;                      // +0 bit 4
    unsigned char ColourDepth;                     // +1
    unsigned char RBits;                           // +2
    unsigned char GBits;                           // +3
    unsigned char BBits;                           // +4
    unsigned char ABits;                           // +5
};

struct CPixelFormat_Methods : CPixelFormat {
    void Initialise(const CPixelFormatInit& init);
};

void CPixelFormat_Methods::Initialise(const CPixelFormatInit& init)
{
    FormatIndex = -1;
    switch (init.Type) {
    case 1: {
        long index = 0;
        const CD3DFormatInfo* info = g_D3DFormatInfo;
        do {
            if (info->PFType == 2 && info->RBits == init.RBits && info->GBits == init.GBits && info->BBits == init.BBits &&
                info->AlphaBits == init.ABits && info->BitDepth == init.ColourDepth) {
                FormatIndex = index;
                return;
            }
            ++info;
            ++index;
        } while (info->BitDepth != -1);
        break;
    }
    case 2: {
        long index = 0;
        const CD3DFormatInfo* info = g_D3DFormatInfo;
        do {
            if (info->PFType == 6 && info->RBits == init.RBits && info->GBits == init.GBits && info->BBits == init.BBits &&
                info->AlphaBits == init.ABits && info->BitDepth == init.ColourDepth) {
                FormatIndex = index;
                return;
            }
            ++info;
            ++index;
        } while (info->BitDepth != -1);
        break;
    }
    case 3: {
        long index = 0;
        const CD3DFormatInfo* info = g_D3DFormatInfo;
        do {
            if (info->PFType == 5) {
                FormatIndex = index;
                return;
            }
            ++info;
            ++index;
        } while (info->BitDepth != -1);
        break;
    }
    case 4: {
        long index = 0;
        const CD3DFormatInfo* info = g_D3DFormatInfo;
        do {
            if (info->PFType == 4) {
                FormatIndex = index;
                return;
            }
            ++info;
            ++index;
        } while (info->BitDepth != -1);
        break;
    }
    case 5: {
        long index = 0;
        const CD3DFormatInfo* info = g_D3DFormatInfo;
        do {
            if (info->PFType == 7) {
                FormatIndex = index;
                return;
            }
            ++info;
            ++index;
        } while (info->BitDepth != -1);
        break;
    }
    }
}
