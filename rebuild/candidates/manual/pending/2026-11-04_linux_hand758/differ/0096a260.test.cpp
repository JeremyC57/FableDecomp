// Behaviour test for CAIStateGroup_WhisperRetreat::FindHitEvent (0x0096A260), g++ model: a fresh unblocked ranged hit (sets the range flag), else a fresh unblocked melee hit, else a fresh fired shot.
#include <cstdio>
#include <vector>
struct Event { int kind; bool ranged, blocked; long frame; };   // kind 0 hit, 1 shot
static bool FindHitEvent(bool hasEvents, const std::vector<Event>& ev, long lastHit, bool& isRange)
{
    isRange = false;
    if (!hasEvents) return false;
    for (size_t i = 0; i < ev.size(); ++i) if (ev[i].kind == 0 && ev[i].ranged && !ev[i].blocked && ev[i].frame > lastHit) { isRange = true; return true; }
    for (size_t i = 0; i < ev.size(); ++i) if (ev[i].kind == 0 && !ev[i].ranged && !ev[i].blocked && ev[i].frame > lastHit) return true;
    for (size_t i = 0; i < ev.size(); ++i) if (ev[i].kind == 1 && ev[i].frame > lastHit) return true;
    return false;
}
int main()
{
    bool r;
    std::vector<Event> e; Event a = { 0, true, true, 50 }, b = { 0, false, false, 50 }, c = { 1, false, false, 5 };
    e.push_back(a); e.push_back(b); e.push_back(c);
    int ok = FindHitEvent(true, e, 10, r) && !r;
    e[0].blocked = false; ok = ok && FindHitEvent(true, e, 10, r) && r;
    ok = ok && !FindHitEvent(true, e, 60, r) && !r && !FindHitEvent(false, e, 0, r);
    e[2].frame = 70; ok = ok && FindHitEvent(true, e, 60, r) && !r;
    std::printf(ok ? "0096a260_TEST PASS\n" : "0096a260_TEST FAIL\n");
    return ok ? 0 : 1;
}
