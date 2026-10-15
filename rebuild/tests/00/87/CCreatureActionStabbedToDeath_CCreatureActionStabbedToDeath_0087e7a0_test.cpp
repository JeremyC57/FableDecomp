// Behaviour test for 0x0087e7a0 CCreatureAction_StabbedToDeath constructor: base arguments from the owner's animation and sound lookups.
#include <cstdio>
#include <cstring>
struct CCharString {
    char m_text[32];
    CCharString(const char* t, int) { std::strcpy(m_text, t); }
};
struct CAnimationEntry { int id; };
static CAnimationEntry g_anim;
static char g_animName[32];
struct CSoundPair { int FirstSound, LastSound; CSoundPair() : FirstSound(0), LastSound(0) { } };
struct CThingCreatureBase {
    char m_soundName[32];
    virtual ~CThingCreatureBase() { }
    virtual CSoundPair GetSoundPair(const CCharString& name) { std::strcpy(m_soundName, name.m_text); CSoundPair p; p.FirstSound = 11; p.LastSound = 12; return p; }
};
static CAnimationEntry* GetTableAnimation(CThingCreatureBase*, const CCharString& name) { std::strcpy(g_animName, name.m_text); return &g_anim; }
struct CCreatureActionBase {
    CThingCreatureBase* m_owner; float m_a, m_b; CAnimationEntry* m_anim; CSoundPair m_sound; int m_c, m_d;
    CCreatureActionBase(CThingCreatureBase* owner, float a, float b, CAnimationEntry* anim, const CSoundPair& sound, int c, int d)
        : m_owner(owner), m_a(a), m_b(b), m_anim(anim), m_sound(sound), m_c(c), m_d(d) { }
};
static CCharString AnimationName() { return CCharString("STABBED_TO_DEAD", -1); }
static CCharString SoundName() { return CCharString("DEATH", -1); }
struct CAction : CCreatureActionBase {
    bool m_flag;
    CAction(CThingCreatureBase* owner, bool flag)
        : CCreatureActionBase(owner, -1.0f, 1.0f, GetTableAnimation(owner, AnimationName()), owner->GetSoundPair(SoundName()), 100, 250), m_flag(flag) { }
};
int main()
{
    CThingCreatureBase owner;
    CAction a(&owner, false);
    if (a.m_owner != &owner || a.m_a != -1.0f || a.m_b != 1.0f || a.m_anim != &g_anim || a.m_sound.FirstSound != 11 || a.m_sound.LastSound != 12
        || a.m_c != 100 || a.m_d != 250 || std::strcmp(g_animName, "STABBED_TO_DEAD") != 0 || std::strcmp(owner.m_soundName, "DEATH") != 0 || a.m_flag != false)
    { std::printf("0087e7a0_TEST FAIL\n"); return 1; }
    std::printf("0087e7a0_TEST PASS\n");
    return 0;
}