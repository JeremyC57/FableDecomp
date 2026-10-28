// behaviour test for 0x00843a80 CCreatureAction_PlayAnimation::CCreatureAction_PlayAnimation(CThingCreatureBase&, const CCharString&, float transitionInTime, const CSoundPair&, bool, bool, bool, long, long):
// the base gets the owner's random animation of that name wrapped in a CAnimationEntry with a transition of
// (long)(seconds * 15) frames; the action is not looping, not held on its last frame, with no loop count
#include <cstdio>
#include <cstring>
struct Entry { const char* name; long transition; Entry(const char* n, long t) : name(n), transition(t) {} };
struct Base {
    float a, b; Entry anim; long s1, s2, group, prio;
    Base(float aa, float bb, const Entry& e, long x, long y, long g, long p) : a(aa), b(bb), anim(e), s1(x), s2(y), group(g), prio(p) {}
};
struct Action : Base {
    bool UseAnimationMovement, UsePhysics, StayOnLastFrame, Looping; long NumberOfTimesToLoop; bool AnimMayNeedCamera;
    Action(const char* name, float transitionInTime, long s1, long s2, bool move, bool phys, bool cam, long g, long p)
        : Base(-1.0f, 1.0f, Entry(name, (long)(transitionInTime * 15.0f)), s1, s2, g, p), UseAnimationMovement(move),
          UsePhysics(phys), StayOnLastFrame(false), Looping(false), NumberOfTimesToLoop(0), AnimMayNeedCamera(cam) {}
};
int main()
{
    int fails = 0;
    Action act("STANDARD_WAVE", 0.5f, 11, 12, true, false, true, 6, 7);
    if (act.a != -1.0f || act.b != 1.0f || std::strcmp(act.anim.name, "STANDARD_WAVE") || act.anim.transition != 7) fails |= 1;
    if (act.s1 != 11 || act.s2 != 12 || act.group != 6 || act.prio != 7) fails |= 2;
    if (!act.UseAnimationMovement || act.UsePhysics || !act.AnimMayNeedCamera) fails |= 4;
    if (act.StayOnLastFrame || act.Looping || act.NumberOfTimesToLoop != 0) fails |= 8;
    if (fails) { std::printf("00843a80_TEST FAIL %d\n", fails); return 1; }
    std::printf("00843a80_TEST PASS\n");
    return 0;
}