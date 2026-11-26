// Behaviour test for 0x0091BCF0 CAIStateGroup_StandStill::Update(): loops while the do-nothing action returns AI_ACTION_IN_PROGRESS (4).
#include <cstdio>
enum EAIActionReturn { AI_ACTION_NULL=0,AI_ACTION_TERMINATE_PROCESS=1,AI_ACTION_FINISHED=2,AI_ACTION_FAILED=3,AI_ACTION_IN_PROGRESS=4 };
static int g_n = 0;
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase() { }
protected:
    virtual void Update() = 0;
    EAIActionReturn AIAction_DoNothing() { g_n++; return g_n < 3 ? AI_ACTION_IN_PROGRESS : AI_ACTION_FINISHED; }
};
class CAIStateGroup_StandStill : public CAIStateGroupBase { public:
    virtual void Update();
};
void CAIStateGroup_StandStill::Update()
{
    while (AIAction_DoNothing() == AI_ACTION_IN_PROGRESS)
        ;
}
int main()
{
    CAIStateGroup_StandStill o; o.Update();
    if (g_n != 3) { std::printf("0091bcf0_TEST FAIL %d\n", g_n); return 1; }
    std::printf("0091bcf0_TEST PASS\n"); return 0;
}
