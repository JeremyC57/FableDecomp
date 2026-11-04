// CPixelFormat::ConvertPFDToRGB(const CPFDColour& in, CRGBColour& out) const -- unpack a packed pixel value of this format into an 8-bit-per-channel colour: each channel is masked out at its offset (blue lowest, then green, red, alpha) and scaled up to 8 bits by a left shift
// Grade: DIFFER(252v280) -- dl=83: retail reloads each GD3DFormats[FormatIndex] field per channel (no CSE) where ours CSEs the format row; 28 bytes short
// Leaf: ?ConvertPFDToRGB@CPixelFormat@@QBEXABVCPFDColour@@AAVCRGBColour@@@Z
class CD3DFormatInfo { public: long D3DFormat; long PFType; long BitDepth, AlphaBits, RBits, GBits, BBits, StencilBits; bool Linear; };
extern const CD3DFormatInfo GD3DFormats[];           // 0x0129BA40
class CPFDColour { public: unsigned long Value; };
class CRGBColour { public: unsigned char B, G, R, A; };
class CPixelFormat {
protected: long FormatIndex;
public:
 unsigned long GetNoABits() const {return GD3DFormats[FormatIndex].AlphaBits;}
 unsigned long GetNoRBits() const {return GD3DFormats[FormatIndex].RBits;}
 unsigned long GetNoGBits() const {return GD3DFormats[FormatIndex].GBits;}
 unsigned long GetNoBBits() const {return GD3DFormats[FormatIndex].BBits;}
 unsigned long GetBPosShift() const {return 0;}
 unsigned long GetGPosShift() const {return GetNoBBits();}
 unsigned long GetRPosShift() const {return GetNoBBits()+GetNoGBits();}
 unsigned long GetAPosShift() const {return GetRPosShift()+GetNoRBits();}
 unsigned long GetAMask() const {return ((1UL<<GetNoABits())-1)<<GetAPosShift();}
 unsigned long GetRMask() const {return ((1UL<<GetNoRBits())-1)<<GetRPosShift();}
 unsigned long GetGMask() const {return ((1UL<<GetNoGBits())-1)<<GetGPosShift();}
 unsigned long GetBMask() const {return ((1UL<<GetNoBBits())-1)<<GetBPosShift();}
 unsigned long GetAScaleShift() const {return 8-GetNoABits();}
 unsigned long GetRScaleShift() const {return 8-GetNoRBits();}
 unsigned long GetGScaleShift() const {return 8-GetNoGBits();}
 unsigned long GetBScaleShift() const {return 8-GetNoBBits();}
 void ConvertPFDToRGB(const CPFDColour& in, CRGBColour& out) const;
};
void CPixelFormat::ConvertPFDToRGB(const CPFDColour& in, CRGBColour& out) const
{
    unsigned long value = in.Value;
    unsigned char a = (unsigned char)(((value & GetAMask()) >> GetAPosShift()) << GetAScaleShift());
    out.R = (unsigned char)(((value & GetRMask()) >> GetRPosShift()) << GetRScaleShift());
    out.G = (unsigned char)(((value & GetGMask()) >> GetGPosShift()) << GetGScaleShift());
    out.A = a;
    out.B = (unsigned char)(((value & GetBMask()) >> GetBPosShift()) << GetBScaleShift());
}
