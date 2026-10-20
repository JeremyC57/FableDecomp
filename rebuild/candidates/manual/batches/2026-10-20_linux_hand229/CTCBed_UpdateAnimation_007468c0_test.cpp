// Behaviour test for CTCBed::UpdateAnimation (0x007468C0).
#include <cstdio>

static float g_epsilon = 1e-4f;
struct CAnimInit {
    long animType, arg24, animIndex, mode; bool flag05; unsigned long flags; long percent;
    CAnimInit(long t, long a, long i, long m, bool f, unsigned long fl, long p)
        : animType(t), arg24(a), animIndex(i), mode(m), flag05(f), flags(fl), percent(p) {}
};
static int g_setMain = 0; static long g_lastAnim = 0;
struct CAnimateThing {
    int m_pCurrent, m_pEnd;
    float Progress;
    unsigned char m_stateFlags;
    void SetMainAnim(const CAnimInit& init, long, long) { ++g_setMain; g_lastAnim = init.animIndex; }
    bool IsFinished() const { return m_pCurrent == m_pEnd || (m_stateFlags & 2); }
};
struct CTCAnimationComplex { CAnimateThing anim; CAnimateThing& GetAnimation() { return anim; } };
class CTCBed {
public:
    CTCAnimationComplex* complex;
    bool Empty, GettingIn, GettingOut, Animating;
    long BedSleepAnim;
    CTCAnimationComplex* GetAnimationComplex() const { return complex; }
    void UpdateAnimation()
    {
        CAnimateThing& animate = GetAnimationComplex()->GetAnimation();
        if (Animating) {
            float progress = animate.Progress;
            if (GettingIn) {
                if (progress > 1.0f - g_epsilon) {
                    GettingIn = false;
                    CAnimInit init(6, 0, BedSleepAnim, 1, true, 0, 100);
                    animate.SetMainAnim(init, 1, 0);
                }
            } else if (GettingOut) {
                if (progress > 1.0f - g_epsilon) {
                    GettingOut = false;
                    Empty = true;
                }
            }
            if (animate.IsFinished())
                Animating = false;
        }
    }
};

int main()
{
    int ok = 1;
    CTCAnimationComplex c; c.anim.m_pCurrent = 1; c.anim.m_pEnd = 2; c.anim.m_stateFlags = 0; c.anim.Progress = 0.5f;
    CTCBed bed; bed.complex = &c; bed.Empty = false; bed.GettingIn = true; bed.GettingOut = false; bed.Animating = true; bed.BedSleepAnim = 77;
    bed.UpdateAnimation();
    if (!bed.GettingIn || g_setMain != 0 || !bed.Animating) ok = 0;            // halfway: nothing yet
    c.anim.Progress = 1.0f;
    bed.UpdateAnimation();
    if (bed.GettingIn || g_setMain != 1 || g_lastAnim != 77 || !bed.Animating) ok = 0;   // switch to the sleep loop
    bed.GettingOut = true; c.anim.Progress = 0.99995f;
    bed.UpdateAnimation();
    if (bed.GettingOut || !bed.Empty) ok = 0;                                    // got out: bed empty
    c.anim.m_stateFlags = 2;
    bed.UpdateAnimation();
    if (bed.Animating) ok = 0;                                                   // finished
    if (!ok) { std::printf("007468c0_TEST FAIL\n"); return 1; }
    std::printf("007468c0_TEST PASS\n");
    return 0;
}
