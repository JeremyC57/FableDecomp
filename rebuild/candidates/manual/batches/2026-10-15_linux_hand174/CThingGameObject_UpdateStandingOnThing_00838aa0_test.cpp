#include <cstdio>
struct Phys { bool a, b, c, Moving; int setCalls;
    bool IsStateA() const { return a; } bool IsStateB() const { return b; } bool IsStateC() const { return c; }
    void SetAsMoving(bool m) { Moving = m; ++setCalls; } };
struct Thing { Phys* PhysicsTC; };
struct IP { Thing* item; Thing* GetPItem() const { return item; } };
struct GO { unsigned char flags; Phys* PhysicsTC; IP PThingStandingOn;
    void Update() {
        if (flags & 0x20) return;
        Phys* p = PhysicsTC; if (!p) return;
        if (p->IsStateA()) return; if (p->IsStateB()) return; if (p->Moving) return; if (!p->IsStateC()) return;
        if (PThingStandingOn.GetPItem()) { if (!PThingStandingOn.GetPItem()->PhysicsTC) return; if (!PThingStandingOn.GetPItem()->PhysicsTC->Moving) return; }
        PhysicsTC->SetAsMoving(true); } };
int main()
{
    Phys p = { false, false, true, false, 0 };
    Phys other = { false, false, false, false, 0 };
    Thing on = { &other };
    GO g = { 0, &p, { 0 } };
    g.Update(); bool ok = p.setCalls == 1 && p.Moving;
    p.Moving = false; g.PThingStandingOn.item = &on; g.Update(); ok = ok && p.setCalls == 1;
    other.Moving = true; g.Update(); ok = ok && p.setCalls == 2;
    p.Moving = false; g.flags = 0x20; g.Update(); ok = ok && p.setCalls == 2;
    g.flags = 0; p.c = false; g.Update(); ok = ok && p.setCalls == 2;
    if (!ok) { std::printf("00838aa0_TEST FAIL\n"); return 1; }
    std::printf("00838aa0_TEST PASS\n");
    return 0;
}
