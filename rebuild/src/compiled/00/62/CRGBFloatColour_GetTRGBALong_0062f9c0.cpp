// CRGBFloatColour::GetTRGBALong() const -- pack a float colour (components in 0..1) into a 32-bit A8R8G8B8 word: each component scaled by 255 and truncated, alpha in the top byte
// Leaf: GetTRGBALong
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0062F9C0, 79 bytes, plain `ret` (thiscall, no arguments).
// Identity: Ego_r.pdb names `?GetTRGBALong@CRGBFloatColour@@QBEKXZ` (unsigned long, const); the body reads the four
// floats of `this` at +0/+4/+8/+0xc (PDB CRGBFloatColour R, G, B, A). No code references this copy (an out-of-line
// COMDAT of the inline member). The catalog label "PackARGBColor" is descriptive.
// Callees: __ftol2 @ 0x00BFEA70 (four float-to-integer conversions); the 255.0f pool constant @ 0x01230014.
// Shape: `((((A*255) << 8 | R*255) << 8 | G*255) << 8) | B*255` as one expression (the same packing the landed
// CEngineLightingManager::SetupDefaultFogStates 0x00B47630 inlines): `x * 255.0f` loads x first, the accumulated word
// lives in edi and the last conversion is or-ed into eax.
class CRGBFloatColour { public:
    float R;                                       // +0x00
    float G;                                       // +0x04
    float B;                                       // +0x08
    float A;                                       // +0x0c
    unsigned long GetTRGBALong() const;
};

unsigned long CRGBFloatColour::GetTRGBALong() const
{
    return (((((long)(A * 255.0f) << 8) | (long)(R * 255.0f)) << 8 | (long)(G * 255.0f)) << 8) | (long)(B * 255.0f);
}