// CScriptScreenText::CScriptScreenText(const CScriptScreenText&) -- memberwise copy: Text (CWideString copy out of line), VPos, Colour (four channel bytes), FontName (CCharString copy out of line), FrameAdded
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0043F490, length 77 bytes.
// Callees: CWideString::CWideString(const CWideString&) @ 0x0099B720, CCharString::CCharString(const
// CCharString&) @ 0x0099EC30. Layout = Ego_r.pdb CScriptScreenText (24 bytes): Text +0, VPos
// (C2DVector) +4, Colour (CRGBColour) +0xc, FontName +0x10, FrameAdded +0x14. The manifest's
// "CCombatAbilityBlockingData::operator=" label is propagated (the body is a copy constructor: the
// CWideString copy constructor is the first call and `this` comes back). The vector's two floats are
// copied as plain dwords (an aggregate copy), the colour as four separate channel bytes that are
// all loaded (into ebx too) before any is stored -- CRGBColour's copy constructor goes through its
// inline four-channel Set(r, g, b, a), whose arguments are evaluated first (a plain aggregate copy
// would be one dword move); the channels sit in D3DCOLOR order in memory (B at +0xc, G, R, A at
// +0xf), which is what gives the retail load/store permutation -- and the copy constructor is
// spelled out so the symbol is emitted out of line.
struct CWideString {
    void* PStringData;
    CWideString(const CWideString& other);                  // 0x0099B720
};

struct CCharString {
    void* PStringData;
    CCharString(const CCharString& other);                  // 0x0099EC30
};

struct C2DVector { float X, Y; };

struct CRGBColour {                                         // the union's channel view: D3DCOLOR byte order
    unsigned char B, G, R, A;
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& other) { Set(other.R, other.G, other.B, other.A); }
};

struct CScriptScreenText {
    CWideString   Text;                                     // +0x00
    C2DVector     VPos;                                     // +0x04
    CRGBColour    Colour;                                   // +0x0c
    CCharString   FontName;                                 // +0x10
    unsigned long FrameAdded;                               // +0x14
    CScriptScreenText(const CScriptScreenText& other);
};

CScriptScreenText::CScriptScreenText(const CScriptScreenText& other)
    : Text(other.Text)
    , VPos(other.VPos)
    , Colour(other.Colour)
    , FontName(other.FontName)
    , FrameAdded(other.FrameAdded)
{
}