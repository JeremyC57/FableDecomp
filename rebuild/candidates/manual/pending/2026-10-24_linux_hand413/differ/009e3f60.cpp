// CPixelFormat::ConvertRGBToPFDData(const CRGBColour&, unsigned char*) -- pack a colour into this format's bit layout and write BitDepth/8 bytes
// Grade: DIFFER(179v189) -- retail re-reads g_D3DFormatInfo[FormatIndex].{A,R,G,B}Bits once per channel (no CSE, no
//   ebp frame); ours CSEs the four bit counts into registers and spills to the frame. Add order and shifts match.
// Retail: Fable.exe @ 0x009E3F60, 189 bytes. PDB: ?ConvertRGBToPFDData@CPixelFormat@@QBEXABVCRGBColour@@PAE@Z.
// Catalog label "Graphics_CopyPixelsToBuffer" is wrong.
// Tried: reference-local expression, accessor expression, accessor statements (this file), inline
//   ConvertRGBToPFD(colour, CPFDColour&) (not inlined). Next: __forceinline ConvertRGBToPFD into a local CPFDColour.
// Leaf: ?ConvertRGBToPFDData@CPixelFormat_Methods@@QBEXABVCRGBColour@@PAE@Z
#include <string.h>
#include "engine/CPixelFormat.h"

class CRGBColour
{
public:
    union {
        struct { unsigned char B, G, R, A; };
        unsigned long Colour;
    };
};

struct CD3DFormatInfo {
    long          D3DFormat;                       // +0x00 _D3DFORMAT
    long          PFType;                          // +0x04 EPixelFormatType
    long          BitDepth;                        // +0x08
    long          AlphaBits;                       // +0x0c
    long          RBits;                           // +0x10
    long          GBits;                           // +0x14
    long          BBits;                           // +0x18
    long          StencilBits;                     // +0x1c
    bool          Linear;                          // +0x20
};

extern const CD3DFormatInfo g_D3DFormatInfo[];     // 0x0129BA40

struct CPixelFormat_Methods : CPixelFormat {
    unsigned long GetNoABits() const { return g_D3DFormatInfo[FormatIndex].AlphaBits; }
    unsigned long GetNoRBits() const { return g_D3DFormatInfo[FormatIndex].RBits; }
    unsigned long GetNoGBits() const { return g_D3DFormatInfo[FormatIndex].GBits; }
    unsigned long GetNoBBits() const { return g_D3DFormatInfo[FormatIndex].BBits; }
    unsigned long GetBPosShift() const { return 0; }
    unsigned long GetGPosShift() const { return GetBPosShift() + GetNoBBits(); }
    unsigned long GetRPosShift() const { return GetGPosShift() + GetNoGBits(); }
    unsigned long GetAPosShift() const { return GetRPosShift() + GetNoRBits(); }
    unsigned long GetAScaleShift() const { return 8 - GetNoABits(); }
    unsigned long GetRScaleShift() const { return 8 - GetNoRBits(); }
    unsigned long GetGScaleShift() const { return 8 - GetNoGBits(); }
    unsigned long GetBScaleShift() const { return 8 - GetNoBBits(); }
    unsigned long GetByteColourDepth() const { return g_D3DFormatInfo[FormatIndex].BitDepth / 8; }
    void ConvertRGBToPFDData(const CRGBColour& colour, unsigned char* data) const;
};

void CPixelFormat_Methods::ConvertRGBToPFDData(const CRGBColour& colour, unsigned char* data) const
{
    unsigned long value = (colour.A >> GetAScaleShift()) << GetAPosShift();
    value += (colour.R >> GetRScaleShift()) << GetRPosShift();
    value += (colour.G >> GetGScaleShift()) << GetGPosShift();
    value += (colour.B >> GetBScaleShift()) << GetBPosShift();
    memcpy(data, &value, GetByteColourDepth());
}
