// Behaviour test for CTCBoastingArea::OnHeroEnteringArea (0x007827D0): only outside mini games, the boast UI opens and then the
// crowd control (when present) is told.
#include <cstdio>
static int g_open, g_enter; static long g_minigame;
struct UI { void RequestActivate() { ++g_open; } };
struct Crowd { void OnHeroEnterPodiumArea() { ++g_enter; } };
struct Thing {
    unsigned int bits[9]; UI* ui; Crowd* crowd;
    bool Has(int type) const { return (bits[type >> 5] & (1u << (type & 31))) != 0; }
    bool GetInterface(UI** o, int t) { if (Has(t)) { *o = ui; return true; } return false; }
    bool GetInterface(Crowd** o, int t) { if (Has(t)) { *o = crowd; return true; } return false; }
};
static void Enter(Thing& hero)
{
    if (g_minigame == 0) {
        UI* ui;
        if (hero.GetInterface(&ui, 0xe8)) {
            ui->RequestActivate();
            Crowd* c;
            if (hero.GetInterface(&c, 0x73)) c->OnHeroEnterPodiumArea();
        }
    }
}
int main()
{
    UI u; Crowd c; Thing h; for (int i = 0; i < 9; ++i) h.bits[i] = 0; h.ui = &u; h.crowd = &c;
    h.bits[7] = 1u << 8; h.bits[3] = 1u << 19;
    Enter(h);
    if (g_open != 1 || g_enter != 1) { std::printf("007827d0_TEST FAIL\n"); return 1; }
    g_minigame = 2; Enter(h);
    g_minigame = 0; h.bits[7] = 0; Enter(h);
    if (g_open != 1 || g_enter != 1) { std::printf("007827d0_TEST FAIL\n"); return 1; }
    std::printf("007827d0_TEST PASS\n");
    return 0;
}
