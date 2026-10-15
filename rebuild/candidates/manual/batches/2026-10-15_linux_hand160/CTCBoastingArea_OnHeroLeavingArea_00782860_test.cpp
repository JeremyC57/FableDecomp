// Behaviour test for CTCBoastingArea::OnHeroLeavingArea (0x00782860): boast UI closed and crowd control told, independently.
#include <cstdio>
static int g_close, g_leave;
struct UI { void Deactivate() { ++g_close; } };
struct Crowd { void OnHeroLeavePodiumArea() { ++g_leave; } };
struct Thing {
    unsigned int bits[9]; UI* ui; Crowd* crowd;
    bool Has(int type) const { return (bits[type >> 5] & (1u << (type & 31))) != 0; }
    bool GetInterface(UI** o, int t) { if (Has(t)) { *o = ui; return true; } return false; }
    bool GetInterface(Crowd** o, int t) { if (Has(t)) { *o = crowd; return true; } return false; }
};
static void Leave(Thing& hero)
{
    UI* ui; if (hero.GetInterface(&ui, 0xe8)) ui->Deactivate();
    Crowd* c; if (hero.GetInterface(&c, 0x73)) c->OnHeroLeavePodiumArea();
}
int main()
{
    UI u; Crowd c; Thing h; for (int i = 0; i < 9; ++i) h.bits[i] = 0; h.ui = &u; h.crowd = &c;
    h.bits[3] = 1u << 19; Leave(h);
    if (g_close != 0 || g_leave != 1) { std::printf("00782860_TEST FAIL\n"); return 1; }
    h.bits[7] = 1u << 8; Leave(h);
    if (g_close != 1 || g_leave != 2) { std::printf("00782860_TEST FAIL\n"); return 1; }
    std::printf("00782860_TEST PASS\n");
    return 0;
}
