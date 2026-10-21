#include <cstdio>
#include <string>
// Behaviour model of CActionMoveTo::OnCleanup: release a claimed action point on a live target, re-add the saved
// physics component under its own name with a snapshot of the current physics state and delete it, restore the
// pushable flag unless dynamic obstacles were avoided (or the creature is flagged), then reset the navigator.
struct Snapshot { float X, Y, Z; int Bounds; };
static int g_freed = -1, g_resets = 0, g_pushable = -1; static std::string g_added; static Snapshot g_snap;
struct Physics { float X, Y, Z; int Bounds; std::string Name; };
struct Creature { Physics* PPhysics; bool Flagged; bool HasNavigator; };
struct MoveTo {
    Creature* PCreature; bool TargetAlive; Physics* POld; bool AvoidDynamic; bool PrevPushable; int ActionPoint; bool GotActionPoint;
    void OnCleanup()
    {
        if (GotActionPoint && TargetAlive) g_freed = ActionPoint;
        if (POld) {
            Physics* p = PCreature->PPhysics;
            Snapshot s; s.X = p->X; s.Y = p->Y; s.Z = p->Z; s.Bounds = p->Bounds;
            g_added = POld->Name; g_snap = s;
            delete POld; POld = 0;
        }
        if (!AvoidDynamic) { if (PCreature && !PCreature->Flagged) g_pushable = PrevPushable ? 1 : 0; }
        if (PCreature->HasNavigator) g_resets++;
    }
};
int main()
{
    Physics now = { 1.0f, 2.0f, 3.0f, 7, "Nav" };
    Creature c = { &now, false, true };
    Physics* old = new Physics; old->Name = "CTCPhysicsStandard";
    MoveTo m = { &c, true, old, false, true, 5, true };
    m.OnCleanup();
    bool ok = g_freed == 5 && g_added == "CTCPhysicsStandard" && g_snap.Y == 2.0f && g_snap.Bounds == 7 && m.POld == 0 && g_pushable == 1 && g_resets == 1;
    g_freed = -1; g_pushable = -1; c.Flagged = true; m.TargetAlive = false; m.OnCleanup();
    ok = ok && g_freed == -1 && g_pushable == -1 && g_resets == 2;
    std::printf(ok ? "00714730_TEST PASS\n" : "00714730_TEST FAIL\n");
    return ok ? 0 : 1;
}
