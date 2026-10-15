// CRGBFloatColour::GetRGBColour() const -- convert a float colour (components in 0..1) to the packed 8-bit CRGBColour by scaling each component by 255 and truncating
// Leaf: GetRGBColour
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00429880, length 105 bytes (`ret 4`: the hidden result pointer).
// Label correction: the manifest's "CDataOutputStream::WriteColour" is propagated; the body reads four floats at +0/+4/+8/+0xc
// of `this` (Ego_r.pdb CRGBFloatColour: R, G, B, A) and builds a 4-byte colour in the caller's result.
// Callees: __ftol2 @ 0x00BFEA70 (four float-to-integer conversions), the 255.0f pool constant @ 0x01230014.
// Shape: the CRGBColour(r, g, b, a) constructor call evaluates its arguments right to left (A first, `fld [esi+0Ch] ;
// fmul [255.0]`: `x * 255.0f` loads x first), parking two converted bytes in the frame and one in bl; its body stores R, G,
// B, A (VC schedules the A store before B). The D3DCOLOR byte order is B, G, R, A.
struct CRGBColour {                                // D3DCOLOR byte order
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};

struct CRGBFloatColour {
    float R;                                       // +0x00
    float G;                                       // +0x04
    float B;                                       // +0x08
    float A;                                       // +0x0c
    CRGBColour GetRGBColour() const;
};

CRGBColour CRGBFloatColour::GetRGBColour() const
{
    return CRGBColour((unsigned char)(R * 255.0f), (unsigned char)(G * 255.0f), (unsigned char)(B * 255.0f),
                      (unsigned char)(A * 255.0f));
}