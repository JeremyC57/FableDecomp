#include <cstdio>
// Behaviour test for 006442ae.

// NPlayerGui::CDrawCrime::Draw: marks the drawer as drawing, sets the stat bar's graphics from the GUI def and draws it with the GUI
// alpha scaled to 0..255 and the def's position/offset/colour.
struct C2DVector { float X, Y; };
struct CStatBar {
    long GraphicIndex, BorderGraphicIndex, EffectIndex;
    int draws; const C2DVector* pos; const C2DVector* offset; unsigned char alpha; bool force; void* info; void* bank;
    CStatBar() : GraphicIndex(0), BorderGraphicIndex(0), EffectIndex(0), draws(0) { }
    virtual void Draw(void* i, void* b, const C2DVector& p, const C2DVector& o, const unsigned int& colour, unsigned char a, bool f)
    { (void)colour; ++draws; info = i; bank = b; pos = &p; offset = &o; alpha = a; force = f; }
};
struct CPlayerGui { float Alpha; CStatBar* bar; };
struct CPlayerGuiDef { long g1, g2; long graphics[6]; C2DVector Pos, Offset; unsigned int Colour; };
static CPlayerGuiDef g_def;
static CPlayerGuiDef* PeekGuiDef() { return &g_def; }
struct CDraw {
    CPlayerGui* PlayerGui; void* Bank; bool Drawing;
    void Draw(void* info)
    {
        Drawing = true;
        CPlayerGuiDef* def = PeekGuiDef();
        CStatBar* bar = PlayerGui->bar;
        bar->GraphicIndex = def->g1;
        bar->BorderGraphicIndex = def->g2;
        bar->Draw(info, Bank, def->Pos, def->Offset, def->Colour, (unsigned char)(PlayerGui->Alpha * 255.0f), false);
    }
};
int main()
{
    g_def.g1 = 11; g_def.g2 = 12; g_def.graphics[4] = 44;
    CStatBar bar; CPlayerGui gui; gui.Alpha = 0.5f; gui.bar = &bar;
    CDraw d; d.PlayerGui = &gui; d.Bank = &gui; d.Drawing = false;
    int info = 0;
    d.Draw(&info);
    int ok = d.Drawing && bar.draws == 1 && bar.info == &info && bar.bank == &gui && bar.pos == &g_def.Pos && bar.offset == &g_def.Offset
          && bar.alpha == 127 && bar.GraphicIndex == 11 && bar.BorderGraphicIndex == 12 && !bar.force;
    if (!ok) { std::printf("006442ae_TEST FAIL\n"); return 1; }
    std::printf("006442ae_TEST PASS\n");
    return 0;
}