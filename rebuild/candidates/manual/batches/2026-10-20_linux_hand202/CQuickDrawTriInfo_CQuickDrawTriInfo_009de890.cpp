// CQuickDrawTriInfo::CQuickDrawTriInfo(const CQuickDrawTriInfo&) -- copy constructor of the 2D quick-draw record: flags, window, string and texture constructed from the other's, the rest assigned (both union views, string metrics, colour, font)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009DE890, length 162 bytes, `ret 4` (returns `this`).
// Leaf: ??0CQuickDrawTriInfo
// The sibling of the landed operator= (0x009DE6F0) in the same TU, written the same way by hand.
// Callees: CWideString::CWideString(const CWideString&) 0x0099B720, CTexture::CTexture(const CTexture&) 0x009F9D00.
// Layout (Ego_r.pdb, CQuickDrawTriInfo 0x3c): bit fields Line (bit 0) and IsText (bit 1) +0, C2DBoxF Window +4,
// CWideString String +0x14, CTexture UploadedTexture +0x18, a union +0x20 of {BlendMode, ZAverage} and
// {StringX, StringY}, StringZ +0x28, StringScale +0x2c, StringFlags +0x30, CRGBColour Colour +0x34, PFont +0x38.
// Shape: the initialiser list takes the bit fields (read-modify-write bit inserts on the fresh byte), the window
// (C2DBoxF's memberwise user copy constructor: four direct moves) and the string/texture copy constructors; the body
// assigns both union views field by field (+0x20/+0x24 copied twice) and the colour through CRGBColour's inline
// Set(r, g, b, a) assignment (all four bytes loaded before any store).
struct C2DBoxF {
    float TLX, TLY, BRX, BRY;
    C2DBoxF(const C2DBoxF& o) : TLX(o.TLX), TLY(o.TLY), BRX(o.BRX), BRY(o.BRY) {}
};

struct CWideString {
    void* m_pData;
    CWideString(const CWideString& other);              // 0x0099B720
};

struct CTexture {
    void* m_p0;
    void* m_p1;
    CTexture(const CTexture& other);                    // 0x009F9D00
};

struct CRGBColour {
    unsigned char B, G, R, A;
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour& operator=(const CRGBColour& c) { Set(c.R, c.G, c.B, c.A); return *this; }
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
    CQuickDrawTriInfo(const CQuickDrawTriInfo& other);
};

CQuickDrawTriInfo::CQuickDrawTriInfo(const CQuickDrawTriInfo& other)
    : Line(other.Line), IsText(other.IsText), Window(other.Window), String(other.String),
      UploadedTexture(other.UploadedTexture)
{
    BlendMode = other.BlendMode;
    ZAverage = other.ZAverage;
    StringX = other.StringX;
    StringY = other.StringY;
    StringZ = other.StringZ;
    StringScale = other.StringScale;
    StringFlags = other.StringFlags;
    Colour = other.Colour;
    PFont = other.PFont;
}
