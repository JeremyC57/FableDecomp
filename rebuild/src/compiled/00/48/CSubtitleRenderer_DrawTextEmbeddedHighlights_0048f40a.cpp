// CSubtitleRenderer::DrawTextEmbeddedHighlights(const CEnginePrimitivePrepareInfo& info, const C2DBoxF& box) -- draw every embedded highlight whose character has been revealed, at its offset from the box corner (virtual -> actual -> virtual coordinates), in the GUI def's game-action text colour
// Leaf: ?DrawTextEmbeddedHighlights@CSubtitleRenderer@@AAEXABVCEnginePrimitivePrepareInfo@@ABVC2DBoxF@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0048F40A, 258 bytes, `ret 8` (the prepare info is never read). PDB spelling (Ego_r /
//   FableWin name sets): ?DrawTextEmbeddedHighlights@CSubtitleRenderer@@AAEXABVCEnginePrimitivePrepareInfo@@ABVC2DBoxF@@@Z.
// Callees: CPlayerGui::PeekGuiDef 0x00438570 (static __fastcall, no `this` set up), GFVYToAY 0x009E1E40 / GFVXToAX 0x009E1E10
//   (__fastcall, float on the stack: `fild` render-target size `fmul x` `fdiv` a virtual-size double = virtual to actual; the PDB orders
//   GFAXToVX, GFAYToVY, GFVXToAX, GFVYToAY as retail 0x009E1DA0, 0x009E1DD0, 0x009E1E10, 0x009E1E40),
//   GFACoordToVCoord(const C2DVector&, C2DVector&) 0x009E21D0 (__fastcall), CSubtitleRenderer::Draw2DText 0x0048F1F4
//   (landed). Float constant 2.0f 0x0122DCDC.
// Layout: engine/CSubtitleRenderer.h (retail STLport offsets: TextCharactersShowing +0x24, PFont +0x80, EmbeddedHighlights
//   +0xC4 as an opaque 12-byte vector of 20-byte CEmbeddedHighlight {Text, CharIndex, Offset, Split}, AllCharactersDisplayed
//   +0xDD). The GUI def colour at retail +0x5BC is Ego_r +0x5F8 TextBoxTextGameActionColour: the same 0x3C shift (15
//   Dinkumware vectors 16 -> 12 bytes) the landed DrawTextEmbeddedGraphics proves for ControllerGraphicOffset
//   (+0x694 v +0x6D0), with no container between the two members.
// Shape: the frame-pointer size TU of the landed neighbours (`push 14h ; pop ecx ; idiv` sizes, leave). The vector is
//   re-read for every element access; GFVYToAY runs first into a named float local parked in the dead box slot.
//   Lever (chunk 609): C2DVector's operator+ builds a named `result` and returns it through the user copy
//   constructor (no NRVO in VC7.1): X is summed first in place on the GFVXToAX value, then Y, and the copy into the
//   GFACoordToVCoord temporary gives retail's `fld st(1) ; fstp X ; fst Y ; fstp st(0) ; fstp st(0)` with the two argument
//   leas between the fadds. `return C2DVector(X + o.X, Y + o.Y)` sums Y first (right-to-left constructor arguments).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define CSubtitleRenderer CSubtitleRendererLayout
#include "engine/CSubtitleRenderer.h"
#undef CSubtitleRenderer
#define CPlayerGui CPlayerGuiLayout
#include "engine/CPlayerGui.h"
#undef CPlayerGui

class C2DVector {
public:
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) { }
    C2DVector operator+(const C2DVector& o) const
    {
        C2DVector result;
        result.X = X + o.X;
        result.Y = Y + o.Y;
        return result;
    }
};
class C2DBoxF { public: float TLX, TLY, BRX, BRY; };
class CRGBColour { public: unsigned char B, G, R, A; };
class CWideString { public: CWideStringData* PStringData; };
class CEnginePrimitivePrepareInfo;

class CPlayerGuiDef {
public:
    unsigned char _pad000[0x5bc];
    CRGBColour    TextBoxTextGameActionColour;     // +0x5BC (Ego_r +0x5F8)
};
class CPlayerGui : public CPlayerGuiLayout {
public:
    static const CPlayerGuiDef& __fastcall PeekGuiDef();   // 0x00438570
};
float __fastcall GFVYToAY(float y);                // 0x009E1E40
float __fastcall GFVXToAX(float x);                // 0x009E1E10
void __fastcall GFACoordToVCoord(const C2DVector& in, C2DVector& out);   // 0x009E21D0

template <class T> struct CRetailVector {          // STLport vector: 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    bool empty() const { return _M_start == _M_finish; }
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long n) { return *(begin() + n); }
};

class CSubtitleRenderer : public CSubtitleRendererLayout {
public:
    class CEmbeddedHighlight {
    public:
        CWideString Text;                          // +0x00
        long        CharIndex;                     // +0x04
        C2DVector   Offset;                        // +0x08
        bool        Split;                         // +0x10
    };
    CRetailVector<CEmbeddedHighlight>& GetEmbeddedHighlights()
    {
        return *(CRetailVector<CEmbeddedHighlight>*)EmbeddedHighlights;
    }
private:
    void Draw2DText(const C2DVector& pos, const CWideString& text, const CFontBank& font, const CRGBColour& colour,
                    bool unused, float scaleX, float scaleY, bool raise);   // 0x0048F1F4
    void DrawTextEmbeddedHighlights(const CEnginePrimitivePrepareInfo& info, const C2DBoxF& box);
};

void CSubtitleRenderer::DrawTextEmbeddedHighlights(const CEnginePrimitivePrepareInfo& info, const C2DBoxF& box)
{
    if (GetEmbeddedHighlights().empty())
        return;
    const CPlayerGuiDef& def = CPlayerGui::PeekGuiDef();
    for (unsigned long i = 0; i < GetEmbeddedHighlights().size(); ++i) {
        if (AllCharactersDisplayed || TextCharactersShowing > GetEmbeddedHighlights()[i].CharIndex) {
            float y = GFVYToAY(box.TLY);
            float x = GFVXToAX(box.TLX);
            C2DVector screen;
            GFACoordToVCoord(C2DVector(x, y) + GetEmbeddedHighlights()[i].Offset, screen);
            Draw2DText(screen, GetEmbeddedHighlights()[i].Text, *PFont, def.TextBoxTextGameActionColour, true, 2.0f, 1.0f, true);
        }
    }
}