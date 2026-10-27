// Behaviour test for 0x007A75F0 CTCBattleCharge::FinalCleanup: mock interfaces log every call; the body is the source's, verbatim.
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

class CThingAICreature : public CThing { public: void SuspendAI(bool s, int n) { char b[32]; std::sprintf(b, "SuspendAI(%d,%d)", (int)s, n); Log(b); } };
class CTCBattleCharge {
public:
    CThing* ParentThing;
    bool CompletedExitModes;
    void FinalCleanup();
};
void CTCBattleCharge::FinalCleanup()
{
    CThing* parent = ParentThing;
    if (parent && !parent->IsDead()) {
        CTCSpecialAbilities* abilities;
        if (parent->GetInterface(&abilities, 0xa7)) {
            if (abilities->CurrentAbility == 12) {
                abilities->AbilityMutexLock = false;
                abilities->CurrentAbility = 0;
            }
            if (!CompletedExitModes && parent->State == 1)
                ((CThingAICreature*)parent)->SuspendAI(false, 0);
        }
        if (!CompletedExitModes) {
            CTCCreatureModeManager* modes;
            if (parent->GetInterface(&modes, 0x31))
                modes->RemoveMode(0x23);
            if (!parent->IsDead() && parent->State == 2)
                parent->GetPlayer().EndSpecialMoveMode();
        }
    }
    if (parent->HasTC(0x109))
        parent->Kill(false);
}
int main()
{
    CThingAICreature t; CTCSpecialAbilities ab; CTCCreatureModeManager mm;
    ab.AbilityMutexLock = true; ab.CurrentAbility = 12; t.Iface[0xa7] = &ab; t.Iface[0x31] = &mm; t.State = 1;
    CTCBattleCharge c; c.ParentThing = &t; c.CompletedExitModes = false;
    int ok = 1;
    c.FinalCleanup(); ok &= Check("SuspendAI(0,0);RemoveMode(23);");
    ok &= !ab.AbilityMutexLock && ab.CurrentAbility == 0;
    t.State = 2; t.TCMask = 0x200; ab.CurrentAbility = 5; ab.AbilityMutexLock = true;
    c.FinalCleanup(); ok &= Check("RemoveMode(23);EndSpecialMove;Kill(0);");
    ok &= ab.AbilityMutexLock && ab.CurrentAbility == 5;
    c.CompletedExitModes = true; c.FinalCleanup(); ok &= Check("Kill(0);");
    t.Dead = true; c.CompletedExitModes = false; t.TCMask = 0; c.FinalCleanup(); ok &= Check("");
    if (ok) std::printf("007a75f0_TEST PASS\n");
    return ok ? 0 : 1;
}