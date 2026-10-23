// Behaviour model of CAIStateGroup_MinionKidGroupTag::IsRunnable (0x00934920): seven-second cooldown, then join the nearest
// group (member within 8), else answer an invitation by creating a group, else post an own invitation unless one is queued.
#include <cstdio>
struct Sim {
    long frame, lastPlayed, fps; bool initiated;
    bool groupFound; float nearestDist; bool addOk; bool invitation; bool createOk; bool ownQueued;
    int posted, removed, created; long duration;
};
static bool IsRunnable(Sim& s)
{
    long fps = s.fps;
    if (s.frame - s.lastPlayed < fps * 7) return false;
    s.initiated = false;
    if (s.groupFound && s.nearestDist < 8.0f && s.addOk) return true;
    if (s.invitation) {
        ++s.created;
        if (s.createOk) { ++s.removed; s.initiated = true; return true; }
    }
    if (!s.ownQueued) { ++s.posted; s.duration = (long)(s.fps * 4.0f); }
    return false;
}
int main()
{
    int ok = 1;
    Sim a = { 100, 0, 10, true, true, 3.0f, true, false, false, false, 0, 0, 0, 0 };
    ok &= IsRunnable(a) && !a.initiated;
    Sim b = a; b.frame = 50; b.initiated = true; ok &= !IsRunnable(b) && b.initiated;
    Sim c = a; c.nearestDist = 9.0f; c.invitation = true; c.createOk = true; ok &= IsRunnable(c) && c.initiated && c.removed == 1;
    Sim d = c; d.createOk = false; d.removed = 0; ok &= !IsRunnable(d) && d.posted == 1 && d.duration == 40;
    Sim e = d; e.posted = 0; e.ownQueued = true; ok &= !IsRunnable(e) && e.posted == 0;
    std::printf(ok ? "00934920_TEST PASS\n" : "00934920_TEST FAIL\n");
    return ok ? 0 : 1;
}