#include <cstdio>
static int g_calls; static int g_terminated;
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase() {}
    long AIAction_DoNothing() { return ++g_calls < 4 ? 4 : 0; }
    void Terminate() { ++g_terminated; }
};
struct CAIStateGroup_CreatureIdleUnderground : CAIStateGroupBase {
    virtual void Update() { while (AIAction_DoNothing() == 4) { } Terminate(); }
};
int main()
{
    CAIStateGroup_CreatureIdleUnderground g; g.Update();
    if (g_calls != 4 || g_terminated != 1) { std::printf("00920770_TEST FAIL\n"); return 1; }
    std::printf("00920770_TEST PASS\n"); return 0;
}