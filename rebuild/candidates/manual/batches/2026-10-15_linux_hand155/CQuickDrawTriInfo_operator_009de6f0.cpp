// CQuickDrawTriInfo::operator=(const CQuickDrawTriInfo& other) -- memberwise assignment of a 2D quick-draw record (flags, window, string, texture, both union views, string metrics, colour, font)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009DE6F0, length 146 bytes (`ret 4`, returns `this`).
// Label correction: the catalog calls it a CQuickDrawTriInfo constructor; it is the assignment operator (it
// assigns String and UploadedTexture through their operator= and returns `this`). Only caller 0x009DE969.
// Callees: CWideString::operator= @ 0x0099B7D0, CTexture::operator= @ 0x009FA1C0.
// Layout (Ego_r.pdb, CQuickDrawTriInfo 0x3c): bit fields Line (bit 0) and IsText (bit 1) +0, C2DBoxF Window +4,
// CWideString String +0x14, CTexture UploadedTexture +0x18, a union +0x20 of {BlendMode, ZAverage} and
// {StringX, StringY}, StringZ +0x28, StringScale +0x2c, StringFlags +0x30, CRGBColour Colour +0x34, PFont +0x38.
// Shape: user-written: each bit field is assigned on its own (`xor ; and 1/2 ; xor` bit inserts), the window
// through C2DBoxF assignment over addresses (`lea edx,[edi+4] ; lea eax,[esi+4]`), and both union
// views are assigned field by field (so +0x20/+0x24 are copied twice).
// The window copy is C2DBoxF's implicit operator= on a class that declares a user copy constructor: that gives
// the `lea` address form (a user operator= gives four direct moves).
// Leaf: ??4CQuickDrawTriInfo
struct C2DBoxF {
    float TLX, TLY, BRX, BRY;
    C2DBoxF(const C2DBoxF& o) : TLX(o.TLX), TLY(o.TLY), BRX(o.BRX), BRY(o.BRY) {}
};

struct CWideString {
    void* m_pData;
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
};

struct CTexture {
    void* m_p0;
    void* m_p1;
    CTexture& operator=(const CTexture& other);         // 0x009FA1C0
};

struct CRGBColour {
    unsigned char b, g, r, a;
};

class CFontBank;

struct CQuickDrawTriInfo {
    unsigned char Line   : 1;                           // +0x00 bit 0
    unsigned char IsText : 1;                           // +0x00 bit 1
    C2DBoxF     Window;                                 // +0x04
    CWideString String;                                 // +0x14
    CTexture    UploadedTexture;                        // +0x18
    union {                                             // +0x20
        struct { unsigned long BlendMode; float ZAverage; };
        struct { float StringX; float StringY; };
    };
    float         StringZ;                              // +0x28
    float         StringScale;                          // +0x2c
    unsigned long StringFlags;                          // +0x30
    CRGBColour    Colour;                               // +0x34
    CFontBank*    PFont;                                // +0x38
    CQuickDrawTriInfo& operator=(const CQuickDrawTriInfo& other);
};

CQuickDrawTriInfo& CQuickDrawTriInfo::operator=(const CQuickDrawTriInfo& other)
{
    Line = other.Line;
    IsText = other.IsText;
    Window = other.Window;
    String = other.String;
    UploadedTexture = other.UploadedTexture;
    BlendMode = other.BlendMode;
    ZAverage = other.ZAverage;
    StringX = other.StringX;
    StringY = other.StringY;
    StringZ = other.StringZ;
    StringScale = other.StringScale;
    StringFlags = other.StringFlags;
    Colour = other.Colour;
    PFont = other.PFont;
    return *this;
}
