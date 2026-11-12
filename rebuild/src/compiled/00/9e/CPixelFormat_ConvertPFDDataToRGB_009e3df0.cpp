// CPixelFormat::ConvertPFDDataToRGB(const unsigned char* data, CRGBColour& colour) const -- decode one pixel of this format from raw
// pixel bytes into an 8-bit-per-channel colour: copy GetByteColourDepth() bytes into a zeroed word, mask each channel out at its
// position, scale it up to 8 bits; a format without alpha bits gives alpha 0xFF.
// Leaf: ?ConvertPFDDataToRGB@CPixelFormat@@QBEXPBEAAVCRGBColour@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009E3DF0, 354 bytes (`ret 8`). Callees: none (memcpy is the intrinsic; every accessor is inline).
// Naming: Ego_r.pdb / FableWin.pdb ?ConvertPFDDataToRGB@CPixelFormat@@QBEXPBEAAVCRGBColour@@@Z; the channel helpers are the FableWin.pdb
//   inline accessors GetXMask / GetXPosShift / GetXScaleShift / GetNoXBits / GetByteColourDepth (as the landed sibling 0x009E3B60).
// Globals: GD3DFormats 0x0129BA40 (36-byte CD3DFormatInfo entries; BitDepth +0x08, AlphaBits +0x0C, RBits +0x10, GBits +0x14, BBits +0x18).
// Shape: `pixel` is the `push ecx` frame word, zeroed and filled by `rep movsd` + `rep movsb` with the signed BitDepth / 8 count.
//   Each channel is ((pixel & mask) >> pos) << (8 - bits) narrowed to a byte, stored red (+2), green (+1), blue (+0), then alpha (+3)
//   under `if (GetNoABits())` with 0xFF otherwise (two epilogues). FormatIndex is re-read after every byte store (it may alias). The
//   position shifts are the debug bodies' local-reference field sums (FABLE_RETAIL_CPIXELFORMAT_POSSHIFT_FORMATREF, blue + green
//   (+ red)): VC7.1 does not share those loads between the mask and the right shift, hence retail's second read of the blue / green
//   (/ red) counts in the red and alpha arms, while the green arm's single blue count and the blue arm's byte arithmetic fold.
// The unlisted twin 0x009E3C80 (ConvertPFDDataToRGBLong by PDB order) is byte-identical.
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CPIXELFORMAT_CONVERT
#define FABLE_RETAIL_CPIXELFORMAT_PFDTORGB
#define FABLE_RETAIL_CPIXELFORMAT_POSSHIFT_FORMATREF
#define FABLE_RETAIL_CPIXELFORMAT_PFDDATATORGB
#include <string.h>
#include "engine/CRGBColour.h"
#include "engine/CPixelFormat.h"

void CPixelFormat::ConvertPFDDataToRGB(const unsigned char* data,CRGBColour& colour) const
{
    unsigned long pixel=0;
    memcpy(&pixel,data,GetByteColourDepth());
    colour.R=(unsigned char)(((pixel&GetRMask())>>GetRPosShift())<<GetRScaleShift());
    colour.G=(unsigned char)(((pixel&GetGMask())>>GetGPosShift())<<GetGScaleShift());
    colour.B=(unsigned char)(((pixel&GetBMask())>>GetBPosShift())<<GetBScaleShift());
    if(GetNoABits())
        colour.A=(unsigned char)(((pixel&GetAMask())>>GetAPosShift())<<GetAScaleShift());
    else
        colour.A=0xff;
}