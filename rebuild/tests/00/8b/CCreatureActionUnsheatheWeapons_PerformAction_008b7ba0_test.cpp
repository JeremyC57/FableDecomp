// Behaviour test for CCreatureAction_UnsheatheWeapons::PerformAction (0x008B7BA0): the pending type picks the (un)sheathe call,
// a carried weapon refreshes the creature mode and makes SIMPLE_NOISE, then strike events are set up and the type cleared;
// no combat component or a pending type 0 does nothing.
#include <stdio.h>
#include <string.h>
static char g_log[256];
static void Log(const char* s) { strcat(g_log, s); strcat(g_log, ";"); }
class CTCCombat { public: int UnsheathingWeaponType; };
class CTCCreatureModeManager { public: void RefreshCurrentMode() { Log("refresh"); } };
class CThingCreatureBase { public:
    CTCCombat* Combat; CTCCreatureModeManager* Modes; bool CarryingWeapon;
    bool GetTC(CTCCombat** out) { if (!Combat) return false; *out = Combat; return true; }
    bool GetTC(CTCCreatureModeManager** out) { if (!Modes) return false; *out = Modes; return true; }
    void SheatheAllWeapons() { Log("sheatheAll"); }
    void UnsheatheAllWeapons() { Log("unsheatheAll"); }
    void UnsheatheDefaultWeapon() { Log("default"); }
    void UnsheatheMeleeWeapon() { Log("melee"); }
    void UnsheatheRangedWeapon() { Log("ranged"); }
    void MakeNoise(const char* noise) { Log(noise); }
};
class CTCCarrying { public: static bool IsThingCarryingWeapon(const CThingCreatureBase& t) { return t.CarryingWeapon; } };
class CCreatureAction_UnsheatheWeapons { public:
    CThingCreatureBase* PCreature;
    void SetupCombatStrikeEvents() { Log("strike"); }
    void PerformAction()
    {
        CTCCombat* combat;
        if (PCreature->GetTC(&combat)) {
            if (combat->UnsheathingWeaponType == 0)
                return;
            switch (combat->UnsheathingWeaponType) {
            case 1: PCreature->SheatheAllWeapons(); break;
            case 2: PCreature->UnsheatheAllWeapons(); break;
            case 3: PCreature->UnsheatheDefaultWeapon(); break;
            case 4: PCreature->UnsheatheMeleeWeapon(); break;
            case 5: PCreature->UnsheatheRangedWeapon(); break;
            }
            if (CTCCarrying::IsThingCarryingWeapon(*PCreature)) {
                CTCCreatureModeManager* modes;
                if (PCreature->GetTC(&modes))
                    modes->RefreshCurrentMode();
                PCreature->MakeNoise("SIMPLE_NOISE");
            }
            SetupCombatStrikeEvents();
            combat->UnsheathingWeaponType = 0;
        }
    }
};
static int Check(CThingCreatureBase& c, int type, const char* want)
{
    g_log[0] = 0;
    CCreatureAction_UnsheatheWeapons action; action.PCreature = &c;
    if (c.Combat) c.Combat->UnsheathingWeaponType = type;
    action.PerformAction();
    if (strcmp(g_log, want) != 0) return 0;
    if (c.Combat && c.Combat->UnsheathingWeaponType != 0) return 0;
    return 1;
}
int main()
{
    CTCCombat combat; CTCCreatureModeManager modes;
    CThingCreatureBase c; c.Combat = &combat; c.Modes = &modes; c.CarryingWeapon = false;
    int ok = Check(c, 0, "") && Check(c, 1, "sheatheAll;strike;") && Check(c, 2, "unsheatheAll;strike;")
          && Check(c, 3, "default;strike;") && Check(c, 4, "melee;strike;") && Check(c, 6, "strike;");
    c.CarryingWeapon = true;
    ok = ok && Check(c, 5, "ranged;refresh;SIMPLE_NOISE;strike;");
    c.Modes = 0;
    ok = ok && Check(c, 4, "melee;SIMPLE_NOISE;strike;");
    c.Combat = 0;
    ok = ok && Check(c, 2, "");
    if (!ok) { printf("008b7ba0_TEST FAIL\n"); return 1; }
    printf("008b7ba0_TEST PASS\n");
    return 0;
}