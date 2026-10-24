// Behaviour test for 0x006B6710 std::__uninitialized_copy(CEnvironmentThemePriorityStackEntry*, .., __false_type).
#include <cstdio>
#include <new>

static int g_arrayCopies = 0;

struct FalseType { FalseType() { } };

struct CThemeWeightArray {
    int* _M_start;
    int* _M_finish;
    int* _M_end_of_storage;
    CThemeWeightArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    CThemeWeightArray(const CThemeWeightArray& o) : _M_start(o._M_start), _M_finish(o._M_finish), _M_end_of_storage(o._M_end_of_storage) { ++g_arrayCopies; }
};

struct CEnvironmentThemePriorityStackEntry {
    float             BaseWeight;
    bool              BaseWeightLocked;
    CThemeWeightArray Themes;
    int               ThemeTransitionTargetID;
    float             ThemeTransitionInitialWeight;
    bool              ThemeTransitionActive;
    double            ThemeTransitionStartTime;
    float             ThemeTransitionDuration;
};

template <class T1, class T2> inline void Construct(T1* p, const T2& v) { new (p) T1(v); }

template <class InputIter, class ForwardIter>
ForwardIter UninitCopy(InputIter first, InputIter last, ForwardIter result, const FalseType&)
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        Construct(&*cur, *first);
    return cur;
}

static int weights[4];
static unsigned char raw[sizeof(CEnvironmentThemePriorityStackEntry) * 4];

int main()
{
    CEnvironmentThemePriorityStackEntry src[2];
    for (int i = 0; i < 2; ++i) {
        src[i].BaseWeight = 0.5f + i; src[i].BaseWeightLocked = i == 1;
        src[i].Themes._M_start = &weights[i]; src[i].ThemeTransitionTargetID = 10 + i;
        src[i].ThemeTransitionInitialWeight = 2.0f; src[i].ThemeTransitionActive = i == 0;
        src[i].ThemeTransitionStartTime = 100.25 + i; src[i].ThemeTransitionDuration = 3.0f;
    }
    CEnvironmentThemePriorityStackEntry* d = (CEnvironmentThemePriorityStackEntry*)raw;
    CEnvironmentThemePriorityStackEntry* end = UninitCopy(src, src + 2, d, FalseType());
    int ok = end == d + 2 && g_arrayCopies == 2;
    for (int i = 0; ok && i < 2; ++i)
        if (d[i].BaseWeight != src[i].BaseWeight || d[i].BaseWeightLocked != src[i].BaseWeightLocked ||
            d[i].Themes._M_start != &weights[i] || d[i].ThemeTransitionTargetID != 10 + i ||
            d[i].ThemeTransitionActive != (i == 0) || d[i].ThemeTransitionStartTime != 100.25 + i ||
            d[i].ThemeTransitionDuration != 3.0f) ok = 0;
    if (UninitCopy(src, src, d, FalseType()) != d || g_arrayCopies != 2) ok = 0;
    if (!ok) { std::printf("006b6710_TEST FAIL\n"); return 1; }
    std::printf("006b6710_TEST PASS\n");
    return 0;
}