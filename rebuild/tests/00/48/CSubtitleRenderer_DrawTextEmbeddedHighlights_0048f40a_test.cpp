// Behaviour test for CSubtitleRenderer::DrawTextEmbeddedHighlights (0x0048F40A): only highlights whose character index has
// been revealed (or every one once all characters are shown) are drawn, at GFACoordToVCoord(box corner converted to
// actual coordinates + highlight offset), with the GUI def's game-action colour, scale (2, 1) and the raise flag.
#include <cstdio>

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
class CWideString { public: int Id; };
class CFontBank { public: int Id; };

class CPlayerGuiDef { public: CRGBColour TextBoxTextGameActionColour; };
static CPlayerGuiDef g_def;
static int g_peeks = 0;
const CPlayerGuiDef& PeekGuiDef() { ++g_peeks; return g_def; }
float GFVYToAY(float y) { return y * 2.0f; }
float GFVXToAX(float x) { return x * 4.0f; }
void GFACoordToVCoord(const C2DVector& in, C2DVector& out) { out.X = in.X / 4.0f; out.Y = in.Y / 2.0f; }

struct SDraw { C2DVector pos; int text; int font; unsigned char r; bool unused; float sx, sy; bool raise; };
static SDraw g_draws[8];
static int g_drawCount = 0;

class CSubtitleRenderer {
public:
    class CEmbeddedHighlight {
    public:
        CWideString Text;
        long        CharIndex;
        C2DVector   Offset;
        bool        Split;
    };
    CEmbeddedHighlight* first;
    CEmbeddedHighlight* last;
    long TextCharactersShowing;
    bool AllCharactersDisplayed;
    CFontBank* PFont;
    void Draw2DText(const C2DVector& pos, const CWideString& text, const CFontBank& font, const CRGBColour& colour,
                    bool unused, float scaleX, float scaleY, bool raise)
    {
        SDraw& d = g_draws[g_drawCount++];
        d.pos = pos; d.text = text.Id; d.font = font.Id; d.r = colour.R; d.unused = unused; d.sx = scaleX; d.sy = scaleY; d.raise = raise;
    }
    void DrawTextEmbeddedHighlights(const C2DBoxF& box)
    {
        if (first == last)
            return;
        const CPlayerGuiDef& def = PeekGuiDef();
        for (unsigned long i = 0; i < (unsigned long)(last - first); ++i) {
            if (AllCharactersDisplayed || TextCharactersShowing > first[i].CharIndex) {
                float y = GFVYToAY(box.TLY);
                float x = GFVXToAX(box.TLX);
                C2DVector screen;
                GFACoordToVCoord(C2DVector(x, y) + first[i].Offset, screen);
                Draw2DText(screen, first[i].Text, *PFont, def.TextBoxTextGameActionColour, true, 2.0f, 1.0f, true);
            }
        }
    }
};

int main()
{
    CFontBank font = { 7 };
    g_def.TextBoxTextGameActionColour.R = 200;
    CSubtitleRenderer::CEmbeddedHighlight h[3];
    h[0].Text.Id = 1; h[0].CharIndex = 2; h[0].Offset = C2DVector(8.0f, 4.0f);
    h[1].Text.Id = 2; h[1].CharIndex = 5; h[1].Offset = C2DVector(0.0f, 0.0f);
    h[2].Text.Id = 3; h[2].CharIndex = 1; h[2].Offset = C2DVector(-4.0f, 2.0f);
    CSubtitleRenderer r;
    r.first = h; r.last = h + 3; r.TextCharactersShowing = 3; r.AllCharactersDisplayed = false; r.PFont = &font;
    C2DBoxF box = { 10.0f, 20.0f, 0.0f, 0.0f };
    r.DrawTextEmbeddedHighlights(box);
    // h[0] (2 < 3) and h[2] (1 < 3) drawn, h[1] (5) skipped: (10*4 + 8) / 4 = 12, (20*2 + 4) / 2 = 22; (40 - 4) / 4 = 9, (40 + 2) / 2 = 21
    if (g_drawCount != 2 || g_peeks != 1) { std::printf("0048f40a_TEST FAIL\n"); return 1; }
    if (g_draws[0].text != 1 || g_draws[0].pos.X != 12.0f || g_draws[0].pos.Y != 22.0f) { std::printf("0048f40a_TEST FAIL\n"); return 1; }
    if (g_draws[1].text != 3 || g_draws[1].pos.X != 9.0f || g_draws[1].pos.Y != 21.0f) { std::printf("0048f40a_TEST FAIL\n"); return 1; }
    if (g_draws[0].font != 7 || g_draws[0].r != 200 || !g_draws[0].unused || g_draws[0].sx != 2.0f || g_draws[0].sy != 1.0f || !g_draws[0].raise) {
        std::printf("0048f40a_TEST FAIL\n"); return 1;
    }
    r.AllCharactersDisplayed = true;
    g_drawCount = 0;
    r.DrawTextEmbeddedHighlights(box);
    if (g_drawCount != 3) { std::printf("0048f40a_TEST FAIL\n"); return 1; }
    r.last = r.first;
    g_drawCount = 0; g_peeks = 0;
    r.DrawTextEmbeddedHighlights(box);
    if (g_drawCount != 0 || g_peeks != 0) { std::printf("0048f40a_TEST FAIL\n"); return 1; }
    std::printf("0048f40a_TEST PASS\n");
    return 0;
}