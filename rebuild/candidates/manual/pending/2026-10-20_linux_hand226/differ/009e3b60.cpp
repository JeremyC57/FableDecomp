// CPixelFormat::ConvertPFDToRGB(const CPFDColour& in, CRGBColour& out) const -- unpack a packed pixel value of this format into an 8-bit-per-channel colour: each channel is masked out at its offset (blue lowest, then green, red, alpha) and scaled up to 8 bits by a left shift
// Grade: DIFFER(199v280) -- ours CSEs the four table fields into registers; retail re-reads [entry+0Ch..18h] for every channel (twelve table loads, the value homed in the dead `in` slot, the alpha byte parked in a frame word) with the stores R, G, A, B
// Retail: Fable.exe (TLC, VC7.1) @ 0x009E3B60, 280 bytes, `ret 8`. PDB: ?ConvertPFDToRGB@CPixelFormat@@QBEXABVCPFDColour@@AAVCRGBColour@@@Z.
// Globals: the 36-byte CD3DFormatInfo table 0x0129BA40 indexed by FormatIndex (AlphaBits +0x0C, RBits +0x10, GBits +0x14,
//   BBits +0x18), as the landed CDisplayManager format rows.
// Layout: engine/CPixelFormat.h (FormatIndex +0); CRGBColour bytes B +0, G +1, R +2, A +3; CPFDColour one dword.
// Shape: one inline channel extractor per channel, arguments of a (b, g, r, a) colour constructor evaluated right to left.
struct CD3DFormatInfo {                            // 36-byte entry of the format table
    long D3DFormat;                                // +0x00 _D3DFORMAT
    long PFType;                                   // +0x04
    long BitDepth;                                 // +0x08
    long AlphaBits;                                // +0x0c
    long RBits;                                    // +0x10
    long GBits;                                    // +0x14
    long BBits;                                    // +0x18
    long StencilBits;                              // +0x1c
    bool Linear;                                   // +0x20
};
extern const CD3DFormatInfo g_D3DFormatInfo[];     // 0x0129BA40

struct CPFDColour { unsigned long Value; };
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char b, unsigned char g, unsigned char r, unsigned char a) : B(b), G(g), R(r), A(a) {}
};

static inline unsigned char GetChannel(unsigned long value, long shift, long bits)
{
    return (unsigned char)(((value & (((1 << bits) - 1) << shift)) >> shift) << (8 - bits));
}

struct CPixelFormat {
    long FormatIndex;                              // +0
    void ConvertPFDToRGB(const CPFDColour& in, CRGBColour& out) const;
};

void CPixelFormat::ConvertPFDToRGB(const CPFDColour& in, CRGBColour& out) const
{
    const CD3DFormatInfo& info = g_D3DFormatInfo[FormatIndex];
    unsigned long value = in.Value;
    out = CRGBColour(GetChannel(value, 0, info.BBits),
                     GetChannel(value, info.BBits, info.GBits),
                     GetChannel(value, info.BBits + info.GBits, info.RBits),
                     GetChannel(value, info.BBits + info.GBits + info.RBits, info.AlphaBits));
}
