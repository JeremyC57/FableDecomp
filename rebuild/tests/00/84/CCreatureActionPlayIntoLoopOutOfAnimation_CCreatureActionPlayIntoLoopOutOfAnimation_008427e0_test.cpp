// behaviour test for 0x008427e0 CCreatureAction_PlayIntoLoopOutOfAnimation::CCreatureAction_PlayIntoLoopOutOfAnimation(CThingCreatureBase&, const CCharString& into, const CCharString& loop, const CCharString& outOf, float amountOfTimeToLoop, bool, long, long):
// the base plays the random "into" animation (-1 / 1, empty sounds, the two interruption longs); the action starts in
// STAGE_INTO with no loop count, FrameToFinish = (long)(fps * time + current frame) and the stored time then cleared
#include <cstdio>
#include <cstring>
enum EStage { STAGE_INTO, STAGE_LOOP, STAGE_OUT_OF };
static long gFrame = 100, gFps = 10;
static const char* gAnimName = 0;
struct Base {
    float a, b; const char* anim; long c, d; long animDelay;
    Base(float aa, float bb, const char* an, long cc, long dd) : a(aa), b(bb), anim(an), c(cc), d(dd), animDelay(7) {}
};
static const char* RandomAnim(const char* name) { gAnimName = name; return name; }
struct Action : Base {
    EStage Stage; long NumberOfTimesToLoop; float AmountOfTimeToLoop; long FrameToFinish; bool UseAnimationMovement;
    const char* LoopAnimName; const char* OutOfAnimName;
    Action(const char* into, const char* loop, const char* outOf, float amount, bool useMove, long c, long d)
        : Base(-1.0f, 1.0f, RandomAnim(into), c, d), Stage(STAGE_INTO), NumberOfTimesToLoop(0), AmountOfTimeToLoop(amount),
          UseAnimationMovement(useMove), LoopAnimName(loop), OutOfAnimName(outOf)
    {
        long framesPerSecond = gFps;
        FrameToFinish = (long)(framesPerSecond * AmountOfTimeToLoop + gFrame);
        AmountOfTimeToLoop = 0;
    }
};
int main()
{
    int fails = 0;
    Action act("INTO", "LOOP", "OUT", 2.5f, true, 3, 4);
    if (act.a != -1.0f || act.b != 1.0f || std::strcmp(act.anim, "INTO") || std::strcmp(gAnimName, "INTO")) fails |= 1;
    if (act.c != 3 || act.d != 4) fails |= 2;
    if (act.Stage != STAGE_INTO || act.NumberOfTimesToLoop != 0) fails |= 4;
    if (act.FrameToFinish != 125 || act.AmountOfTimeToLoop != 0.0f) fails |= 8;
    if (!act.UseAnimationMovement || std::strcmp(act.LoopAnimName, "LOOP") || std::strcmp(act.OutOfAnimName, "OUT")) fails |= 16;
    if (fails) { std::printf("008427e0_TEST FAIL %d\n", fails); return 1; }
    std::printf("008427e0_TEST PASS\n");
    return 0;
}