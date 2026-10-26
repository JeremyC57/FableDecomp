// Behaviour test for CQuickDrawTriInfo's copy constructor (0x009DE890): every field equals the source (both bit
// fields, window, union views, metrics, colour channels, font) and the string/texture members are copy-constructed
// from the source's members.
#include <cstdio>
#include <cstring>
class C2DBoxF { public:
    float TLX, TLY, BRX, BRY;
    C2DBoxF() {}
    C2DBoxF(const C2DBoxF& o) : TLX(o.TLX), TLY(o.TLY), BRX(o.BRX), BRY(o.BRY) {}
};
static int g_stringCopies, g_textureCopies;
class CWideString { public:
    const void* From; void* m_pData;
    CWideString() : From(0), m_pData(0) {}
    CWideString(const CWideString& other) : From(&other), m_pData(other.m_pData) { ++g_stringCopies; }
};
class CTexture { public:
    const void* From; void* m_p1;
    CTexture() : From(0), m_p1(0) {}
    CTexture(const CTexture& other) : From(&other), m_p1(other.m_p1) { ++g_textureCopies; }
};
class CRGBColour { public:
    unsigned char B, G, R, A;
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour& operator=(const CRGBColour& c) { Set(c.R, c.G, c.B, c.A); return *this; }
};
class CFontBank;
class CQuickDrawTriInfo { public:
    unsigned char Line   : 1;
    unsigned char IsText : 1;
    C2DBoxF     Window;
    CWideString String;
    CTexture    UploadedTexture;
    union {
        struct { unsigned long BlendMode; float ZAverage; } m;
        struct { float StringX; float StringY; } s;
    };
    float         StringZ;
    float         StringScale;
    unsigned long StringFlags;
    CRGBColour    Colour;
    CFontBank*    PFont;
    CQuickDrawTriInfo() {}
    CQuickDrawTriInfo(const CQuickDrawTriInfo& other);
};
CQuickDrawTriInfo::CQuickDrawTriInfo(const CQuickDrawTriInfo& other)
    : Line(other.Line), IsText(other.IsText), Window(other.Window), String(other.String),
      UploadedTexture(other.UploadedTexture)
{
    m.BlendMode = other.m.BlendMode;
    m.ZAverage = other.m.ZAverage;
    s.StringX = other.s.StringX;
    s.StringY = other.s.StringY;
    StringZ = other.StringZ;
    StringScale = other.StringScale;
    StringFlags = other.StringFlags;
    Colour = other.Colour;
    PFont = other.PFont;
}
int main()
{
    int bad = 0;
    for (int bits = 0; bits < 4; ++bits) {
        CQuickDrawTriInfo a;
        a.Line = bits & 1; a.IsText = (bits >> 1) & 1;
        a.Window.TLX = 1; a.Window.TLY = 2; a.Window.BRX = 3; a.Window.BRY = 4;
        a.String.m_pData = &a; a.UploadedTexture.m_p1 = &bits;
        a.m.BlendMode = 7; a.m.ZAverage = 0.25f;
        a.StringZ = 5; a.StringScale = 6; a.StringFlags = 0x55;
        a.Colour.R = 10; a.Colour.G = 20; a.Colour.B = 30; a.Colour.A = 40;
        a.PFont = (CFontBank*)&a;
        CQuickDrawTriInfo b(a);
        if (b.Line != (bits & 1) || b.IsText != ((bits >> 1) & 1)) bad |= 1;
        if (b.Window.TLX != 1 || b.Window.TLY != 2 || b.Window.BRX != 3 || b.Window.BRY != 4) bad |= 2;
        if (b.String.From != &a.String || b.String.m_pData != &a || b.UploadedTexture.From != &a.UploadedTexture || b.UploadedTexture.m_p1 != &bits) bad |= 4;
        if (b.m.BlendMode != 7 || b.m.ZAverage != 0.25f || b.StringZ != 5 || b.StringScale != 6 || b.StringFlags != 0x55) bad |= 8;
        if (b.Colour.R != 10 || b.Colour.G != 20 || b.Colour.B != 30 || b.Colour.A != 40 || b.PFont != (CFontBank*)&a) bad |= 16;
    }
    if (g_stringCopies != 4 || g_textureCopies != 4) bad |= 32;
    if (bad) { std::printf("009de890_TEST FAIL\n"); return 1; }
    std::printf("009de890_TEST PASS\n");
    return 0;
}