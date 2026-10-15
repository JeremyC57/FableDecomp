// CEnvironmentLookupTexture::LookupFloatColour(long x, long y) const -- read the 32-bit BGRA texel at (x, y) and return it as a float colour, each channel mapped to the centre of its 1/255 step ((c + 0.5) / 255)
// Retail: Fable.exe (TLC, VC7.1) @ 0x008860D0, 134 bytes (+ int3 padding).
// Callees: none. Double pool: 0x012316F0 = 0.5, 0x01278088 = 1/255 (0.00392156862745098).
// Layout: local model -- the row pitch in bytes at +0x08 and the texel data pointer at +0x0c (4 bytes per texel).
// Shape: the colour comes back through the hidden pointer (`ret 0Ch`, eax = the slot) because CRGBFloatColour has a user
//   constructor; VC evaluates its four arguments right to left, so alpha (byte 3) is converted first, then blue (byte 0),
//   green (byte 1), red (byte 2), and the four values leave the FPU stack R, G, B, A; each byte goes through `fild` of a
//   dword temporary homed in the dead `y` argument slot, with the double constants (`fadd qword ; fmul qword`).
struct CRGBFloatColour {
    float R, G, B, A;
    CRGBFloatColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) {}
};

struct CEnvironmentLookupTexture {
    unsigned char  _pad0[0x8];
    long           m_pitch;                      // +0x08 bytes per row
    unsigned char* m_pData;                      // +0x0c BGRA texels
    static float ByteToFloat(unsigned char c) { return (float)((c + 0.5) * (1.0 / 255.0)); }
    CRGBFloatColour LookupFloatColour(long x, long y) const;
};

CRGBFloatColour CEnvironmentLookupTexture::LookupFloatColour(long x, long y) const
{
    const unsigned char* texel = m_pData + (m_pitch * y + x * 4);
    return CRGBFloatColour(ByteToFloat(texel[2]), ByteToFloat(texel[1]), ByteToFloat(texel[0]), ByteToFloat(texel[3]));
}
