#include <cstdio>
#include <cstring>
// behaviour test: CTC_BanditHostageKeeper::Main waits for the hero within 15 units, then on a 1-in-5 roll says the parent's current attack line and cycles the line index 0 -> 1 -> 0
static const char* g_line = 0; static int g_lines = 0;
struct CParent { long AttackLineIndex; };
struct CKeeper {
    CParent* ParentClass; float Distance; int Roll; int Frames;
    void NewScriptFrame() { ++Frames; Distance -= 5.0f; }
    void Say(const char* line) { g_line = line; ++g_lines; }
    void Main()
    {
        while (!(Distance < 15.0f)) NewScriptFrame();
        if (Roll % 5 == 0) {
            switch (ParentClass->AttackLineIndex) {
            case 0: Say("TEXT_QST_B11_BANDIT_SECURITY_ATTACK_HERO_10"); break;
            case 1: Say("TEXT_QST_B11_BANDIT_SECURITY_ATTACK_HERO_20"); break;
            default: Say("TEXT_QST_B11_BANDIT_SECURITY_ATTACK_HERO_20"); break;
            }
            if (ParentClass->AttackLineIndex < 1) ++ParentClass->AttackLineIndex;
            else ParentClass->AttackLineIndex = 0;
        }
    }
};
int main()
{
    CParent p; p.AttackLineIndex = 0;
    CKeeper k; k.ParentClass = &p; k.Distance = 30.0f; k.Roll = 10; k.Frames = 0;
    k.Main();
    bool ok = k.Frames == 4 && g_lines == 1 && std::strcmp(g_line, "TEXT_QST_B11_BANDIT_SECURITY_ATTACK_HERO_10") == 0 && p.AttackLineIndex == 1;
    k.Main();
    ok = ok && g_lines == 2 && std::strcmp(g_line, "TEXT_QST_B11_BANDIT_SECURITY_ATTACK_HERO_20") == 0 && p.AttackLineIndex == 0;
    k.Roll = 3; k.Main();
    ok = ok && g_lines == 2 && p.AttackLineIndex == 0;
    if (!ok) { std::printf("00dfba60_TEST FAIL\n"); return 1; }
    std::printf("00dfba60_TEST PASS\n");
    return 0;
}