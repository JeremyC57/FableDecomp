// behaviour test for 0x007E7A40 CTCFireheartMinigame::Quit
#include <cstdio>
#include <cstring>
struct Player { int mode; int removed; int GetCurrentMode() { return mode; } void RemoveMode(int m) { if (mode == m) mode = 0; ++removed; } };
struct Gui { bool pending; char text[8]; void SetButton2OverrideText(const char* t) { std::strcpy(text, t); } };
static Gui g_gui;
struct Game {
    bool InSession; Player* player; int handles;
    void Quit()
    {
        InSession = false;
        Player* p = player;
        if (p->GetCurrentMode() == 0x2a) p->RemoveMode(0x2a);
        g_gui.pending = true;
        g_gui.SetButton2OverrideText("");
        handles = 0;
    }
};
int main()
{
    Player p = { 0x2a, 0 };
    g_gui.pending = false; std::strcpy(g_gui.text, "X");
    Game g = { true, &p, 5 };
    g.Quit();
    bool ok = !g.InSession && p.mode == 0 && p.removed == 1 && g_gui.pending && g_gui.text[0] == 0 && g.handles == 0;
    p.mode = 3; g.Quit();
    ok = ok && p.removed == 1 && p.mode == 3;
    if (!ok) { std::printf("007e7a40_TEST FAIL\n"); return 1; }
    std::printf("007e7a40_TEST PASS\n");
    return 0;
}
