#include <cstdio>
// Behaviour test for CTCInventoryBase::DrawInventoryText (0x005BCE49): same logic over a copy of the model.
struct C2DVector { float X; float Y; };
struct CWideString { float width; };
struct CRGBColour { unsigned long rgba; };
struct CFontBank { float GetVStringWidth(const CWideString& text) { return text.width; } };
static void GFACoordToVCoord(const C2DVector* in, C2DVector* out) { out->X = in->X * 2.0f; out->Y = in->Y * 2.0f; }
static C2DVector g_drawn; static float g_scale = 0.0f; static bool g_shadow = false; static int g_draws = 0;
static void Draw2DText(C2DVector* position, CWideString*, CFontBank*, CRGBColour*, float scale, bool shadow) { g_drawn = *position; g_scale = scale; g_shadow = shadow; ++g_draws; }
enum ETextLayout { TEXT_LAYOUT_LEFT = 0, TEXT_LAYOUT_CENTRE = 1, TEXT_LAYOUT_RIGHT = 2 };
struct CTCInventoryBase_Retail {
    void DrawInventoryText(C2DVector position, CFontBank* font, CWideString* text, ETextLayout layout, CRGBColour* colour, bool shadow)
    {
        GFACoordToVCoord(&position, &position);
        if (layout == TEXT_LAYOUT_CENTRE)
            position.X -= font->GetVStringWidth(*text) * 0.5f;
        else if (layout == TEXT_LAYOUT_RIGHT)
            position.X -= font->GetVStringWidth(*text);
        Draw2DText(&position, text, font, colour, 1.0f, shadow);
    }
};
int main()
{
    CTCInventoryBase_Retail inv; CFontBank font; CWideString text = { 40.0f }; CRGBColour c = { 0xffffffff };
    C2DVector p = { 100.0f, 50.0f };
    inv.DrawInventoryText(p, &font, &text, TEXT_LAYOUT_LEFT, &c, true);
    if (g_drawn.X != 200.0f || g_drawn.Y != 100.0f || g_scale != 1.0f || !g_shadow) { std::printf("005bce49_TEST FAIL\n"); return 1; }
    inv.DrawInventoryText(p, &font, &text, TEXT_LAYOUT_CENTRE, &c, false);
    if (g_drawn.X != 180.0f || g_shadow) { std::printf("005bce49_TEST FAIL\n"); return 1; }
    inv.DrawInventoryText(p, &font, &text, TEXT_LAYOUT_RIGHT, &c, false);
    if (g_drawn.X != 160.0f || g_draws != 3) { std::printf("005bce49_TEST FAIL\n"); return 1; }
    std::printf("005bce49_TEST PASS\n");
    return 0;
}
