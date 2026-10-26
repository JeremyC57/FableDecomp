// CRGBFloatColour::CRGBFloatColour(const CRGBColour& colour) -- converts a packed 8-bit BGRA colour to four 0..1 floats
// Retail: Fable.exe (TLC, VC7.1) @ 0x0043E470, 91 bytes, `ret 4`. No direct callers (inlined almost everywhere).
// Layout (Ego_r.pdb, struct_layouts_egor.tsv): CRGBColour is a 4-byte union whose bytes are B, G, R, A (D3DCOLOR order);
// CRGBFloatColour is R, G, B, A floats. The scale 1/255 is the float pool 0x01231724.
// Shape: each byte widened with `movzx` and `fild`ed through the argument slot; the four products sit on the x87 stack
// (A, B, G, R computed in that order: the right-to-left arguments of an inline four-float setter) and are popped into
// R, G, B, A. A member-initialiser list stores each product as soon as it is computed.
class CRGBColour { public:
    unsigned char B;                            // +0
    unsigned char G;                            // +1
    unsigned char R;                            // +2
    unsigned char A;                            // +3
};

class CRGBFloatColour { public:
    float R, G, B, A;
    CRGBFloatColour(const CRGBColour& colour);
    void Set(float r, float g, float b, float a) { R = r; G = g; B = b; A = a; }
};

CRGBFloatColour::CRGBFloatColour(const CRGBColour& colour)
{
    Set(colour.R * (1.0f / 255.0f), colour.G * (1.0f / 255.0f), colour.B * (1.0f / 255.0f), colour.A * (1.0f / 255.0f));
}