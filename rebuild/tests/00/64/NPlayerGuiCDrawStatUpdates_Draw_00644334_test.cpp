// Behaviour test for 0x00644334 NPlayerGui::CDrawStatUpdates::Draw: clear the bar graphic, draw at the def position with alpha*255, not forced open (standalone copy of the logic)
#include <cstdio>
struct Vec { float X, Y; };
struct Colour { unsigned char B, G, R, A; };
struct Bar { long GraphicIndex; const Vec* pos; const Vec* off; const Colour* col; unsigned char alpha; bool force; int calls;
    void DrawStatBarOnLeft(const Vec& p, const Vec& o, const Colour& c, unsigned char a, bool f) { pos = &p; off = &o; col = &c; alpha = a; force = f; ++calls; } };
struct Def { Vec Pos; Vec TextOffset; Colour TextColour; };
struct Gui { float Alpha; Bar* PBar; };
struct Drawer { Gui* PlayerGui; bool Drawing;
    void Draw(const Def& def) { Drawing = true; Bar* bar = PlayerGui->PBar; bar->GraphicIndex = 0;
        bar->DrawStatBarOnLeft(def.Pos, def.TextOffset, def.TextColour, (unsigned char)(PlayerGui->Alpha * 255.0f), false); } };

int main()
{
    Bar b; b.GraphicIndex = 7; b.calls = 0; Gui g; g.Alpha = 0.5f; g.PBar = &b; Def d; Drawer dr; dr.PlayerGui = &g; dr.Drawing = false;
    dr.Draw(d);
    if (!dr.Drawing || b.GraphicIndex != 0 || b.calls != 1 || b.alpha != 127 || b.force) { std::printf("00644334_TEST FAIL\n"); return 1; }
    if (b.pos != &d.Pos || b.off != &d.TextOffset || b.col != &d.TextColour) { std::printf("00644334_TEST FAIL\n"); return 1; }
    std::printf("00644334_TEST PASS\n");
    return 0;
}