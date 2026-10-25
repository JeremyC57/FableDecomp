// Behaviour test for CPlayerGui::Draw2DText (0x00438010): forwards every argument to PDraw2DText->AddText.
#include <stdio.h>
class C2DVector { public: float X, Y; };
class CWideString { public: void* PStringData; };
class CFontBank { public: int id; };
class CRGBColour { public: unsigned char B, G, R, A; };
class CCPPointerInfo;
enum EEngine2DLayer { ENGINE_2D_LAYER_0, ENGINE_2D_LAYER_TEXT = 0x43 };
namespace NPlayerGui {
class CDraw2DText {
public:
    const C2DVector* pos; const CWideString* text; const CFontBank* font; const CRGBColour* colour;
    float scale; EEngine2DLayer layer; bool flag; int calls;
    void AddText(const C2DVector& p, const CWideString& t, const CFontBank& f, const CRGBColour& c, float s, EEngine2DLayer l, bool b)
    { pos = &p; text = &t; font = &f; colour = &c; scale = s; layer = l; flag = b; ++calls; }
};
}
template <class T> class CCountedPointer {
public:
    T* Data; CCPPointerInfo* Info;
    T* operator->() const { return Data; }
};
class CPlayerGui {
public:
    unsigned char _pad_0x000[0x238];
    CCountedPointer<NPlayerGui::CDraw2DText> PDraw2DText;
    void Draw2DText(const C2DVector& pos, const CWideString& text, const CFontBank& font, const CRGBColour& colour,
                    float scale, EEngine2DLayer layer, bool flag)
    { PDraw2DText->AddText(pos, text, font, colour, scale, layer, flag); }
};
int main()
{
    static NPlayerGui::CDraw2DText drawer; drawer.calls = 0;
    static CPlayerGui gui; gui.PDraw2DText.Data = &drawer; gui.PDraw2DText.Info = 0;
    C2DVector p = { 1.0f, 2.0f }; CWideString t = { 0 }; CFontBank f = { 7 }; CRGBColour c = { 1, 2, 3, 4 };
    gui.Draw2DText(p, t, f, c, 0.5f, ENGINE_2D_LAYER_TEXT, true);
    int ok = drawer.calls == 1 && drawer.pos == &p && drawer.text == &t && drawer.font == &f && drawer.colour == &c
        && drawer.scale == 0.5f && drawer.layer == ENGINE_2D_LAYER_TEXT && drawer.flag
        && (unsigned char*)&gui.PDraw2DText - (unsigned char*)&gui == 0x238;
    printf(ok ? "00438010_TEST PASS\n" : "00438010_TEST FAIL\n");
    return ok ? 0 : 1;
}
