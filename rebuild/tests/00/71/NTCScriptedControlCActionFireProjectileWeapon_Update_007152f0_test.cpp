// Behaviour test for 0x007152F0 NTCScriptedControl::CActionFireProjectileWeapon::Update
#include <cstdio>
struct CAction { long ActionID; bool Finished; };
struct CCreature {
    CAction* PCurrentAction; bool performing;
    bool IsPerformingAnyAction() { return performing; }
    void SetCurrentAction(CAction* a) { PCurrentAction = a; performing = true; }
};
struct CTarget { bool valid, alive; bool IsValid() const { return valid; } void Refresh() { valid = true; } bool IsAlive() const { return alive; } };
struct CFire {
    CTarget PTarget; CAction* queue[4]; int head, tail; long CurrentActionID; bool Terminated; CCreature* creature;
    bool empty() const { return head == tail; }
    void Update()
    {
        if (!PTarget.IsValid())
            PTarget.Refresh();
        CCreature* c = creature;
        if (!PTarget.IsAlive() || empty()) {
            if (!c->IsPerformingAnyAction() || c->PCurrentAction->ActionID != CurrentActionID)
                Terminated = true;
        }
        if (empty())
            return;
        if (c->IsPerformingAnyAction() && !c->PCurrentAction->Finished)
            return;
        c->SetCurrentAction(queue[head]);
        CurrentActionID = c->PCurrentAction->ActionID;
        ++head;
    }
};
int main()
{
    CAction aim = { 11, false }, fire = { 12, false };
    CCreature cr; cr.PCurrentAction = 0; cr.performing = false;
    CFire f; f.PTarget.valid = false; f.PTarget.alive = true; f.queue[0] = &aim; f.queue[1] = &fire;
    f.head = 0; f.tail = 2; f.CurrentActionID = -1; f.Terminated = false; f.creature = &cr;
    f.Update();
    int ok = f.PTarget.valid && cr.PCurrentAction == &aim && f.CurrentActionID == 11 && f.head == 1 && !f.Terminated;
    f.Update();
    ok = ok && cr.PCurrentAction == &aim && f.head == 1;
    aim.Finished = true;
    f.Update();
    ok = ok && cr.PCurrentAction == &fire && f.CurrentActionID == 12 && f.head == 2 && !f.Terminated;
    f.Update();
    ok = ok && !f.Terminated;
    cr.performing = false;
    f.Update();
    ok = ok && f.Terminated;
    if (!ok) { std::printf("007152f0_TEST FAIL\n"); return 1; }
    std::printf("007152f0_TEST PASS\n");
    return 0;
}