// Behaviour test for 006eabb0 CCombatManager::UpdateAttackingCombatantSequenceResets: a finished
// sequence outside its zone resets (and finishes a finishable action); a sequence that resets on a
// lost target resets and picks the next action only when the target cannot be attacked.
#include <cstdio>

struct CThing { bool attackable; };
struct CCreature { bool performing, freeToFinish; int finished; bool IsPerformingAnyAction() { return performing; } bool IsFreeToFinishAction() { return freeToFinish; } void FinishCurrentAction() { ++finished; } };
static bool IsAbleToBeAttacked(CThing* t) { return t->attackable; }
struct CSequence { bool finished, zoneValid, resetOnLost, ready;
    virtual ~CSequence() { }
    virtual bool IsFinished() { return finished; } virtual bool IsResetOnTargetLost() { return resetOnLost; } virtual bool IsReadyToReset() { return ready; }
    bool IsZoneValid() { return zoneValid; } };
struct CPtr { CSequence* m_pObject; void* m_pCount; };
struct CInfo { CCreature* Parent; CThing* Attacked; long CurrentSequence; CPtr* first; };
struct CManager {
    int resets, updates;
    void PerformSequenceReset(CCreature*) { ++resets; } void UpdateCurrentAction(CCreature*) { ++updates; }
    void Update(CInfo* info)
    {
        CCreature* parent = info->Parent; CThing* attacked = info->Attacked;
        CSequence* s = info->first[info->CurrentSequence].m_pObject;
        if (s->IsFinished() && !s->IsZoneValid()) {
            PerformSequenceReset(parent);
            if (parent->IsPerformingAnyAction() && parent->IsFreeToFinishAction()) parent->FinishCurrentAction();
            return;
        }
        if (s->IsResetOnTargetLost() && s->IsReadyToReset() && !IsAbleToBeAttacked(attacked)) { PerformSequenceReset(parent); UpdateCurrentAction(parent); }
    }
};

int main()
{
    CCreature c = { true, true, 0 }; CThing t = { false };
    CSequence s0, s1; s0.finished = false; s0.zoneValid = true; s0.resetOnLost = false; s0.ready = false;
    s1.finished = true; s1.zoneValid = false; s1.resetOnLost = true; s1.ready = true;
    CPtr seqs[2] = { { &s0, 0 }, { &s1, 0 } };
    CInfo info = { &c, &t, 1, seqs };
    CManager m; m.resets = 0; m.updates = 0;
    m.Update(&info);
    bool ok = m.resets == 1 && m.updates == 0 && c.finished == 1;
    s1.zoneValid = true; m.Update(&info);                      // not finished-out-of-zone: lost target path
    ok = ok && m.resets == 2 && m.updates == 1 && c.finished == 1;
    t.attackable = true; m.Update(&info);                       // target still attackable: nothing
    ok = ok && m.resets == 2 && m.updates == 1;
    info.CurrentSequence = 0; t.attackable = false; m.Update(&info);   // idle sequence: nothing
    ok = ok && m.resets == 2;
    if (!ok) { std::printf("006eabb0_TEST FAIL\n"); return 1; }
    std::printf("006eabb0_TEST PASS\n");
    return 0;
}