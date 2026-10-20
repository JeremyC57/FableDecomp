// Behaviour test for CV_HeroDollsScript::RemoveHeroDolls (0x00E97EA0): the seven doll objects taken, in order.
#include <cstdio>
#include <cstring>
struct CCharString { const char* Text; CCharString(const char* t) : Text(t) {} };
static const char* g_taken[16]; static int g_n = 0;
struct CGameScriptInterface { void TakeObjectFromHero(const CCharString& n) const { g_taken[g_n++] = n.Text; } };
struct CV_HeroDollsScript {
    CGameScriptInterface* Game;
    void RemoveHeroDolls()
    {
        Game->TakeObjectFromHero("OBJECT_DOLL_BRIAR_ROSE_01");
        Game->TakeObjectFromHero("OBJECT_DOLL_MAZE_01");
        Game->TakeObjectFromHero("OBJECT_DOLL_SCARLET_ROBE_01");
        Game->TakeObjectFromHero("OBJECT_DOLL_THUNDER_01");
        Game->TakeObjectFromHero("OBJECT_DOLL_WHISPER_01");
        Game->TakeObjectFromHero("OBJECT_DOLL_TWINBLADE_01");
        Game->TakeObjectFromHero("OBJECT_DOLL_SCYTHE_01");
    }
};
int main()
{
    const char* want[7] = { "OBJECT_DOLL_BRIAR_ROSE_01", "OBJECT_DOLL_MAZE_01", "OBJECT_DOLL_SCARLET_ROBE_01", "OBJECT_DOLL_THUNDER_01",
        "OBJECT_DOLL_WHISPER_01", "OBJECT_DOLL_TWINBLADE_01", "OBJECT_DOLL_SCYTHE_01" };
    CGameScriptInterface g; CV_HeroDollsScript s; s.Game = &g;
    s.RemoveHeroDolls();
    bool ok = g_n == 7;
    for (int i = 0; ok && i < 7; ++i) ok = std::strcmp(g_taken[i], want[i]) == 0;
    if (!ok) { std::printf("00e97ea0_TEST FAIL\n"); return 1; }
    std::printf("00e97ea0_TEST PASS\n");
    return 0;
}
