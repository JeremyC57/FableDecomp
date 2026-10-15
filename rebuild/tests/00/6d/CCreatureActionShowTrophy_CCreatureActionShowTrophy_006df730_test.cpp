// Behaviour test for 0x006DF730 CCreatureAction_ShowTrophy(owner, trophy, animation, last): base arguments and member initialisation.
#include <cstdio>
struct CThing { int id; };
struct CAnimationEntry { int id; };
static CAnimationEntry g_anim;
static const char* g_lastName = 0;
static CAnimationEntry* GetTableAnimation(CThing*, const char* name) { g_lastName = name; return &g_anim; }
struct CSoundPair { int FirstSound, LastSound; CSoundPair() : FirstSound(0), LastSound(0) { } };
struct CBaseIntelligentPointer { CThing* m_pItem; CBaseIntelligentPointer() : m_pItem(0) { } void SetPItem(CThing* p) { m_pItem = p; } };
struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer() { }
    CIntelligentPointer(CThing* item) { SetPItem(item); }
};
struct CCreatureActionBase {
    CThing* m_owner; float m_a, m_b; CAnimationEntry* m_anim; CSoundPair m_sound; int m_c, m_d;
    CCreatureActionBase(CThing* owner, float a, float b, CAnimationEntry* anim, const CSoundPair& sound, int c, int d)
        : m_owner(owner), m_a(a), m_b(b), m_anim(anim), m_sound(sound), m_c(c), m_d(d) { }
};
struct CCreatureAction_ShowTrophy : CCreatureActionBase {
    CIntelligentPointer PTrophy; int TrophyState; unsigned char Shown; CIntelligentPointer PTrophyObject;
    CCreatureAction_ShowTrophy(CThing* owner, CThing* trophy, const char* animation, int last)
        : CCreatureActionBase(owner, -1.0f, 0.95f, GetTableAnimation(owner, animation), CSoundPair(), 100, last),
          PTrophy(trophy), TrophyState(0), Shown(0), PTrophyObject() { }
};
int main()
{
    CThing owner, trophy;
    CCreatureAction_ShowTrophy a(&owner, &trophy, "SHOW_TROPHY", 7);
    if (a.m_owner != &owner || a.m_a != -1.0f || a.m_b != 0.95f || a.m_anim != &g_anim || a.m_sound.FirstSound != 0 || a.m_sound.LastSound != 0
        || a.m_c != 100 || a.m_d != 7 || a.PTrophy.m_pItem != &trophy || a.TrophyState != 0 || a.Shown != 0 || a.PTrophyObject.m_pItem != 0
        || g_lastName == 0 || g_lastName[0] != 'S') { std::printf("006df730_TEST FAIL\n"); return 1; }
    std::printf("006df730_TEST PASS\n");
    return 0;
}