// CRGBFloatColour::Interpolate(const CRGBFloatColour& colour, float t) const -- linear blend of two float RGBA colours, this * (1 - t) + colour * t
// Leaf: ?Interpolate@CRGBFloatColour@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B09B90, length 132 bytes, thiscall `ret 0Ch` (hidden result pointer,
//   &colour, t), the result pointer returned in eax.
// Signature from FableWin.pdb: ?Interpolate@CRGBFloatColour@@QBE?AV1@ABV1@M@Z (Ego_r.pdb has the CRightHandedSet twin
//   only). No direct caller in retail (an out-of-line copy of the header inline in the particle engine's TU).
// Callees: none; reads the 1.0f literal @ 0x0122DED8.
// Layout (Ego_r.pdb): CRGBFloatColour = float R +0, G +4, B +8, A +0x0C.
// Shape: `return *this * (1.0f - t) + colour * t;` with the member operator* / operator+ inline -- VC evaluates the
//   operator+ argument (colour * t) first, keeping R and G on the x87 stack and parking B and A in its temporary, then
//   the scaled copy of *this, then the four sums.
// Label: catalog "InterpolateRadialBlurPositionParams" (__fastcall) is a guess -- this is CRGBFloatColour::Interpolate.
class CRGBFloatColour {
public:
    float R;                                                // +0x00
    float G;                                                // +0x04
    float B;                                                // +0x08
    float A;                                                // +0x0C
    CRGBFloatColour() { }
    CRGBFloatColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) { }
    CRGBFloatColour operator*(float f) const { return CRGBFloatColour(R * f, G * f, B * f, A * f); }
    CRGBFloatColour operator+(const CRGBFloatColour& c) const { return CRGBFloatColour(R + c.R, G + c.G, B + c.B, A + c.A); }
    CRGBFloatColour Interpolate(const CRGBFloatColour& colour, float t) const;
};

CRGBFloatColour CRGBFloatColour::Interpolate(const CRGBFloatColour& colour, float t) const
{
    return *this * (1.0f - t) + colour * t;
}