#include <cstdio>
#include <string>
// Behaviour model of CActionFollowCreature::OnActivate: mark active, unblock a creature that cannot move, swap a
// non-navigator physics component for "CTCPhysicsNavigator" (keeping a clone of the old one), start sneaking behind
// a live sneaking target, then save and clear the pushable flag unless dynamic obstacles are avoided.
struct Physics { std::string Name; bool Pushable; };
struct Creature { Physics* PPhysics; bool CanMove; bool Unblocked; std::string AddedTC; };
struct Follow {
    Creature* PCreature; bool Activated; bool TargetAlive; bool TargetSneaking; Physics* POld; bool Sneaking; bool AvoidDynamic; bool PrevPushable;
    void OnActivate()
    {
        Activated = true;
        if (!PCreature->CanMove) PCreature->Unblocked = true;
        POld = 0;
        Physics* p = PCreature->PPhysics;
        if (p->Name != "CTCPhysicsNavigator") { POld = new Physics(*p); std::string name("CTCPhysicsNavigator"); PCreature->AddedTC = name; }
        if (TargetAlive && TargetSneaking) Sneaking = true;
        if (!AvoidDynamic) { PrevPushable = PCreature->PPhysics->Pushable; PCreature->PPhysics->Pushable = false; }
    }
};
int main()
{
    Physics p = { "CTCPhysicsStandard", true };
    Creature c = { &p, false, false, "" };
    Follow f = { &c, false, true, true, 0, false, false, false };
    f.OnActivate();
    bool ok = f.Activated && c.Unblocked && f.POld != 0 && f.POld->Name == "CTCPhysicsStandard" && c.AddedTC == "CTCPhysicsNavigator"
        && f.Sneaking && f.PrevPushable && !p.Pushable;
    delete f.POld;
    Physics n = { "CTCPhysicsNavigator", false };
    Creature c2 = { &n, true, false, "" };
    Follow g = { &c2, false, false, true, 0, false, true, true };
    g.OnActivate();
    ok = ok && g.POld == 0 && c2.AddedTC == "" && !c2.Unblocked && !g.Sneaking && g.PrevPushable;
    std::printf(ok ? "00715710_TEST PASS\n" : "00715710_TEST FAIL\n");
    return ok ? 0 : 1;
}