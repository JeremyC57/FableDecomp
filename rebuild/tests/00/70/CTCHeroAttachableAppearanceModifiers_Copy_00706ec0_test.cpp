#include <cstdio>

// Model of CTCHeroAttachableAppearanceModifiers::Copy(const CTCBase&) @ 0x00706EC0: the other TC's modifier lists are
// assigned to ours, then Changed is set and the frame stamped (no CTCBase::Copy chain).
static long g_frame = 0; static int g_baseCopies = 0;
struct CWorld { static long GetFrame() { return g_frame; } };
static CWorld g_world;
struct CModifierVectorVector {
    long values[4]; long count;
    CModifierVectorVector& operator=(const CModifierVectorVector& o) { count = o.count; for (long i = 0; i < 4; ++i) values[i] = o.values[i]; return *this; }
};
struct CTCBase { CWorld& GetWorld() const { return g_world; } virtual void Copy(const CTCBase&) { ++g_baseCopies; } virtual ~CTCBase() { } };
struct CTCHeroAttachableAppearanceModifiers : CTCBase {
    unsigned long FrameLastChanged; bool Changed : 1; bool RecreatePrimitives : 1;
    CModifierVectorVector AppearanceModifiers;
    void SetAsChanged() { Changed = true; FrameLastChanged = GetWorld().GetFrame(); }
    virtual void Copy(const CTCBase& other)
    {
        const CTCHeroAttachableAppearanceModifiers& source = static_cast<const CTCHeroAttachableAppearanceModifiers&>(other);
        AppearanceModifiers = source.AppearanceModifiers;
        SetAsChanged();
    }
};

int main()
{
    CTCHeroAttachableAppearanceModifiers a, b;
    a.AppearanceModifiers.count = 3; for (long i = 0; i < 4; ++i) a.AppearanceModifiers.values[i] = 10 + i;
    b.AppearanceModifiers.count = 0; for (long i = 0; i < 4; ++i) b.AppearanceModifiers.values[i] = 0;
    b.Changed = false; b.RecreatePrimitives = true; b.FrameLastChanged = 0; g_frame = 77;
    CTCBase& base = b; base.Copy(a);
    if (b.AppearanceModifiers.count != 3 || b.AppearanceModifiers.values[2] != 12 || !b.Changed || !b.RecreatePrimitives ||
        b.FrameLastChanged != 77 || g_baseCopies != 0) { std::printf("00706ec0_TEST FAIL\n"); return 1; }
    std::printf("00706ec0_TEST PASS\n");
    return 0;
}