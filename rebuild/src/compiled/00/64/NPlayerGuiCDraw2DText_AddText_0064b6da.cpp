// NPlayerGui::CDraw2DText::AddText(const C2DVector&, const CWideString&, CFontBank*, const CRGBColour&, float, EEngine2DLayer, bool) -- round the
// position to the nearest pixel and convert it to absolute coordinates; with `shadow`, first queue three black (source alpha) copies one layer
// up at (+1,+1), (+1,-1) and (-1,+1); then queue the text itself.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064B6DA, length 473 bytes, `ret 1Ch`.
// Callees: GFRoundVXToNearestPixel 0x009E1D00 / GFRoundVYToNearestPixel 0x009E1D50 (__fastcall, double), GFVCoordToACoord 0x009E22C0
// (__fastcall), CEnginePrimitive2DText(unsigned __int64, const C2DVector&, EEngine2DLayer, float, const CWideString&, CFontBank*, const CRGBColour&,
// EEngineInterpolationSet) 0x00543910 (manifest signature), list push 0x006561C8 on the text list at +0x28 (manifest "ListPushFrontWrapper"),
// ~CWideString 0x0099B510 (the inline ~CEnginePrimitive2DText, text member at +0x24). Constant 1.0f 0x0122DED8.
// Layout: CEnginePrimitive2DText 0x2c bytes as the landed copy constructor 0x00655048; CDraw2DText text list +0x28 (descriptive).
// Shape: size TU with only `#pragma optimize("s", on)` (biased `lea ebp,[esp-5Ch]` frame); one shadow position local ASSIGNED from
// `pos + C2DVector(...)` each time (the operator+ temporary is copied through eax); the shadow colour a CRGBColour temporary in the
// constructor call (homed in the dead `shadow` slot).
// Leaf: AddText
#pragma optimize("s", on)
class C2DVector { public:
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector operator+(const C2DVector& o) const { return C2DVector(X + o.X, Y + o.Y); }
};
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
class CWideString { public:
    void* m_pData;
    ~CWideString();                                // 0x0099B510
};
class CFontBank;
enum EEngine2DLayer { ENGINE_2D_LAYER_0 = 0 };
enum EEngineInterpolationSet { ENGINE_INTERPOLATION_SET_0 = 0 };
float __fastcall GFRoundVXToNearestPixel(double x);    // 0x009E1D00
float __fastcall GFRoundVYToNearestPixel(double y);    // 0x009E1D50
void __fastcall GFVCoordToACoord(const C2DVector& v, C2DVector& a);   // 0x009E22C0

class CEnginePrimitive2DText { public:                    // 0x2c bytes
    unsigned char _header[0x0c];
    C2DVector     m_position;                      // +0x0c
    unsigned long m_userData;                      // +0x14
    long          m_layer;                         // +0x18
    float         m_depth;                         // +0x1c
    unsigned long m_colour;                        // +0x20
    CWideString   m_text;                          // +0x24
    CFontBank*    m_pFont;                         // +0x28
    CEnginePrimitive2DText(unsigned __int64 id, const C2DVector& pos, EEngine2DLayer layer, float depth,
                           const CWideString& text, CFontBank* font, const CRGBColour& colour,
                           EEngineInterpolationSet interpolation);   // 0x00543910
};
struct CTextList {
    void push_back(const CEnginePrimitive2DText& text);   // 0x006561C8
};

namespace NPlayerGui {
class CDraw2DText { public:
    unsigned char _pad00[0x28];
    CTextList     Texts;                           // +0x28
    void AddText(const C2DVector& position, const CWideString& text, CFontBank* font, const CRGBColour& colour,
                 float depth, EEngine2DLayer layer, bool shadow);
};

void CDraw2DText::AddText(const C2DVector& position, const CWideString& text, CFontBank* font, const CRGBColour& colour,
                          float depth, EEngine2DLayer layer, bool shadow)
{
    C2DVector rounded = position;
    rounded.X = GFRoundVXToNearestPixel(rounded.X);
    rounded.Y = GFRoundVYToNearestPixel(rounded.Y);
    C2DVector pos;
    GFVCoordToACoord(rounded, pos);
    if (shadow) {
        C2DVector shadowPos;
        shadowPos = pos + C2DVector(1.0f, 1.0f);
        CEnginePrimitive2DText t1(0, shadowPos, (EEngine2DLayer)(layer + 1), depth, text, font, CRGBColour(0, 0, 0, colour.A), ENGINE_INTERPOLATION_SET_0);
        Texts.push_back(t1);
        shadowPos = pos + C2DVector(1.0f, -1.0f);
        CEnginePrimitive2DText t2(0, shadowPos, (EEngine2DLayer)(layer + 1), depth, text, font, CRGBColour(0, 0, 0, colour.A), ENGINE_INTERPOLATION_SET_0);
        Texts.push_back(t2);
        shadowPos = pos + C2DVector(-1.0f, 1.0f);
        CEnginePrimitive2DText t3(0, shadowPos, (EEngine2DLayer)(layer + 1), depth, text, font, CRGBColour(0, 0, 0, colour.A), ENGINE_INTERPOLATION_SET_0);
        Texts.push_back(t3);
    }
    CEnginePrimitive2DText t(0, pos, layer, depth, text, font, colour, ENGINE_INTERPOLATION_SET_0);
    Texts.push_back(t);
}
}