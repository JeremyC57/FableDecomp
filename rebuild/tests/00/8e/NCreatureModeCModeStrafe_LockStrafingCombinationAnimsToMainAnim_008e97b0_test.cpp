// Behaviour test for 0x008E97B0 NCreatureMode::CModeStrafe::LockStrafingCombinationAnimsToMainAnim: mock interfaces log every call; the body is the source's, verbatim.
#include <cstdio>
#include <cstring>

static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static int Check(const char* expect) { int ok = std::strcmp(g_log, expect) == 0; if (!ok) std::printf("got  %s\nwant %s\n", g_log, expect); g_log[0] = 0; return ok; }

class CAnimSet { public: unsigned char OwnedByMainAnim : 1; };
class CAnimateThing {
public:
    CAnimSet Sets[0x30]; bool Present[0x30];
    CAnimSet* FindCombinationAnim(long type) { return Present[type] ? &Sets[type] : 0; }
};
class CTCAnimationComplex { public: CAnimateThing Anim; CAnimateThing& GetAnimation() { return Anim; } };
class CThing {
public:
    unsigned char m_dead : 1; CTCAnimationComplex* Complex;
    bool GetInterface(CTCAnimationComplex** out, long type) { *out = Complex; return type == 0x5a && Complex != 0; }
};
class CModeStrafe { public: CThing* ParentThing; void LockStrafingCombinationAnimsToMainAnim(); };
void CModeStrafe::LockStrafingCombinationAnimsToMainAnim()
{
    CTCAnimationComplex* animation;
    if (ParentThing && !ParentThing->m_dead && ParentThing->GetInterface(&animation, 0x5a)) {
        CAnimateThing& animator = animation->GetAnimation();
        CAnimSet* set = animator.FindCombinationAnim(0x23);
        if (set)
            set->OwnedByMainAnim = true;
        set = animator.FindCombinationAnim(0x24);
        if (set)
            set->OwnedByMainAnim = true;
        set = animator.FindCombinationAnim(0x25);
        if (set)
            set->OwnedByMainAnim = true;
        set = animator.FindCombinationAnim(0x1f);
        if (set)
            set->OwnedByMainAnim = true;
    }
}
int main()
{
    CTCAnimationComplex ac; CThing t; t.m_dead = 0; t.Complex = &ac;
    for (int i = 0; i < 0x30; ++i) { ac.Anim.Sets[i].OwnedByMainAnim = 0; ac.Anim.Present[i] = true; }
    ac.Anim.Present[0x24] = false;
    CModeStrafe m; m.ParentThing = &t;
    int ok = 1;
    t.m_dead = 1; m.LockStrafingCombinationAnimsToMainAnim(); ok &= ac.Anim.Sets[0x23].OwnedByMainAnim == 0;
    t.m_dead = 0; m.LockStrafingCombinationAnimsToMainAnim();
    ok &= ac.Anim.Sets[0x23].OwnedByMainAnim == 1 && ac.Anim.Sets[0x24].OwnedByMainAnim == 0;
    ok &= ac.Anim.Sets[0x25].OwnedByMainAnim == 1 && ac.Anim.Sets[0x1f].OwnedByMainAnim == 1 && ac.Anim.Sets[0x20].OwnedByMainAnim == 0;
    m.ParentThing = 0; m.LockStrafingCombinationAnimsToMainAnim();
    if (ok) std::printf("008e97b0_TEST PASS\n");
    return ok ? 0 : 1;
}