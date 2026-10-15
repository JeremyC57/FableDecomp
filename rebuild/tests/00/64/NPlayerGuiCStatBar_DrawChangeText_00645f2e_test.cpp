#include <cstdio>
// Behaviour test for 00645f2e: the change text is drawn in the bar's font with the colour's alpha scaled by the UI's
// global alpha (truncated), scale 1.0, layer 0x43 and the flag set; R, G and B are kept.
struct C2DVector { float X, Y; };
class CWideString { public: int m_id; };
class CFontBank { public: int m_id; };
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
enum EEngine2DLayer { ENGINE_2D_LAYER_TEXT = 0x43 };
struct SDrawCall { const C2DVector* pos; const CWideString* text; CFontBank* font; unsigned char r, g, b, a; float scale; int layer; bool flag; int count; };
static SDrawCall g_call;
struct CGameUI {
    float m_globalAlpha;
    void Draw2DText(const C2DVector& pos, const CWideString& text, CFontBank* font, const CRGBColour& colour,
                    float scale, EEngine2DLayer layer, bool flag)
    {
        g_call.pos = &pos; g_call.text = &text; g_call.font = font; g_call.r = colour.R; g_call.g = colour.G; g_call.b = colour.B;
        g_call.a = colour.A; g_call.scale = scale; g_call.layer = layer; g_call.flag = flag; ++g_call.count;
    }
};
static CGameUI g_ui;
static CGameUI* g_pGameUI = &g_ui;
struct CStatBar {
    CFontBank* m_pFont;
    void DrawChangeText(const C2DVector& pos, const CWideString& text, const CRGBColour& colour);
};
void CStatBar::DrawChangeText(const C2DVector& pos, const CWideString& text, const CRGBColour& colour)
{
    CRGBColour faded(colour.R, colour.G, colour.B, (unsigned char)(colour.A * g_pGameUI->m_globalAlpha));
    g_pGameUI->Draw2DText(pos, text, m_pFont, faded, 1.0f, ENGINE_2D_LAYER_TEXT, true);
}
int main()
{
    CFontBank font; CStatBar bar; bar.m_pFont = &font;
    C2DVector pos = { 1.0f, 2.0f }; CWideString text; text.m_id = 3;
    CRGBColour colour(10, 20, 30, 200);
    g_ui.m_globalAlpha = 0.5f; g_call.count = 0;
    bar.DrawChangeText(pos, text, colour);
    if (g_call.count != 1 || g_call.pos != &pos || g_call.text != &text || g_call.font != &font || g_call.r != 10 || g_call.g != 20
        || g_call.b != 30 || g_call.a != 100 || g_call.scale != 1.0f || g_call.layer != 0x43 || !g_call.flag) {
        std::printf("00645f2e_TEST FAIL\n"); return 1;
    }
    g_ui.m_globalAlpha = 0.25f; colour.A = 255;
    bar.DrawChangeText(pos, text, colour);
    if (g_call.a != 63 || colour.A != 255) { std::printf("00645f2e_TEST FAIL\n"); return 1; }
    std::printf("00645f2e_TEST PASS\n");
    return 0;
}