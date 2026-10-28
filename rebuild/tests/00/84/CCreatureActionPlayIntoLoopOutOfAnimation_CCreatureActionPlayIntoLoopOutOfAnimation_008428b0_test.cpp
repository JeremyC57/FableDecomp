// behaviour test for 0x008428b0 CCreatureAction_PlayIntoLoopOutOfAnimation::CCreatureAction_PlayIntoLoopOutOfAnimation(CThingCreatureBase&, const CCharString& loop, const CCharString& outOf, long loops, float frameToFinish, bool, long, long) (private):
// the base plays the random loop animation; the action starts in STAGE_LOOP with the given loop count, the finish frame
// truncated from the float and the base's AnimDelayTime cleared in the body
#include <cstdio>
#include <cstring>
enum EStage { STAGE_INTO, STAGE_LOOP, STAGE_OUT_OF };
struct Base {
    float a, b; const char* anim; long c, d; float AnimDelayTime;
    Base(float aa, float bb, const char* an, long cc, long dd) : a(aa), b(bb), anim(an), c(cc), d(dd), AnimDelayTime(0.5f) {}
};
struct Action : Base {
    EStage Stage; long NumberOfTimesToLoop; long FrameToFinish; bool UseAnimationMovement; const char* LoopAnimName; const char* OutOfAnimName;
    Action(const char* loop, const char* outOf, long n, float frameToFinish, bool u, long c, long d)
        : Base(-1.0f, 1.0f, loop, c, d), Stage(STAGE_LOOP), NumberOfTimesToLoop(n), FrameToFinish((long)frameToFinish),
          UseAnimationMovement(u), LoopAnimName(loop), OutOfAnimName(outOf)
    {
        AnimDelayTime = 0;
    }
};
int main()
{
    int fails = 0;
    Action act("LOOP", "OUT", 3, 41.9f, false, 1, 2);
    if (act.a != -1.0f || act.b != 1.0f || std::strcmp(act.anim, "LOOP")) fails |= 1;
    if (act.Stage != STAGE_LOOP || act.NumberOfTimesToLoop != 3 || act.FrameToFinish != 41) fails |= 2;
    if (act.AnimDelayTime != 0.0f || act.UseAnimationMovement || act.c != 1 || act.d != 2) fails |= 4;
    if (std::strcmp(act.OutOfAnimName, "OUT")) fails |= 8;
    if (fails) { std::printf("008428b0_TEST FAIL %d\n", fails); return 1; }
    std::printf("008428b0_TEST PASS\n");
    return 0;
}