// CAIStateGroup_MinionWorkInInn::CleanBench() -- behaviour test: no bench / no free point returns early; a failed move or look
// frees the point; a full run plays ST_WIPE_TABLE facing pos + forward and frees the point afterwards.
#include <stdio.h>
#include <string.h>

struct Vec { float X, Y, Z; };
static Vec Add(const Vec& a, const Vec& b) { Vec r; r.X = a.X + b.X; r.Y = a.Y + b.Y; r.Z = a.Z + b.Z; return r; }
struct Points {
    bool HasFree; long Freed; int FreeCalls;
    bool GetFreeActionPoint(Vec& pos, Vec& forward, long& index)
    { if (!HasFree) return false; pos.X = 1; pos.Y = 2; pos.Z = 3; forward.X = 0; forward.Y = 1; forward.Z = 0; index = 7; return true; }
    void SetActionPointAsFree(long index) { Freed = index; ++FreeCalls; }
};
struct Group {
    Points* Bench; int MoveResult, LookResult; Vec LookTarget; char Played[32]; Vec CreaturePos;
    Points* GetFreeBench() { return Bench; }
    int AIAction_MoveToPos(const Vec&, bool& reached) { reached = MoveResult == 2; return MoveResult; }
    int AIAction_LookAtPos(const Vec& target) { LookTarget = target; return LookResult; }
    void AIAction_PerformAction(const char* anim) { strcpy(Played, anim); }
    void CleanBench()
    {
        Points* points = GetFreeBench();
        if (!points) return;
        long index; Vec pos, forward;
        if (!points->GetFreeActionPoint(pos, forward, index)) return;
        {
            bool reached;
            if (AIAction_MoveToPos(pos, reached) != 2) { points->SetActionPointAsFree(index); return; }
        }
        Vec target = Add(forward, CreaturePos);
        if (AIAction_LookAtPos(target) != 2) { points->SetActionPointAsFree(index); return; }
        AIAction_PerformAction("ST_WIPE_TABLE");
        points->SetActionPointAsFree(index);
    }
};

int main()
{
    bool ok = true;
    Group g; Points p;
    g.CreaturePos.X = 10; g.CreaturePos.Y = 20; g.CreaturePos.Z = 30;
    g.Bench = 0; g.Played[0] = 0; g.CleanBench(); ok = ok && g.Played[0] == 0;
    p.HasFree = false; p.FreeCalls = 0; g.Bench = &p; g.CleanBench(); ok = ok && p.FreeCalls == 0 && g.Played[0] == 0;
    p.HasFree = true; g.MoveResult = 1; g.CleanBench(); ok = ok && p.FreeCalls == 1 && p.Freed == 7 && g.Played[0] == 0;
    g.MoveResult = 2; g.LookResult = 0; g.CleanBench(); ok = ok && p.FreeCalls == 2 && g.Played[0] == 0;
    g.LookResult = 2; g.CleanBench();
    ok = ok && p.FreeCalls == 3 && strcmp(g.Played, "ST_WIPE_TABLE") == 0 && g.LookTarget.X == 10 && g.LookTarget.Y == 21 && g.LookTarget.Z == 30;
    printf(ok ? "0090bd40_TEST PASS\n" : "0090bd40_TEST FAIL\n");
    return ok ? 0 : 1;
}