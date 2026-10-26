#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif
// Behaviour test: NPlayerGui::CStatBar::DrawStatBoxWithText -- DrawStatBox (withText = rightAlign, then false, true), then
// the text at def money-text offset + pos converted to virtual coordinates; right-aligned as
// VX(screen width) - x - string width; preloaded, drawn white at scale 1 on the text layer.
class C2DVector { public: float X, Y; C2DVector() {} C2DVector(float x, float y) : X(x), Y(y) {} };
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
class CWideString { public: int Len; };
class CEnginePrimitivePrepareInfo { public: int Id; };
class CEnginePrimitiveHandle { public: unsigned long m_handle; };
struct CHandleVector { CEnginePrimitiveHandle* _M_start; CEnginePrimitiveHandle* _M_finish; CEnginePrimitiveHandle* _M_end_of_storage; };
class C2DExtentsI { public: long Width, Height; };
class CDisplayManager { public: C2DExtentsI GetRenderTargetDimensions() { C2DExtentsI e; e.Width = 640; e.Height = 480; return e; } };
static CDisplayManager g_display;
static CDisplayManager* g_pDisplayManager = &g_display;
void __fastcall GFACoordToVCoord(const C2DVector& in, C2DVector& out) { out.X = in.X * 2.0f; out.Y = in.Y * 2.0f; }
float __fastcall GFAXToVX(float x) { return x * 2.0f; }
class CFontBank {
public:
    int Preloads;
    bool Preload(const CWideString& text, bool wait) const { ++const_cast<CFontBank*>(this)->Preloads; return !wait && text.Len > 0; }
    float GetVStringWidth(const CWideString& text) const { return (float)(text.Len * 10); }
};
enum EEngine2DLayer { ENGINE_2D_LAYER_TEXT = 0x43 };
struct CGameUI {
    CFontBank* Font;
    int Draws; C2DVector LastPos; const CFontBank* LastFont; unsigned char R, G, B, A; float Scale; EEngine2DLayer Layer; bool Flag;
    void Draw2DText(const C2DVector& pos, const CWideString& text, CFontBank* font, const CRGBColour& colour,
                    float scale, EEngine2DLayer layer, bool flag)
    {
        ++Draws; LastPos = pos; LastFont = font; R = colour.R; G = colour.G; B = colour.B; A = colour.A;
        Scale = scale; Layer = layer; Flag = flag;
    }
};
static CGameUI g_ui;
static CGameUI* g_pGameUI = &g_ui;
class CPlayerGuiDef { public: C2DVector MoneyTextOffset; };
static CPlayerGuiDef g_def;
const CPlayerGuiDef* __fastcall PeekGuiDef() { return &g_def; }
namespace NPlayerGui {
static int g_boxes = 0; static bool g_withText, g_flag, g_extra; static long g_value; static unsigned char g_alpha;
class CStatBar {
public:
    static void __fastcall DrawStatBox(const CEnginePrimitivePrepareInfo& info, CHandleVector& handles,
        const C2DVector& pos, long graphic, const CWideString& text, long value, unsigned char alpha, bool withText, bool flag,
        bool extra)
    { ++g_boxes; g_withText = withText; g_flag = flag; g_extra = extra; g_value = value; g_alpha = alpha; }
    static void __fastcall DrawStatBoxWithText(const CEnginePrimitivePrepareInfo& info, CHandleVector& handles,
        const C2DVector& pos, long graphic, const CWideString& text, long value, unsigned char alpha, bool rightAlign)
    {
        DrawStatBox(info, handles, pos, graphic, text, value, alpha, rightAlign, false, true);
        CFontBank* font = g_pGameUI->Font;
        const CPlayerGuiDef* def = PeekGuiDef();
        C2DVector screenPos(def->MoneyTextOffset.X + pos.X, def->MoneyTextOffset.Y + pos.Y);
        C2DVector textPos;
        GFACoordToVCoord(screenPos, textPos);
        if (rightAlign)
            textPos.X = GFAXToVX((float)g_pDisplayManager->GetRenderTargetDimensions().Width) - textPos.X
                      - font->GetVStringWidth(text);
        font->Preload(text, false);
        g_pGameUI->Draw2DText(textPos, text, font, CRGBColour(255, 255, 255, 255), 1.0f, ENGINE_2D_LAYER_TEXT, false);
    }
};
}
int main()
{
    CFontBank font; font.Preloads = 0;
    g_ui.Font = &font; g_ui.Draws = 0;
    g_def.MoneyTextOffset.X = 10.0f; g_def.MoneyTextOffset.Y = 20.0f;
    CEnginePrimitivePrepareInfo info = { 1 };
    CHandleVector handles = { 0, 0, 0 };
    C2DVector pos(100.0f, 50.0f);
    CWideString text; text.Len = 3;
    NPlayerGui::CStatBar::DrawStatBoxWithText(info, handles, pos, 7, text, 99, 200, false);
    if (NPlayerGui::g_boxes != 1 || NPlayerGui::g_withText || NPlayerGui::g_flag || !NPlayerGui::g_extra) { std::printf("00646a46_TEST FAIL\n"); return 1; }
    if (NPlayerGui::g_value != 99 || NPlayerGui::g_alpha != 200) { std::printf("00646a46_TEST FAIL\n"); return 1; }
    if (g_ui.Draws != 1 || g_ui.LastPos.X != 220.0f || g_ui.LastPos.Y != 140.0f || font.Preloads != 1) { std::printf("00646a46_TEST FAIL\n"); return 1; }
    if (g_ui.R != 255 || g_ui.G != 255 || g_ui.B != 255 || g_ui.A != 255 || g_ui.Scale != 1.0f) { std::printf("00646a46_TEST FAIL\n"); return 1; }
    if (g_ui.Layer != ENGINE_2D_LAYER_TEXT || g_ui.Flag || g_ui.LastFont != &font) { std::printf("00646a46_TEST FAIL\n"); return 1; }
    NPlayerGui::CStatBar::DrawStatBoxWithText(info, handles, pos, 7, text, 99, 200, true);
    if (!NPlayerGui::g_withText || g_ui.Draws != 2) { std::printf("00646a46_TEST FAIL\n"); return 1; }
    if (g_ui.LastPos.X != 1280.0f - 220.0f - 30.0f || g_ui.LastPos.Y != 140.0f) { std::printf("00646a46_TEST FAIL\n"); return 1; }
    std::printf("00646a46_TEST PASS\n");
    return 0;
}