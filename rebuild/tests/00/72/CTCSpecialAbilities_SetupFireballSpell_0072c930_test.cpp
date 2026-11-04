// Behaviour test for CTCSpecialAbilities::SetupFireballSpell (0x0072C930): a model of the row -- busy creature: false and no
// component; free creature without the spell: the component is added once (active) and its weapon state set; with it: true only.
#include <stdio.h>
#include <string.h>
static int g_added = 0, g_state = -1;
struct CTCFireballSpell { void SetWeaponState(int s) { g_state = s; } };
static CTCFireballSpell g_spell;
struct CThingCreatureBase { bool Busy; bool HasSpell; int Weapon;
    bool IsFreeToPerformAction() const { return !Busy; }
    bool HasTC() const { return HasSpell; }
    CTCFireballSpell* AddTC(const char* n, bool act, const void* p) { if (!strcmp(n, "CTCFireballSpell") && act && !p) { ++g_added; HasSpell = true; } return &g_spell; } };
static int CalculateWeaponState(CThingCreatureBase& c) { return c.Weapon; }
struct CTCSpecialAbilities { CThingCreatureBase* Creature;
    bool SetupFireballSpell() {
        if (Creature->IsFreeToPerformAction()) {
            if (!Creature->HasTC()) { CTCFireballSpell* s = Creature->AddTC("CTCFireballSpell", true, 0); s->SetWeaponState(CalculateWeaponState(*Creature)); }
            return true;
        }
        return false;
    } };
int main()
{
    CThingCreatureBase c; c.Busy = true; c.HasSpell = false; c.Weapon = 3; CTCSpecialAbilities a; a.Creature = &c;
    int ok = !a.SetupFireballSpell() && g_added == 0;
    c.Busy = false; ok = ok && a.SetupFireballSpell() && g_added == 1 && g_state == 3;
    ok = ok && a.SetupFireballSpell() && g_added == 1;
    printf(ok ? "0072c930_TEST PASS\n" : "0072c930_TEST FAIL\n");
    return ok ? 0 : 1;
}