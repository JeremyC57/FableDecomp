// Behaviour test for CAIStateGroupBase::AIAction_MoveToPossiblyBlockedObjectAndFace (0x00980770): guards, approach, free-point wait with timeout, claim check, walk and face; the point is freed on failures.
#include <cstdio>
#include <cstring>
struct Sim {
    bool alive, filterOk, hasPoint, hasRoute, hasClaim, claimed, filterLost;
    int routeResult, approachResult, moveResult, lookResult, wanderResult;
    int pointFreeAfter, frame, endFrames, freed, approaches, wanders;
    bool GetFreePoint() { return pointFreeAfter-- <= 0; }
    int Run()
    {
        if (!alive || !filterOk || !hasPoint) return 3;
        if (hasRoute && routeResult != 2) return routeResult;
        int r;
        for (;;) { ++approaches; r = approachResult; if (r == 2) break; if (r == 4) { approachResult = 2; continue; } return r; }
        int endFrame = frame + endFrames;
        while (!GetFreePoint()) {
            ++wanders;
            if (wanderResult == 1) return 1;
            if (++frame > endFrame) return 3;
        }
        if (!hasClaim || claimed) { ++freed; return 3; }
        r = moveResult;
        if (r != 2 || filterLost) { ++freed; if (r == 2) r = 3; return r; }
        r = lookResult;
        if (r == 2) return 2;
        ++freed;
        return r;
    }
};
static Sim base()
{
    Sim s; std::memset(&s, 0, sizeof(s));
    s.alive = s.filterOk = s.hasPoint = s.hasClaim = true;
    s.approachResult = 4; s.moveResult = 2; s.lookResult = 2; s.wanderResult = 2; s.endFrames = 3;
    return s;
}
int main()
{
    Sim a = base();
    if (a.Run() != 2 || a.approaches != 2 || a.freed != 0) { std::printf("00980770_TEST FAIL\n"); return 1; }
    Sim b = base(); b.pointFreeAfter = 10;
    if (b.Run() != 3 || b.wanders != 4) { std::printf("00980770_TEST FAIL\n"); return 1; }
    Sim c = base(); c.pointFreeAfter = 1; c.wanderResult = 1;
    if (c.Run() != 1) { std::printf("00980770_TEST FAIL\n"); return 1; }
    Sim d = base(); d.claimed = true;
    if (d.Run() != 3 || d.freed != 1) { std::printf("00980770_TEST FAIL\n"); return 1; }
    Sim e = base(); e.moveResult = 0;
    if (e.Run() != 0 || e.freed != 1) { std::printf("00980770_TEST FAIL\n"); return 1; }
    Sim h = base(); h.filterLost = true;
    if (h.Run() != 3 || h.freed != 1) { std::printf("00980770_TEST FAIL\n"); return 1; }
    Sim f = base(); f.hasRoute = true; f.routeResult = 1;
    if (f.Run() != 1 || f.approaches != 0) { std::printf("00980770_TEST FAIL\n"); return 1; }
    Sim g = base(); g.filterOk = false;
    if (g.Run() != 3) { std::printf("00980770_TEST FAIL\n"); return 1; }
    std::printf("00980770_TEST PASS\n");
    return 0;
}