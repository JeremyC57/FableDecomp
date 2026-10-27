// Behaviour test for 0x007CCD70 CTCMultiStrike::FinalCleanup: mock interfaces log every call; the body is the source's, verbatim.
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

class CTCInventoryWeapons { public: CIntelligentPointer<CThing> PActiveMeleeWeapon; };
class CTCCarrying { public: CThing* Primary; CThing* GetThingInPrimarySlot() const { return Primary; } };
class CTCMultiStrike {
public:
    CThing* ParentThing;
    CThing* Removed;
    void RemoveEffectOnWeapon(CThing* w) { Removed = w; Log("RemoveEffect"); }
    void FinalCleanup();
};
void CTCMultiStrike::FinalCleanup()
{
    const CThing* parent = ParentThing;
    CThing* weapon = 0;
    const CTCInventoryWeapons* weapons;
    const CTCCarrying* carrying;
    if (parent->GetInterface(&weapons, 0x13))
        weapon = weapons->PActiveMeleeWeapon.GetPItem();
    else if (parent->GetInterface(&carrying, 0x46))
        weapon = carrying->GetThingInPrimarySlot();
    if (weapon)
        RemoveEffectOnWeapon(weapon);
}
int main()
{
    CThing hero, sword, axe; CTCInventoryWeapons inv; CTCCarrying car; inv.PActiveMeleeWeapon.Item = &sword; car.Primary = &axe;
    CTCMultiStrike m; m.ParentThing = &hero; m.Removed = 0;
    int ok = 1;
    m.FinalCleanup(); ok &= Check("") && m.Removed == 0;
    hero.Iface[0x46] = &car; m.FinalCleanup(); ok &= Check("RemoveEffect;") && m.Removed == &axe;
    hero.Iface[0x13] = &inv; m.FinalCleanup(); ok &= Check("RemoveEffect;") && m.Removed == &sword;
    inv.PActiveMeleeWeapon.Item = 0; m.Removed = 0; m.FinalCleanup(); ok &= Check("") && m.Removed == 0;
    if (ok) std::printf("007ccd70_TEST PASS\n");
    return ok ? 0 : 1;
}