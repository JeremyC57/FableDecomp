#include <cstdio>
#include <cstring>
static int g_dtors = 0;
struct CCharString { const char* text; ~CCharString() { ++g_dtors; } };
static const char* g_table[4] = { "loop1", "loop2", "loop3", "loop4" };
struct CDefStringTable { CCharString GetString(long id) { CCharString s; s.text = g_table[id]; return s; } };
static CDefStringTable g_DefStringTable;
struct CThing { unsigned char Type; };
struct CSpecialAbilitiesPhysicalShieldDef { long LoopingSound[4]; };
static char g_lastSound[16] = "";
static int g_lastPlayer = -1;
struct CTCPhysicalShieldSpell {
    CThing* ParentThing;
    const CSpecialAbilitiesPhysicalShieldDef* PDef;
    long SpellLevel;
    unsigned long LoopingSound;
    bool CutSceneCancelled;
    unsigned long PlaySound(const CCharString& sound, bool player) { std::strcpy(g_lastSound, sound.text); g_lastPlayer = player ? 1 : 0; return 77; }
    void ResumeForCutscene()
    {
        if (CutSceneCancelled) {
            CThing* owner = ParentThing;
            CutSceneCancelled = false;
            CCharString sound = g_DefStringTable.GetString(PDef->LoopingSound[SpellLevel - 1]);
            LoopingSound = PlaySound(sound, owner->Type == 2);
        }
    }
};
int main()
{
    CThing hero; hero.Type = 2;
    CSpecialAbilitiesPhysicalShieldDef d; d.LoopingSound[0] = 0; d.LoopingSound[1] = 1; d.LoopingSound[2] = 2; d.LoopingSound[3] = 3;
    CTCPhysicalShieldSpell s; s.ParentThing = &hero; s.PDef = &d; s.SpellLevel = 3; s.LoopingSound = 0; s.CutSceneCancelled = false;
    s.ResumeForCutscene();
    if (s.LoopingSound != 0 || g_lastPlayer != -1) { std::printf("0079e520_TEST FAIL\n"); return 1; }
    s.CutSceneCancelled = true; s.ResumeForCutscene();
    if (s.LoopingSound != 77 || std::strcmp(g_lastSound, "loop3") != 0 || g_lastPlayer != 1 || s.CutSceneCancelled || g_dtors < 1) { std::printf("0079e520_TEST FAIL\n"); return 1; }
    std::printf("0079e520_TEST PASS\n");
    return 0;
}
