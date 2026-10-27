// CRGBFloatColour::operator*=(float scale) -- scale all four channels (R, G, B, A) by one factor; returns *this
// Leaf: ??XCRGBFloatColour@@QAEAAV0@M@Z
// Retail: Fable.exe @ 0x00405F70, 43 bytes, __fastcall `ret 4`: four `fld [scale] ; fmul [this+n] ; fstp [this+n]`.
// Identity (probable): the PDB name sets hold exactly two float compound-multiplies, ??XCRGBFloatColour@@QAEAAV0@M@Z and
// ??XcfVector@@QAEAAV0@M@Z; CRGBFloatColour is the 16-byte R,G,B,A float class (Ego_r layout), matching the four-float
// body. The body is an inline COMDAT with no direct caller in retail (gap-scan function).
class CRGBFloatColour { public:
    float R, G, B, A;
    CRGBFloatColour& operator*=(float scale);
};

CRGBFloatColour& CRGBFloatColour::operator*=(float scale)
{
    R *= scale;
    G *= scale;
    B *= scale;
    A *= scale;
    return *this;
}