// Grade: DIFFER(305v354) -- CSE: retail reloads the format fields after the inline memcpy (the mask shift uses B/G loaded around the rep movs, the right shift reloads them; the alpha arm spills the record offset) where VC7.1 here CSEs every channel shift
// CPixelFormat::ConvertPFDDataToRGB(const unsigned char* data, CRGBColour& colour) const -- decode one pixel of this format into an 8-bit-per-channel BGRA colour: copy BitDepth/8 bytes of the pixel, mask out each channel by the format's R/G/B/alpha bit counts and scale it to 8 bits; formats without alpha give alpha 0xFF
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E3DF0, length 352 bytes (+ int3 padding), `ret 8`.
// Identity: Ego_r.pdb `?ConvertPFDDataToRGB@CPixelFormat@@QBEXPBEAAVCRGBColour@@@Z`.
// Data: the CD3DFormatInfo table @ 0x0129BA40 (36-byte records: BitDepth +8, AlphaBits +0xC, RBits +0x10, GBits +0x14,
// BBits +0x18), as the landed CPixelFormat::Initialise 0x009E3790.
// Layout: CRGBColour (Ego_r.pdb, 4 bytes, a union over the packed D3DCOLOR): B +0, G +1, R +2, A +3. `this` from
// engine/CPixelFormat.h.
// Shape: the pixel word is zeroed and filled by the memcpy intrinsic with a variable byte count (`rep movsd` +
// `rep movsb`); each channel is `((pixel & (((1 << bits) - 1) << shift)) >> shift) << (8 - bits)` truncated to a byte,
// in the order R, G, B, A, with the format record re-indexed after each colour byte store (the byte store may alias
// `this`).
#include "engine/CPixelFormat.h"

extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

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

struct CRGBColour {                                // D3DCOLOR byte order
    unsigned char B;                               // +0
    unsigned char G;                               // +1
    unsigned char R;                               // +2
    unsigned char A;                               // +3
};

struct CPixelFormat_Methods : CPixelFormat {
    const CD3DFormatInfo& GetInfo() const { return g_D3DFormatInfo[FormatIndex]; }
    void ConvertPFDDataToRGB(const unsigned char* data, CRGBColour& colour) const;
};

void CPixelFormat_Methods::ConvertPFDDataToRGB(const unsigned char* data, CRGBColour& colour) const
{
    unsigned long pixel = 0;
    memcpy(&pixel, data, GetInfo().BitDepth / 8);
    colour.R = (unsigned char)(((pixel & (((1 << GetInfo().RBits) - 1) << (GetInfo().GBits + GetInfo().BBits))) >>
                                (GetInfo().GBits + GetInfo().BBits)) << (8 - GetInfo().RBits));
    colour.G = (unsigned char)(((pixel & (((1 << GetInfo().GBits) - 1) << GetInfo().BBits)) >> GetInfo().BBits)
                               << (8 - GetInfo().GBits));
    colour.B = (unsigned char)((pixel & ((1 << GetInfo().BBits) - 1)) << (8 - GetInfo().BBits));
    if (GetInfo().AlphaBits != 0)
        colour.A = (unsigned char)(((pixel & (((1 << GetInfo().AlphaBits) - 1) << (GetInfo().GBits + GetInfo().BBits + GetInfo().RBits))) >>
                                    (GetInfo().GBits + GetInfo().BBits + GetInfo().RBits)) << (8 - GetInfo().AlphaBits));
    else
        colour.A = 0xff;
}
