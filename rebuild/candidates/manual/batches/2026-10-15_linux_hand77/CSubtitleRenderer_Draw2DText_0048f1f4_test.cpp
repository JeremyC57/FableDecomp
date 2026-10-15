#include <cstdio>
// Behaviour test for 0048f1f4: CSubtitleRenderer members used by the body (retail layout names from the PDB), with
// recording stand-ins for the out-of-line callees.
struct C2DVector { float X, Y; };
struct CRGBColour { unsigned char B, G, R, A; };
class CFontBank { public: int m_id; };
struct CFadeInFadeOutBase {
    bool  Active;
    float FadeInTime;
    float FadeOutTime;
    bool  Closing;
    bool  Opening;
    float OpenTimer;
    float CloseTimer;
};
class CWideString { public: int m_id; };
enum EEngine2DLayer { ENGINE_2D_LAYER_SUBTITLE = 0x43 };
struct SDraw { const C2DVector* pos; const CWideString* text; CFontBank* font; const CRGBColour* colour; float scale; int layer; bool flag; };
static SDraw g_draw;
struct CGameUI {
    void Draw2DText(const C2DVector& pos, const CWideString& text, CFontBank* font, const CRGBColour& colour,
                    float scale, EEngine2DLayer layer, bool flag)
    { g_draw.pos = &pos; g_draw.text = &text; g_draw.font = font; g_draw.colour = &colour; g_draw.scale = scale; g_draw.layer = layer; g_draw.flag = flag; }
};
static CGameUI g_ui;
static CGameUI* g_pGameUI = &g_ui;
class CSubtitleRenderer {
public:
    unsigned long      DisplayFlags;
    bool               InCutscene;
    bool               CutsceneSkipped;
    bool               CutsceneSkippable;
    bool               CutsceneActionMode;
    bool               DisplayingText;
    CFadeInFadeOutBase SkipCutsceneTimer;
    void Draw2DText(const C2DVector& pos, const CWideString& text, CFontBank* font, const CRGBColour& colour,
                    bool flag, float unused, float scale, bool raiseLayer);
};
void CSubtitleRenderer::Draw2DText(const C2DVector& pos, const CWideString& text, CFontBank* font, const CRGBColour& colour,
                                   bool flag, float unused, float scale, bool raiseLayer)
{
    long layer = 0x43;
    if (DisplayFlags & 0x800)
        layer = 0x40;
    if (raiseLayer)
        --layer;
    g_pGameUI->Draw2DText(pos, text, font, colour, scale, (EEngine2DLayer)layer, flag);
}
#define FAIL do { std::printf("0048f1f4_TEST FAIL\n"); return 1; } while (0)
int main()
{
    CSubtitleRenderer r; r.DisplayFlags = 0;
    C2DVector pos = { 1.0f, 2.0f }; CWideString text; text.m_id = 1; CFontBank font; CRGBColour colour = { 1, 2, 3, 4 };
    r.Draw2DText(pos, text, &font, colour, true, 9.0f, 1.5f, false);
    if (g_draw.pos != &pos || g_draw.text != &text || g_draw.font != &font || g_draw.colour != &colour || g_draw.scale != 1.5f
        || g_draw.layer != 0x43 || !g_draw.flag) FAIL;
    r.Draw2DText(pos, text, &font, colour, false, 9.0f, 2.0f, true);
    if (g_draw.layer != 0x42 || g_draw.flag || g_draw.scale != 2.0f) FAIL;
    r.DisplayFlags = 0x800;
    r.Draw2DText(pos, text, &font, colour, false, 9.0f, 2.0f, false);
    if (g_draw.layer != 0x40) FAIL;
    r.Draw2DText(pos, text, &font, colour, false, 9.0f, 2.0f, true);
    if (g_draw.layer != 0x3F) FAIL;
    std::printf("0048f1f4_TEST PASS\n");
    return 0;
}
