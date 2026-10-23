// 0072c790 behaviour test: CTCSpecialAbilities::SetupEnflameSpell (match.cpp body, host-modelled types)
#include <stdio.h>
#include <map>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
class CThing;
struct CPlayer { bool LockFlag; };
struct CIntelligentPointerThing { CThing* m_pItem; CIntelligentPointerThing() : m_pItem(0) {} CIntelligentPointerThing(CThing* i) : m_pItem(i) {} CThing* GetPItem() const { return m_pItem; } void SetPItem(CThing* i) { m_pItem = i; } };
struct CTCInventory { CThing* Primary; CThing* GetThingInPrimarySlot() { return Primary; } };
struct CTCCombat { CThing* Target; CThing* GetCurrentTarget() { return Target; } };
class CThing { public:
    std::map<long, void*> TCs; unsigned char Flags6c; bool Dead; CPlayer* Player;
    CPlayer* GetPlayer() { return Player; }
    bool Has(long k) const { return TCs.count(k) != 0; }
    void* Get(long k) { return TCs.find(k)->second; }
};
struct CCombatActionBase { int Kind; CThing* Target; bool Lock; CThing* Weapon; virtual ~CCombatActionBase() {} };
struct CCreatureAction_CastWillEnflameSpell : CCombatActionBase {
    CIntelligentPointerThing WeaponPtr;
    CCreatureAction_CastWillEnflameSpell(CThing*, CThing* t, bool l, long) { Kind = 1; Target = t; Lock = l; Weapon = 0; }
};
static CCombatActionBase* g_wrapped;
struct CCombatAction_GenericStrikeResponse : CCombatActionBase {
    CCombatAction_GenericStrikeResponse(CThing*, const char*, CCombatActionBase* a, long) { Kind = 2; g_wrapped = a; Target = a->Target; Lock = a->Lock; Weapon = ((CCreatureAction_CastWillEnflameSpell*)a)->WeaponPtr.GetPItem(); }
};
static int g_kind; static CThing* g_target; static bool g_lock; static CThing* g_weapon;
struct CThingCreatureBase : CThing { bool Accept; bool SetCurrentAction(const CCombatActionBase& a) { g_kind = a.Kind; g_target = a.Target; g_lock = a.Lock; g_weapon = a.Weapon; return Accept; } };
struct CTCSpecialAbilities { CThingCreatureBase* Creature; bool SetupEnflameSpell(); };
bool CTCSpecialAbilities::SetupEnflameSpell()
{
    CTCInventory* inventory = 0;
    if (Creature->Has(0x46)) inventory = (CTCInventory*)Creature->Get(0x46);
    CThing* weapon = inventory->GetThingInPrimarySlot();
    CThing* target = 0;
    bool lock = false;
    if (Creature->Has(8)) {
        CTCCombat* combat = (CTCCombat*)Creature->Get(8);
        target = combat->GetCurrentTarget();
        if (target && !target->Dead && (target->Flags6c & 8) && (Creature->Flags6c & 0x20))
            lock = Creature->GetPlayer()->LockFlag;
    }
    if (weapon && !weapon->Dead) {
        CCreatureAction_CastWillEnflameSpell* action = new CCreatureAction_CastWillEnflameSpell(Creature, target, lock, 0);
        action->WeaponPtr.SetPItem(CIntelligentPointerThing(weapon).GetPItem());
        CCombatAction_GenericStrikeResponse response(Creature, "StrikeResponse", action, 0);
        bool r = Creature->SetCurrentAction(*(CCombatActionBase*)&response);
        delete action;
        return r;
    }
    return Creature->SetCurrentAction(CCreatureAction_CastWillEnflameSpell(Creature, target, lock, 0));
}
int main()
{
    CPlayer pl = { true };
    CThingCreatureBase c; c.Flags6c = 0x20; c.Dead = false; c.Player = &pl; c.Accept = true;
    CThing w; w.Dead = false; CThing t; t.Dead = false; t.Flags6c = 8;
    CTCInventory inv = { &w }; CTCCombat cb = { &t };
    c.TCs[0x46] = &inv; c.TCs[8] = &cb;
    CTCSpecialAbilities s; s.Creature = &c;
    CHECK(s.SetupEnflameSpell() && g_kind == 2 && g_target == &t && g_lock && g_weapon == &w);
    pl.LockFlag = false; CHECK(s.SetupEnflameSpell() && !g_lock);
    pl.LockFlag = true; t.Flags6c = 0; CHECK(s.SetupEnflameSpell() && !g_lock);
    w.Dead = true; c.Accept = false; CHECK(!s.SetupEnflameSpell() && g_kind == 1 && g_target == &t && g_weapon == 0);
    c.TCs.erase(8); inv.Primary = 0; c.Accept = true; CHECK(s.SetupEnflameSpell() && g_kind == 1 && g_target == 0 && !g_lock);
    if (!g_fail) printf("0072c790_TEST PASS\n");
    return g_fail;
}