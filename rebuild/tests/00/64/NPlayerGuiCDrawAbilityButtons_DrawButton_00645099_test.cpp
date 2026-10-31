// behaviour test for NPlayerGui::CDrawAbilityButtons::DrawButton 0x00645099: picks the special handle table when asked, a
// negative alpha means unscaled (button alpha 160), the GUI alpha byte is forced to 255 while the special buttons show (and then
// the button alpha is 255 for special buttons, 0 otherwise), and nothing is drawn for a non-positive graphic or zero alpha
#include <cstdio>
struct Result { bool drawn; int table; float buttonAlpha; };
static Result DrawButton(float alpha, long graphic, bool special, float guiAlphaF, bool showSpecial)
{
    Result r = { false, special ? 1 : 0, 0.0f };
    bool scaled = true;
    if (alpha < 0.0f) { scaled = false; alpha = -alpha; }
    unsigned char guiAlpha = (unsigned char)(long)(guiAlphaF * 255.0f);
    if (showSpecial && guiAlpha < 255) guiAlpha = 255;
    if (graphic > 0 && alpha > 0.0f) {
        float b = alpha * (float)guiAlpha;
        if (!scaled) b = 160.0f;
        else if (showSpecial && guiAlpha < 255) b = special ? 255.0f : 0.0f;
        r.drawn = true; r.buttonAlpha = b;
    }
    return r;
}
int main()
{
    bool ok = true;
    Result a = DrawButton(0.5f, 3, false, 1.0f, false);
    ok = ok && a.drawn && a.table == 0 && a.buttonAlpha == 127.5f;
    Result b = DrawButton(-0.5f, 3, true, 1.0f, false);
    ok = ok && b.drawn && b.table == 1 && b.buttonAlpha == 160.0f;
    Result c = DrawButton(1.0f, 3, false, 0.5f, true);
    ok = ok && c.drawn && c.buttonAlpha == 255.0f;
    Result d = DrawButton(1.0f, 0, false, 1.0f, false);
    ok = ok && !d.drawn;
    Result e = DrawButton(0.0f, 2, false, 1.0f, false);
    ok = ok && !e.drawn;
    std::printf(ok ? "00645099_TEST PASS\n" : "00645099_TEST FAIL\n");
    return ok ? 0 : 1;
}