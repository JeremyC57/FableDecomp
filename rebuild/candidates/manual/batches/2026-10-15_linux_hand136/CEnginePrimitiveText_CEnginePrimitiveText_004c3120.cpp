// CEnginePrimitiveText::CEnginePrimitiveText(unsigned __int64, const C3DVector&, float, const CWideString&, const CFontBank&, const CRGBColour&, bool, EEngineInterpolationSet) -- primitive type 5: position, scale, colour, copied text, font and 3D-scaling flag
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C3120, length 117 bytes.
// Callees: 0x0099B720 CWideString::CWideString(const CWideString&) (the text member's copy constructor).
// Signature from Ego_r.pdb: ??0CEnginePrimitiveText@@QAE@_KABVC3DVector@@MABVCWideString@@ABVCFontBank@@ABVCRGBColour@@_NW4EEngineInterpolationSet@@@Z
// (9 stack words, `ret 24h`); the 64-bit primitive handle is not stored. Layout = PDB CEnginePrimitiveText
// (0x2c): CEnginePrimitive base {Type +0, InterpolationSet +4, ThemeChannelDesc +8}, Pos +0xc, Scale +0x18,
// Colour +0x1c, Text +0x20 (CWideString: one data pointer), PFont +0x24, ScaleIn3D +0x28.
// Shape: base inline constructor (interpolation set argument, type immediate, zero channel bytes), the vector
// copied memberwise, the colour through the inline Set(r, g, b, a) copy (its four byte stores are scheduled
// around the push of the string argument), the out-of-line CWideString copy constructor, then the font
// pointer (a reference parameter stored by address) and the flag; `this` is kept in esi across the call.

struct C3DVector {                                       // memberwise user copy constructor: one register per dword
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};
struct C2DVector {
    float X, Y;
    C2DVector(const C2DVector& other) : X(other.X), Y(other.Y) { }
};
struct CRGBColour {                                      // the union's channel view: D3DCOLOR byte order
    unsigned char B, G, R, A;
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& other) { Set(other.R, other.G, other.B, other.A); }
};

struct CEnginePrimitiveThemeChannelDesc {                // 4 bytes (PDB)
    unsigned char ChannelIndex0, ChannelIndex1, ChannelBlendFactor, Pad;
    CEnginePrimitiveThemeChannelDesc() : ChannelIndex0(0), ChannelIndex1(0), ChannelBlendFactor(0) { }
};
struct CEnginePrimitive {                                // 12 bytes (PDB)
    long                             Type;               // +0x00 EEnginePrimitiveType
    long                             InterpolationSet;   // +0x04 EEngineInterpolationSet
    CEnginePrimitiveThemeChannelDesc ThemeChannelDesc;   // +0x08
    CEnginePrimitive(long type, long interpolationSet = 0) : Type(type), InterpolationSet(interpolationSet) { }
};

struct CWideString {                                     // 4 bytes: PStringData
    void* PStringData;
    CWideString(const CWideString& other);               // 0x0099B720
};
struct CFontBank;

struct CEnginePrimitiveText : CEnginePrimitive {
    C3DVector   Pos;                                     // +0x0c
    float       Scale;                                   // +0x18
    CRGBColour  Colour;                                  // +0x1c
    CWideString Text;                                    // +0x20
    CFontBank*  PFont;                                   // +0x24
    bool        ScaleIn3D;                               // +0x28   sizeof == 0x2c
    CEnginePrimitiveText(unsigned __int64 handle, const C3DVector& pos, float scale, const CWideString& text,
                         const CFontBank& font, const CRGBColour& colour, bool scaleIn3D, long interpolationSet);
};

CEnginePrimitiveText::CEnginePrimitiveText(unsigned __int64 handle, const C3DVector& pos, float scale,
                                           const CWideString& text, const CFontBank& font,
                                           const CRGBColour& colour, bool scaleIn3D, long interpolationSet)
    : CEnginePrimitive(5, interpolationSet)              // ENGINE_PRIMITIVE_TEXT
    , Pos(pos)
    , Scale(scale)
    , Colour(colour)
    , Text(text)
    , PFont(const_cast<CFontBank*>(&font))
    , ScaleIn3D(scaleIn3D)
{
}
