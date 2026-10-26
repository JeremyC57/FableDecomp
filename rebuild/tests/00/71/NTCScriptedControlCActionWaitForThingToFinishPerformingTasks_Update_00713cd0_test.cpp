// Behaviour test for 0x00713CD0 NTCScriptedControl::CActionWaitForThingToFinishPerformingTasks::Update
#include <cstdio>
struct SNode { SNode* next; };
struct CControl { SNode* node; bool Locked; bool empty() const { return node->next == node; } };
class CThing { public:
    unsigned long UsingComponents[9]; CControl* control;
    bool GetInterface(CControl** out, long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31))) { *out = control; return true; }
        return false;
    }
};
class CScriptThing { public:
    bool valid, alive; int refreshes; CThing* thing;
    bool IsValid() const { return valid; }
    void Refresh() { ++refreshes; valid = true; }
    bool IsAlive() const { return alive; }
    CThing* GetThing() const { return thing; }
};
struct CAction {
    bool Terminated; CScriptThing PTargetThing;
    void Update()
    {
        if (!PTargetThing.IsValid())
            PTargetThing.Refresh();
        if (PTargetThing.IsAlive()) {
            CControl* control;
            if (PTargetThing.GetThing()->GetInterface(&control, 0x1f)) {
                if (control->Locked || !control->empty())
                    return;
            }
        }
        Terminated = true;
    }
};
int main()
{
    SNode sentinel, n1; sentinel.next = &n1; n1.next = &sentinel;
    CControl c; c.node = &sentinel; c.Locked = false;
    CThing t; int i; for (i = 0; i < 9; ++i) t.UsingComponents[i] = 0;
    t.UsingComponents[0] = 0x80000000UL; t.control = &c;
    CAction a; a.Terminated = false; a.PTargetThing.valid = false; a.PTargetThing.alive = true;
    a.PTargetThing.refreshes = 0; a.PTargetThing.thing = &t;
    a.Update();
    int ok = !a.Terminated && a.PTargetThing.refreshes == 1;
    sentinel.next = &sentinel; c.Locked = true;
    a.Update();
    ok = ok && !a.Terminated && a.PTargetThing.refreshes == 1;
    c.Locked = false;
    a.Update();
    ok = ok && a.Terminated;
    a.Terminated = false; c.Locked = true; t.UsingComponents[0] = 0;
    a.Update();
    ok = ok && a.Terminated;
    a.Terminated = false; t.UsingComponents[0] = 0x80000000UL; a.PTargetThing.alive = false;
    a.Update();
    ok = ok && a.Terminated;
    if (!ok) { std::printf("00713cd0_TEST FAIL\n"); return 1; }
    std::printf("00713cd0_TEST PASS\n");
    return 0;
}