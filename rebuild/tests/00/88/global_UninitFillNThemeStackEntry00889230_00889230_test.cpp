// Behaviour test for 0x00889230 UninitFillN_ThemeStackEntry_00889230: copy-construct n theme priority-stack entries.
#include <cstdio>
#include <new>

struct __false_type { __false_type() { } };
static int g_vectorCopies = 0;
struct CThemeWeight { int ThemeID; float Weight; bool LockWeight; };
struct CThemeWeightVector {
    CThemeWeight* _M_start; CThemeWeight* _M_finish; CThemeWeight* _M_end_of_storage;
    CThemeWeightVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    CThemeWeightVector(const CThemeWeightVector& o) : _M_start(o._M_start), _M_finish(o._M_finish), _M_end_of_storage(o._M_end_of_storage) { ++g_vectorCopies; }
};
struct CEnvironmentThemePriorityStackEntry {
    float              BaseWeight;
    bool               BaseWeightLocked;
    CThemeWeightVector Themes;
    int                ThemeTransitionTargetID;
    float              ThemeTransitionInitialWeight;
    bool               ThemeTransitionActive;
    double             ThemeTransitionStartTime;
    float              ThemeTransitionDuration;
};

static CEnvironmentThemePriorityStackEntry* UninitFillN(CEnvironmentThemePriorityStackEntry* first, unsigned int n, const CEnvironmentThemePriorityStackEntry& x, const __false_type&)
{
    CEnvironmentThemePriorityStackEntry* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) CEnvironmentThemePriorityStackEntry(x);
    return cur;
}

int main()
{
    static CThemeWeight weights[2];
    CEnvironmentThemePriorityStackEntry x;
    x.BaseWeight = 0.5f; x.BaseWeightLocked = true; x.Themes._M_start = &weights[0]; x.Themes._M_finish = &weights[2];
    x.ThemeTransitionTargetID = 17; x.ThemeTransitionInitialWeight = 0.25f; x.ThemeTransitionActive = true;
    x.ThemeTransitionStartTime = 1234.5; x.ThemeTransitionDuration = 2.0f;
    static double storage[4 * sizeof(CEnvironmentThemePriorityStackEntry) / sizeof(double) + 1];
    CEnvironmentThemePriorityStackEntry* dst = (CEnvironmentThemePriorityStackEntry*)storage;
    __false_type tag;
    CEnvironmentThemePriorityStackEntry* r = UninitFillN(dst, 4, x, tag);
    bool ok = r == dst + 4 && g_vectorCopies == 4;
    for (int k = 0; k < 4; ++k)
        if (dst[k].BaseWeight != 0.5f || !dst[k].BaseWeightLocked || dst[k].Themes._M_start != &weights[0] || dst[k].Themes._M_finish != &weights[2]
            || dst[k].ThemeTransitionTargetID != 17 || dst[k].ThemeTransitionInitialWeight != 0.25f || !dst[k].ThemeTransitionActive
            || dst[k].ThemeTransitionStartTime != 1234.5 || dst[k].ThemeTransitionDuration != 2.0f) ok = false;
    if (UninitFillN(dst, 0, x, tag) != dst || g_vectorCopies != 4) ok = false;
    if (!ok) { std::printf("00889230_TEST FAIL\n"); return 1; }
    std::printf("00889230_TEST PASS\n");
    return 0;
}