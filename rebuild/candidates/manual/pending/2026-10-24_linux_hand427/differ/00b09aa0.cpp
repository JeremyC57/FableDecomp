// CRGBFloatColour::GetRGBColourClamped() const -- converts a float RGBA colour to the 8-bit CRGBColour, each channel scaled by 255, rounded and clamped to [0, 255]
// Grade: DIFFER(252v233) -- retail_asm candidate: the four channel conversions are the inline-asm `fstp ; fld ; fistp dword` GFFloatToLong helper, which VC7.1 cannot emit from C++; this is the C++ reference body.
// Leaf: ?GetRGBColourClamped@CRGBFloatColour@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B09AA0, length 233 bytes, thiscall `ret 4` (hidden result pointer).
// Signature from FableWin.pdb: ?GetRGBColourClamped@CRGBFloatColour@@QBE?AVCRGBColour@@XZ. No direct caller in retail.
// Constants: 255.0f @ 0x01230014.
// Layout: CRGBFloatColour R, G, B, A floats; CRGBColour the D3DCOLOR byte order B +0, G +1, R +2, A +3.
// Label: catalog "Color_FloatToRGBA_Clamp" is behavioural.
#include <math.h>

inline long GFFloatToLong(float f) { return (long)floor(f + 0.5f); }   // C++ stand-in for the inline-asm fld/fistp

class CRGBColour {
public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : B(b), G(g), R(r), A(a) { }
};

class CRGBFloatColour {
public:
    float R, G, B, A;
    CRGBColour GetRGBColourClamped() const;
};

CRGBColour CRGBFloatColour::GetRGBColourClamped() const
{
    long r = GFFloatToLong(R * 255.0f);
    long g = GFFloatToLong(G * 255.0f);
    long b = GFFloatToLong(B * 255.0f);
    long a = GFFloatToLong(A * 255.0f);
    if (r < 0) r = 0; else if (r > 255) r = 255;
    if (g < 0) g = 0; else if (g > 255) g = 255;
    if (b < 0) b = 0; else if (b > 255) b = 255;
    if (a < 0) a = 0; else if (a > 255) a = 255;
    return CRGBColour((unsigned char)r, (unsigned char)g, (unsigned char)b, (unsigned char)a);
}
