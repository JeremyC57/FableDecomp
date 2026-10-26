// Behaviour test for CAIStateGroup_MeleeCombatNearbyBase::RequestMoney (0x0093D580): model copy with stub callees.
#include <cstdio>

class CTCVillage { public: long fineDue; long GetFineAmountForCurrentCrimes() { return fineDue; } };
struct CTCVillager { CTCVillage* village; };
static CTCVillage* GFGetPVillageInterface(CTCVillager* v) { return v->village; }
class CTCFineDialogue { public:
    bool allowed; int shown; long fine, amount; CTCVillage* village;
    bool IsAllowed() { return allowed; }
    void Show(long f, long a, CTCVillage* v) { ++shown; fine = f; amount = a; village = v; }
};
struct CTCHeroInterface4 { long m_amount; };
class CTCTargetingAI { public: int updated; void UpdatePBestEnemyCreatureForAttack(bool force) { if (force) ++updated; } };
class CThing { public:
    unsigned char m_flags91;
    CTCFineDialogue* fine; CTCHeroInterface4* tc4; CTCTargetingAI* targeting; CTCVillager* villager;
    bool GetInterface(CTCFineDialogue** o, long) { if (!fine) return false; *o = fine; return true; }
    bool GetInterface(CTCHeroInterface4** o, long) { if (!tc4) return false; *o = tc4; return true; }
    bool GetInterface(CTCTargetingAI** o, long) { if (!targeting) return false; *o = targeting; return true; }
    unsigned long HasTC(long) const { return villager != 0; }
    template<class T> T* GetTC(long) { return villager; }
};
struct CPointer { CThing* item; CThing* GetPItem() const { return item; } };
class CAIBrain { public: int yields; CThing* PCreature; void Yield() { ++yields; } };
static CThing* g_hero;

struct CAIStateGroup_MeleeCombatNearbyBase {
    CAIBrain* PBrain;
    CPointer PEnemy;
    bool runnable; int terminated;
    bool IsStillRunnable(bool) { return runnable; }
    void Terminate() { ++terminated; }
    CThing* GetHero() { return g_hero; }
    void RequestMoney();
};

void CAIStateGroup_MeleeCombatNearbyBase::RequestMoney()
{
    CThing* hero = GetHero();
    CTCFineDialogue* fine;
    if (hero != 0 && !(hero->m_flags91 & 1) && hero == PEnemy.GetPItem() && hero->GetInterface(&fine, 0xf3)) {
        if (fine->IsAllowed()) {
            CThing* creature = PBrain->PCreature;
            if (creature->HasTC(0x23)) {
                CTCVillager* villager = creature->GetTC<CTCVillager>(0x23);
                if (GFGetPVillageInterface(villager) != 0) {
                    CTCVillage* village = GFGetPVillageInterface(villager);
                    long amount = village->GetFineAmountForCurrentCrimes();
                    if (amount > 0) {
                        long heroAmount = 0;
                        CTCHeroInterface4* tc4;
                        if (hero->GetInterface(&tc4, 4))
                            heroAmount = tc4->m_amount;
                        fine->Show(amount, heroAmount, village);
                        PBrain->Yield();
                        CTCTargetingAI* targeting;
                        if (PBrain->PCreature->GetInterface(&targeting, 8))
                            targeting->UpdatePBestEnemyCreatureForAttack(true);
                        if (!IsStillRunnable(true))
                            Terminate();
                    }
                }
            }
        }
    }
}

int main()
{
    int ok = 1;
    CTCFineDialogue dlg = { true, 0, 0, 0, 0 };
    CTCHeroInterface4 money = { 250 };
    CThing hero = { 0, &dlg, &money, 0, 0 };
    CTCVillage village = { 40 };
    CTCVillager villager = { &village };
    CTCTargetingAI targeting = { 0 };
    CThing creature = { 0, 0, 0, &targeting, &villager };
    CAIBrain brain = { 0, &creature };
    CAIStateGroup_MeleeCombatNearbyBase s; s.PBrain = &brain; s.PEnemy.item = &hero; s.runnable = false; s.terminated = 0;
    g_hero = &hero;
    s.RequestMoney();
    if (dlg.shown != 1 || dlg.fine != 40 || dlg.amount != 250 || dlg.village != &village || brain.yields != 1 || targeting.updated != 1 || s.terminated != 1) ok = 0;
    hero.tc4 = 0; s.runnable = true; s.RequestMoney();
    if (dlg.shown != 2 || dlg.amount != 0 || s.terminated != 1) ok = 0;
    village.fineDue = 0; s.RequestMoney(); if (dlg.shown != 2) ok = 0;
    village.fineDue = 5; villager.village = 0; s.RequestMoney(); if (dlg.shown != 2) ok = 0;
    villager.village = &village; dlg.allowed = false; s.RequestMoney(); if (dlg.shown != 2) ok = 0;
    dlg.allowed = true; hero.m_flags91 = 1; s.RequestMoney(); if (dlg.shown != 2) ok = 0;
    hero.m_flags91 = 0; s.PEnemy.item = &creature; s.RequestMoney(); if (dlg.shown != 2) ok = 0;
    s.PEnemy.item = &hero; g_hero = 0; s.RequestMoney(); if (dlg.shown != 2) ok = 0;
    g_hero = &hero; s.RequestMoney(); if (dlg.shown != 3) ok = 0;
    if (!ok) { std::printf("0093d580_TEST FAIL\n"); return 1; }
    std::printf("0093d580_TEST PASS\n");
    return 0;
}