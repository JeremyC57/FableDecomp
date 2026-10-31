// Behaviour test for NPlayerGui::CDrawNPCName::Draw (0x006572A0): the name is centred using the font's string width and the
// render-target width; the screenshot line is drawn only while |frame - counter| < 25, falling back to the literal text.
#include <cstdio>
#include <cmath>

class C2DVector { public: float X, Y; C2DVector(float x, float y) : X(x), Y(y) {} };
class C2DExtentsF { public: float Width, Height; C2DExtentsF(float w, float h) : Width(w), Height(h) {} };
class CRGBColour { public: unsigned char B, G, R, A; CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; } };
class CWideString { public:
    const char* Text;
    long GetLength() const { long n = 0; while (Text && Text[n]) ++n; return n; }
};
class CFontBank { public:
    float CharWidth;
    virtual ~CFontBank() {}
    virtual C2DExtentsF GetStringExtents(const char* text, bool) const { long n = 0; while (text[n]) ++n; return C2DExtentsF(n * CharWidth, 10.0f); }
    float GetStringWidth(const CWideString& text) const { return GetStringExtents(text.Text, false).Width; }
};
static int g_draws; static float g_x[2], g_y[2]; static const char* g_text[2];
class CPlayerGui { public:
    void Draw2DText(const C2DVector& pos, const CWideString& text, const CFontBank&, const CRGBColour& c, float scale, int layer, bool)
    {
        if (c.R != 0xff || scale != 0.7f || layer != 0x43) return;
        g_x[g_draws] = pos.X; g_y[g_draws] = pos.Y; g_text[g_draws] = text.Text; ++g_draws;
    }
};
static long g_frame; static long g_renderWidth = 640; static const char* g_bankText;
static float GFAXToVX(float x) { return x * 2.0f; }
static float GFAYToVY(float y) { return y * 3.0f; }

struct CDrawNPCName {
    CPlayerGui* PlayerGui; CFontBank* Font; bool Drawing; CWideString Name; long ScreenShotMessageCounter;
    void Draw()
    {
        if (Name.GetLength() > 0) {
            Drawing = true;
            const CFontBank& font = *Font;
            static CRGBColour colour(0xff, 0xff, 0xff, 0xff);
            float width = font.GetStringWidth(Name);
            float x = g_renderWidth * 0.5f - width * 0.5f;
            PlayerGui->Draw2DText(C2DVector(GFAXToVX(x), GFAYToVY(10.0f)), Name, font, colour, 0.7f, 0x43, true);
        }
        if (ScreenShotMessageCounter > 0 && std::fabs((float)(g_frame - ScreenShotMessageCounter)) < 25.0f) {
            Drawing = true;
            CRGBColour colour(0xff, 0xff, 0xff, 0xff);
            CWideString text; text.Text = g_bankText;
            if (text.GetLength() == 0) text.Text = "Screenshot taken";
            PlayerGui->Draw2DText(C2DVector(GFAXToVX(280.0f), GFAYToVY(35.0f)), text, *Font, colour, 0.7f, 0x43, true);
        }
    }
};

int main()
{
    CPlayerGui gui; CFontBank font; font.CharWidth = 8.0f;
    CDrawNPCName d; d.PlayerGui = &gui; d.Font = &font; d.Drawing = false; d.Name.Text = "Bob"; d.ScreenShotMessageCounter = 0;
    d.Draw();
    // width 24 -> x = 320 - 12 = 308 -> 616
    if (g_draws != 1 || g_x[0] != 616.0f || g_y[0] != 30.0f || !d.Drawing) { std::printf("006572a0_TEST FAIL\n"); return 1; }
    g_draws = 0; d.Name.Text = ""; d.Drawing = false; d.ScreenShotMessageCounter = 100; g_frame = 110; g_bankText = "";
    d.Draw();
    if (g_draws != 1 || g_x[0] != 560.0f || g_y[0] != 105.0f || g_text[0][0] != 'S' || !d.Drawing) { std::printf("006572a0_TEST FAIL\n"); return 1; }
    g_draws = 0; d.Drawing = false; g_frame = 125;
    d.Draw();
    if (g_draws != 0 || d.Drawing) { std::printf("006572a0_TEST FAIL\n"); return 1; }
    g_frame = 90; g_bankText = "Shot";
    d.Draw();
    if (g_draws != 1 || g_text[0][1] != 'h') { std::printf("006572a0_TEST FAIL\n"); return 1; }
    std::printf("006572a0_TEST PASS\n");
    return 0;
}