// Behaviour test for CTCBob::SetForceVisible 0x00787740: forcing a bobbing thing visible sets ForceVisible and runs its (virtual)
// FrameUpdate at once, which sees the flag already set.
#include <cstdio>
struct CTCBase { virtual ~CTCBase() {} virtual void FrameUpdate() {} };
struct CTCBob : CTCBase {
    bool ForceVisible; int Updates; bool SawFlag;
    CTCBob() : ForceVisible(false), Updates(0), SawFlag(false) {}
    void FrameUpdate() { ++Updates; SawFlag = ForceVisible; }
    void SetForceVisible() { ForceVisible = true; FrameUpdate(); }
};
int main()
{
    CTCBob bob;
    bob.SetForceVisible();
    if (!bob.ForceVisible || bob.Updates != 1 || !bob.SawFlag) { std::printf("00787740_TEST FAIL\n"); return 1; }
    std::printf("00787740_TEST PASS\n");
    return 0;
}