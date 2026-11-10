// CPixelFormat::ConvertRGBToPFDData(const CRGBColour& colour, unsigned char* data) const -- pack an RGBA colour into the pixel format's raw bytes: each channel scaled down to the format's bit count and shifted to its position, the sum written out at the format's byte depth
// Grade: DIFFER(179v189, graded against the corrected 189-byte extent) -- ours CSEs the GD3DFormats row address; retail re-reads every channel bit count with absolute [reg+0x129BA4C] addressing
// Leaf: ?ConvertRGBToPFDData@CPixelFormat@@QBEXABVCRGBColour@@PAE@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009E3F60, 189 bytes, __thiscall, `ret 8`. FableWin.pdb ?ConvertRGBToPFDData@CPixelFormat@@QBEXABVCRGBColour@@PAE@Z.
// Callees: none (the channel accessors are inline; memcpy is the intrinsic `rep movsd / rep movsb`). Data: GD3DFormats 0x0129BA40
//   (36-byte CD3DFormatInfo rows: BitDepth +8, AlphaBits +0xC, RBits +0x10, GBits +0x14, BBits +0x18).
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CPIXELFORMAT_CONVERT
#include "engine/CPixelFormat.h"
#include "engine/CRGBColour.h"
#include <string.h>

void CPixelFormat::ConvertRGBToPFDData(const CRGBColour& colour, unsigned char* data) const
{
    unsigned long value = ((colour.A >> GetAScaleShift()) << GetAPosShift()) + ((colour.R >> GetRScaleShift()) << GetRPosShift())
                        + ((colour.G >> GetGScaleShift()) << GetGPosShift()) + ((colour.B >> GetBScaleShift()) << GetBPosShift());
    memcpy(data, &value, GetByteColourDepth());
}
