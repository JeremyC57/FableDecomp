// Behaviour test for CAIStateGroup_SkillPractice::Update (0x0092B3A0): when PracticeSkills() succeeds and the group is not stopping, the next
// practice frame is scheduled one minute ahead (frame + ConstantFPS * 60); the group always terminates; IsStopping is only asked after success.
#include <cstdio>
static long gFrame = 1000, gFPS = 30;
static int gPracticeCalls, gStoppingCalls, gTerminateCalls; static bool gPracticeResult, gStopping;
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase() { }
    virtual void Update() = 0;
    bool IsStopping() const { ++gStoppingCalls; return gStopping; }
    void Terminate() { ++gTerminateCalls; }
};
class CAIStateGroup_ApprenticeHero : public CAIStateGroupBase { public:
    long NextFramePractice;
protected:
    virtual bool PracticeSkills() { return false; }
};
class CAIStateGroup_SkillPractice : public CAIStateGroup_ApprenticeHero { public:
    virtual void Update();
protected:
    virtual bool PracticeSkills() { ++gPracticeCalls; return gPracticeResult; }
};
class CWorld { public: static long GetFrame() { return gFrame; } };
class CMainGameComponent { public: static long GetConstantFPS() { return gFPS; } };
void CAIStateGroup_SkillPractice::Update()
{
    if (!PracticeSkills() || IsStopping()) {
        Terminate();
        return;
    }
    NextFramePractice = CWorld::GetFrame() + CMainGameComponent::GetConstantFPS() * 60;
    Terminate();
}
static void Reset() { gPracticeCalls = gStoppingCalls = gTerminateCalls = 0; }
int main()
{
    CAIStateGroup_SkillPractice g; CAIStateGroupBase* b = &g; g.NextFramePractice = -1;
    gPracticeResult = true; gStopping = false; Reset(); b->Update();
    if (g.NextFramePractice != 1000 + 30 * 60 || gPracticeCalls != 1 || gStoppingCalls != 1 || gTerminateCalls != 1) { std::printf("0092b3a0_TEST FAIL\n"); return 1; }
    g.NextFramePractice = -1; gStopping = true; Reset(); b->Update();
    if (g.NextFramePractice != -1 || gStoppingCalls != 1 || gTerminateCalls != 1) { std::printf("0092b3a0_TEST FAIL\n"); return 1; }
    gPracticeResult = false; gStopping = false; Reset(); b->Update();
    if (g.NextFramePractice != -1 || gStoppingCalls != 0 || gTerminateCalls != 1) { std::printf("0092b3a0_TEST FAIL\n"); return 1; }
    std::printf("0092b3a0_TEST PASS\n");
    return 0;
}