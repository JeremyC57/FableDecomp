// CPixelFormat::ConvertPFDToRGB(const CPFDColour& pfd, CRGBColour& rgb) const -- unpack a packed pixel value of this format into an
// 8-bit-per-channel colour: each channel is masked out at its position and scaled up to 8 bits.
// Leaf: ?ConvertPFDToRGB@CPixelFormat@@QBEXABVCPFDColour@@AAVCRGBColour@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009E3B60, 280 bytes (`ret 8`).
// Naming: FableWin.pdb / Ego_r.pdb ?ConvertPFDToRGB@CPixelFormat@@QBEXABVCPFDColour@@AAVCRGBColour@@@Z; the channel helpers are the
//   FableWin.pdb inline accessors GetXMask / GetXPosShift / GetXScaleShift / GetNoXBits and CRGBColour::Initialise(r, g, b, a).
// Globals: GD3DFormats 0x0129BA40 (36-byte CD3DFormatInfo entries; AlphaBits +0x0C, RBits +0x10, GBits +0x14, BBits +0x18).
// Shape: every channel is ((pfd.GetLong() & mask) >> pos) << (8 - bits), narrowed to a byte. The position shifts are the debug
//   bodies' local-reference field sums (FABLE_RETAIL_CPIXELFORMAT_POSSHIFT_FORMATREF): VC7.1 does not share those loads with the
//   accessor reads, hence retail's twelve table loads. FormatIndex is read before the packed value (0x009E3B63), so the value is
//   read inside each channel expression, not into a local first; alpha (the last Initialise argument) is evaluated first and
//   parked in a frame word, then red, green and blue.
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CPIXELFORMAT_CONVERT
#define FABLE_RETAIL_CPIXELFORMAT_PFDTORGB
#define FABLE_RETAIL_CPIXELFORMAT_POSSHIFT_FORMATREF
#define FABLE_RETAIL_CRGBCOLOUR
#include "engine/CRGBColour.h"
#include "engine/CPixelFormat.h"
// No shared header: FableWin.pdb CPFDColour (??0CPFDColour@@QAE@XZ, ?GetLong@CPFDColour@@QBEJXZ, ?SetLong@CPFDColour@@QAEXJ@Z), one dword.
class CPFDColour {
public:
    long GetLong() const { return Long; }
private:
    long Long;   // made-up descriptive name: Ego_r.pdb has only an unnamed union at +0 (CPFDColour_u_0); GetLong / SetLong are the PDB accessors
};

void CPixelFormat::ConvertPFDToRGB(const CPFDColour& pfd,CRGBColour& rgb) const
{
    rgb.Initialise((unsigned char)(((pfd.GetLong()&GetRMask())>>GetRPosShift())<<GetRScaleShift()),
                   (unsigned char)(((pfd.GetLong()&GetGMask())>>GetGPosShift())<<GetGScaleShift()),
                   (unsigned char)(((pfd.GetLong()&GetBMask())>>GetBPosShift())<<GetBScaleShift()),
                   (unsigned char)(((pfd.GetLong()&GetAMask())>>GetAPosShift())<<GetAScaleShift()));
}