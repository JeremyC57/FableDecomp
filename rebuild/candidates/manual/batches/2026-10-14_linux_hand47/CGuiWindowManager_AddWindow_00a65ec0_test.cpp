#include <cstdio>
#include <cstdlib>
#include <cstring>
// behaviour test for 0x00A65EC0: exercises the same logic through a copy of the model (compiled separately from the source)
struct CGuiWindow { int id; CGuiWindow* PPrevWindow; CGuiWindow* PNextWindow; };
struct CGuiWindowManager {
    CGuiWindow* PLastWindow; CGuiWindow* PFirstWindow;
    void AddWindow(CGuiWindow* window)
    {
        window->PPrevWindow = PLastWindow;
        if (PLastWindow) PLastWindow->PNextWindow = window;
        window->PNextWindow = 0;
        CGuiWindow* last = PLastWindow;
        PLastWindow = window;
        if (!last) PFirstWindow = window;
    }
    void AddWindowToFront(CGuiWindow* window)
    {
        window->PNextWindow = PFirstWindow;
        if (PFirstWindow) PFirstWindow->PPrevWindow = window;
        window->PPrevWindow = 0;
        CGuiWindow* first = PFirstWindow;
        PFirstWindow = window;
        if (!first) PLastWindow = window;
    }
};
static bool Run()
{
    CGuiWindowManager m; m.PLastWindow = 0; m.PFirstWindow = 0; CGuiWindow a, b;
    m.AddWindow(&a);
    if (m.PFirstWindow != &a || m.PLastWindow != &a || a.PPrevWindow != 0 || a.PNextWindow != 0) return false;
    m.AddWindow(&b);
    return m.PFirstWindow == &a && m.PLastWindow == &b && a.PNextWindow == &b && b.PPrevWindow == &a && b.PNextWindow == 0;
}
int main()
{
    if (!Run()) { std::printf("00a65ec0_TEST FAIL\n"); return 1; }
    std::printf("00a65ec0_TEST PASS\n");
    return 0;
}
