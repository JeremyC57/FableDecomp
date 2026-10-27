// Behaviour test for 0x007C90A0 CTCFireballSpell::FinalCleanup: mock interfaces log every call; the body is the source's, verbatim.
#include <cstdio>
#include <cstring>

static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static int Check(const char* expect) { int ok = std::strcmp(g_log, expect) == 0; if (!ok) std::printf("got  %s\nwant %s\n", g_log, expect); g_log[0] = 0; return ok; }

class CTCSpecialAbilities { public: bool AbilityMutexLock; int CurrentAbility; };
class CPlayer { public: void EndSpecialMoveMode() { Log("EndSpecialMove"); } };
class CTCCreatureModeManager { public: void RemoveMode(int m) { char b[32]; std::sprintf(b, "RemoveMode(%x)", m); Log(b); } };
class CThing {
public:
    bool Dead; int State; void* Iface[0x120]; unsigned long TCMask; CPlayer Player;
    CThing() : Dead(false), State(0), TCMask(0) { for (int i = 0; i < 0x120; ++i) Iface[i] = 0; }
    bool IsDead() const { return Dead; }
    template <class T> bool GetInterface(T** out, long type) const { *out = (T*)Iface[type]; return Iface[type] != 0; }
    unsigned long HasTC(long type) const { return type == 0x109 ? TCMask : 0; }
    void Kill(bool b) { Log(b ? "Kill(1)" : "Kill(0)"); }
    CPlayer& GetPlayer() { return Player; }
};

class CThing;
template <class T> class CIntelligentPointer { public: T* Item; CIntelligentPointer() : Item(0) { } T* GetPItem() const { return Item; } };

inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p) { return p.GetPItem() && !p.GetPItem()->IsDead(); }
class CTCSoundPlayer { public: void StopAttachedSound(unsigned long s) { char b[32]; std::sprintf(b, "StopSound(%lu)", s); Log(b); } };
class CTCFireballSpell {
public:
    CThing* ParentThing; unsigned long InitLoopingSound; CIntelligentPointer<CThing> FireballBuildUpEffect;
    void FinalCleanup();
};
void CTCFireballSpell::FinalCleanup()
{
    CTCSoundPlayer* sound;
    if (ParentThing->GetInterface(&sound, 0x10))
        sound->StopAttachedSound(InitLoopingSound);
    if (IS_THING_ALIVE(FireballBuildUpEffect))
        FireballBuildUpEffect.GetPItem()->Kill(true);
    CTCSpecialAbilities* abilities;
    if (ParentThing->GetInterface(&abilities, 0xa7) && abilities->CurrentAbility == 0x10) {
        abilities->AbilityMutexLock = false;
        abilities->CurrentAbility = 0;
    }
}
int main()
{
    CThing hero, effect; CTCSoundPlayer sp; CTCSpecialAbilities ab; ab.AbilityMutexLock = true; ab.CurrentAbility = 0x10;
    hero.Iface[0x10] = &sp; hero.Iface[0xa7] = &ab;
    CTCFireballSpell f; f.ParentThing = &hero; f.InitLoopingSound = 44; f.FireballBuildUpEffect.Item = &effect;
    int ok = 1;
    f.FinalCleanup(); ok &= Check("StopSound(44);Kill(1);"); ok &= !ab.AbilityMutexLock && ab.CurrentAbility == 0;
    effect.Dead = true; ab.CurrentAbility = 3; ab.AbilityMutexLock = true; hero.Iface[0x10] = 0;
    f.FinalCleanup(); ok &= Check(""); ok &= ab.AbilityMutexLock && ab.CurrentAbility == 3;
    if (ok) std::printf("007c90a0_TEST PASS\n");
    return ok ? 0 : 1;
}