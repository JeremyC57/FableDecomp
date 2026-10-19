// behaviour copy of NCreatureMode::CModeCarryingStandBase::FrameUpdate: drop the carrying combination anim (type 0x18) while the
// main anim is of type 1, else add the carrying anim when the mode has one and it is not already playing
#include <cstdio>
static int g_base = 0, g_removed = 0, g_added = 0, g_addIndex = -2;
struct CAnimInit { long AnimType, Arg24, AnimIndex, Mode; bool Flag05; unsigned long Flags; long Percent; float Blend[2];
    CAnimInit(long t, long a, long i, long m, bool f, unsigned long fl, long p) : AnimType(t), Arg24(a), AnimIndex(i), Mode(m), Flag05(f), Flags(fl), Percent(p) { Blend[0] = Blend[1] = 0.0f; } };
struct CAnimateThing {
    unsigned char CurrentAnimType; bool hasCombo;
    void* FindCombinationAnim(long type) { return (type == 0x18 && hasCombo) ? this : 0; }
    void AddCombinationAnim(const CAnimInit& init, long, long, bool, bool) { ++g_added; g_addIndex = init.AnimIndex; if (init.AnimType != 0x18 || !init.Flag05 || init.Percent != 100) g_addIndex = -3; }
    void RemoveCombinationAnimsOfType(long type) { if (type == 0x18) ++g_removed; }
};
struct CTCAnimationComplex { CAnimateThing anim; CAnimateThing& GetAnimation() { return anim; } };
struct CThing { CTCAnimationComplex* tc; bool GetInterface(CTCAnimationComplex** out, long type) { if (type == 0x5a) { *out = tc; return true; } return false; } };
struct CModeCarryingStandBase {
    CThing* ParentThing; long carryIndex;
    void BaseFrameUpdate() { ++g_base; }
    long GetCarryingAnimIndex() { return carryIndex; }
    void FrameUpdate()
    {
        BaseFrameUpdate();
        CTCAnimationComplex* animation;
        ParentThing->GetInterface(&animation, 0x5a);
        CAnimateThing& animate = animation->GetAnimation();
        if (animate.CurrentAnimType != 1) {
            long index = GetCarryingAnimIndex();
            if (index != -1 && animate.FindCombinationAnim(0x18) == 0) {
                CAnimInit init(0x18, 0, index, 1, true, 0, 100);
                animate.AddCombinationAnim(init, 0, 0, false, false);
            }
        }
        else
            animate.RemoveCombinationAnimsOfType(0x18);
    }
};
int main()
{
    CTCAnimationComplex tc; tc.anim.CurrentAnimType = 0; tc.anim.hasCombo = false;
    CThing t; t.tc = &tc; CModeCarryingStandBase m; m.ParentThing = &t; m.carryIndex = 7;
    m.FrameUpdate();
    if (g_base != 1 || g_added != 1 || g_addIndex != 7 || g_removed != 0) { std::printf("008d88a0_TEST FAIL\n"); return 1; }
    tc.anim.hasCombo = true; m.FrameUpdate(); m.carryIndex = -1; tc.anim.hasCombo = false; m.FrameUpdate();
    if (g_added != 1) { std::printf("008d88a0_TEST FAIL\n"); return 1; }
    tc.anim.CurrentAnimType = 1; m.FrameUpdate();
    if (g_removed != 1 || g_base != 4) { std::printf("008d88a0_TEST FAIL\n"); return 1; }
    std::printf("008d88a0_TEST PASS\n");
    return 0;
}