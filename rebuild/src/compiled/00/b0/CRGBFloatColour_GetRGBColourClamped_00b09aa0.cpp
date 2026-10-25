// CRGBFloatColour::GetRGBColourClamped() const -- converts a float RGBA colour to the 8-bit CRGBColour, each channel scaled by 255, rounded and clamped to [0, 255]
// Grade: RELOCATION_MATCH (2 grades: CRGBColour ctor assigns in its body, per GOTCHAS 0052f440) -- rasm1 retail_asm: C++ body + a minimal 2-instruction __asm (no control flow) + the Retail-asm evidence line
// Retail-asm: 0x00B09AB6, 0x00B09AD1, 0x00B09AEE, 0x00B09B09 `fistp dword ptr [esp+..]` each after `fstp [slot] ; fld [slot]` -- four expansions of the engine's inline-assembly GFFloatToLong helper (by-value copy stored and reloaded, bare dword fistp). VC7.1 C++ `(long)x` calls __ftol2 and /QIfist (RTM 3077 and QFE 4035, with /Op, /Og or /G5-/G7) emits `fistp qword` into an 8-byte slot, /arch:SSE2 a cvttss2si: no C/C++ form gives the bare dword fistp, so it is the original's `__asm`.
// Leaf: ?GetRGBColourClamped@CRGBFloatColour@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B09AA0, length 233 bytes, thiscall `ret 4` (hidden result pointer).
// Signature from FableWin.pdb: ?GetRGBColourClamped@CRGBFloatColour@@QBE?AVCRGBColour@@XZ. No direct caller in retail.
// Constants: 255.0f @ 0x01230014.
// Layout: CRGBFloatColour R, G, B, A floats; CRGBColour the D3DCOLOR byte order B +0, G +1, R +2, A +3.
// Label: catalog "Color_FloatToRGBA_Clamp" is behavioural.
// the engine's inline-assembly float-to-long helper (round to nearest, the FPU default); expanded in place as a __forceinline
__forceinline long GFFloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}

class CRGBColour {
public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
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