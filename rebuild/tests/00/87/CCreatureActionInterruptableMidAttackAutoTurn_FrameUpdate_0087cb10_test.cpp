// 0087cb10 behaviour test: CCreatureAction_InterruptableMidAttackAutoTurn::FrameUpdate (match.cpp body)
#include <stdio.h>
#include <map>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
struct CEventBase { virtual ~CEventBase() {} virtual int IsMissedStrike() const { return 0; } };
struct CEventMissedStrike : CEventBase { int IsMissedStrike() const { return 1; } };
class CTCEntityEvents { public: int Missed; void AddEvent(CEventBase* e) { if (e->IsMissedStrike()) ++Missed; delete e; } };
class CTCStealth { public: bool Able; bool IsAbleToFlourishDangerThings() { return Able; } };
class CTCCombat { public: int SetValidCalls; bool LastValid; bool Flag94; void SetMeleeFlourishValid(bool v) { ++SetValidCalls; LastValid = v; } };
class CWeaponDef { public: bool AllowsFlourish; };
template <class T> class CDefPointer { public: T* Data; T* operator->() const { return Data; } bool IsValid() const { return Data != 0; } };
struct CTC68 { CDefPointer<CWeaponDef> PDef; CDefPointer<CWeaponDef> GetDef() const { return PDef; } };
class CThing { public:
    std::map<long, void*> TCs;
    template <class T> bool GetInterface(T** pp, long type) { std::map<long, void*>::iterator i = TCs.find(type); if (i == TCs.end()) return false; *pp = (T*)i->second; return true; }
};
static int g_baseUpdates;
class CCombatActionBase { public:
    bool R19, R17; CThing* Creature; CDefPointer<CWeaponDef> PMeleeAbilityDef; bool HitCreature;
    bool v17() { return R17; } bool v19() { return R19; }
    void FrameUpdate() { ++g_baseUpdates; }
    CThing* GetCreature() const { return Creature; }
    bool HasMeleeAbilityDef() const { return PMeleeAbilityDef.IsValid(); }
};
class CCreatureAction_InterruptableMidAttackAutoTurn : public CCombatActionBase { public: bool PerformedStrikeEnd; void FrameUpdate(); };
void CCreatureAction_InterruptableMidAttackAutoTurn::FrameUpdate()
{
    if (v19() && v17() && !PerformedStrikeEnd) {
        PerformedStrikeEnd = true;
        bool flourish = false;
        bool valid;
        if (!HitCreature) {
            CTCEntityEvents* events;
            GetCreature()->GetInterface(&events, 0x42);
            events->AddEvent(new CEventMissedStrike);
            valid = true;
        } else {
            CTC68* tc68;
            if (GetCreature()->GetInterface(&tc68, 0x68) && tc68->GetDef()->AllowsFlourish)
                flourish = true;
            CTCStealth* stealth;
            valid = (GetCreature()->GetInterface(&stealth, 0x36) && !stealth->IsAbleToFlourishDangerThings()) || flourish;
        }
        CTCCombat* combat;
        if (valid && GetCreature()->GetInterface(&combat, 0x30))
            combat->SetMeleeFlourishValid(false);
        if (HasMeleeAbilityDef() && GetCreature()->GetInterface(&combat, 0x30))
            combat->Flag94 = false;
    }
    CCombatActionBase::FrameUpdate();
}
int main()
{
    CTCEntityEvents ev = { 0 }; CTCStealth st = { false }; CTCCombat cb = { 0, true, true };
    CWeaponDef wdFlourish = { true }, wdPlain = { false }; CTC68 tc68; tc68.PDef.Data = &wdPlain;
    CThing t; t.TCs[0x42] = &ev; t.TCs[0x36] = &st; t.TCs[0x30] = &cb; t.TCs[0x68] = &tc68;
    CCreatureAction_InterruptableMidAttackAutoTurn a;
    a.R19 = a.R17 = true; a.Creature = &t; a.PMeleeAbilityDef.Data = &wdPlain; a.HitCreature = false; a.PerformedStrikeEnd = false;
    g_baseUpdates = 0;
    a.FrameUpdate();   // missed: event posted, flourish invalidated, Flag94 cleared
    CHECK(ev.Missed == 1 && cb.SetValidCalls == 1 && cb.LastValid == false && cb.Flag94 == false && a.PerformedStrikeEnd && g_baseUpdates == 1);
    a.FrameUpdate();   // second frame: only the base update
    CHECK(ev.Missed == 1 && cb.SetValidCalls == 1 && g_baseUpdates == 2);
    // hit, stealth able to flourish, no 0x68 flourish: valid = false; no ability def -> Flag94 untouched
    a.PerformedStrikeEnd = false; a.HitCreature = true; st.Able = true; a.PMeleeAbilityDef.Data = 0; cb.Flag94 = true;
    a.FrameUpdate();
    CHECK(cb.SetValidCalls == 1 && cb.Flag94 == true && ev.Missed == 1);
    // hit, stealth unable: valid
    a.PerformedStrikeEnd = false; st.Able = false; a.FrameUpdate();
    CHECK(cb.SetValidCalls == 2);
    // hit, stealth able but the 0x68 def allows a flourish: valid
    a.PerformedStrikeEnd = false; st.Able = true; tc68.PDef.Data = &wdFlourish; a.FrameUpdate();
    CHECK(cb.SetValidCalls == 3);
    // hit, no stealth component, no flourish: invalid
    a.PerformedStrikeEnd = false; t.TCs.erase(0x36); tc68.PDef.Data = &wdPlain; a.FrameUpdate();
    CHECK(cb.SetValidCalls == 3);
    // v17 false: nothing but the base update
    a.PerformedStrikeEnd = false; a.R17 = false; int before = g_baseUpdates; a.FrameUpdate();
    CHECK(!a.PerformedStrikeEnd && g_baseUpdates == before + 1);
    if (!g_fail) printf("0087cb10_TEST PASS\n");
    return g_fail;
}