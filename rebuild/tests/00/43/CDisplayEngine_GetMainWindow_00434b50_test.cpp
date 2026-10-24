// Behaviour test for CDisplayEngine::GetMainWindow (0x00434B50): the main window box is run through the virtual-to-actual
// conversion and the converted box comes back by value; the engine's own box is untouched.
#include <cstdio>
class C2DBoxF {
public:
    C2DBoxF() {}
    C2DBoxF(const C2DBoxF& o) : TLX(o.TLX), TLY(o.TLY), BRX(o.BRX), BRY(o.BRY) {}
    float TLX, TLY, BRX, BRY;
};
static void GFVBoxToABox(const C2DBoxF* vbox, C2DBoxF* abox)   // virtual 0..1 -> a 640x480 screen
{
    abox->TLX = vbox->TLX * 640.0f; abox->TLY = vbox->TLY * 480.0f;
    abox->BRX = vbox->BRX * 640.0f; abox->BRY = vbox->BRY * 480.0f;
}
class CDisplayEngine {
public:
    C2DBoxF MainWindow;
    C2DBoxF GetMainWindow() const { C2DBoxF box; GFVBoxToABox(&MainWindow, &box); return box; }
};
int main()
{
    CDisplayEngine e;
    e.MainWindow.TLX = 0.0f; e.MainWindow.TLY = 0.25f; e.MainWindow.BRX = 0.5f; e.MainWindow.BRY = 1.0f;
    C2DBoxF b = e.GetMainWindow();
    int bad = 0;
    if (b.TLX != 0.0f || b.TLY != 120.0f || b.BRX != 320.0f || b.BRY != 480.0f) bad = 1;
    if (e.MainWindow.TLY != 0.25f || e.MainWindow.BRX != 0.5f) bad = 1;
    if (bad) { std::printf("00434b50_TEST FAIL\n"); return 1; }
    std::printf("00434b50_TEST PASS\n");
    return 0;
}