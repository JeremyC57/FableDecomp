#include <cstdio>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#endif
// Model of _STL::__uninitialized_copy over CEnvironmentThemePriorityStackEntry @ 0x00889300: copy-construct each 48-byte
// entry (its theme-weight vector through its own deep copy constructor) into raw storage, return the new end.
struct __false_type { __false_type() { } };
struct CThemeWeight {
    int   ThemeID;
    float Weight;
    bool  LockWeight;
};
static int g_vectorCopies = 0;
struct CThemeWeightVector {
    CThemeWeight* _M_start;
    CThemeWeight* _M_finish;
    CThemeWeight* _M_end_of_storage;
    CThemeWeightVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    CThemeWeightVector(const CThemeWeightVector& other)          // deep copy (0x006B58F0)
    {
        ++g_vectorCopies;
        int n = (int)(other._M_finish - other._M_start);
        _M_start = n ? (CThemeWeight*)malloc(n * sizeof(CThemeWeight)) : 0;
        for (int i = 0; i < n; ++i)
            _M_start[i] = other._M_start[i];
        _M_finish = _M_start + n;
        _M_end_of_storage = _M_finish;
    }
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
template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall UninitCopy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
int main()
{
    bool ok = true;
    CThemeWeight weights[2] = { { 1, 0.5f, false }, { 2, 0.25f, true } };
    CEnvironmentThemePriorityStackEntry src[3];
    for (int i = 0; i < 3; ++i) {
        src[i].BaseWeight = 1.0f + i;
        src[i].BaseWeightLocked = (i & 1) != 0;
        src[i].Themes._M_start = weights;
        src[i].Themes._M_finish = weights + (i == 1 ? 0 : 2);
        src[i].Themes._M_end_of_storage = src[i].Themes._M_finish;
        src[i].ThemeTransitionTargetID = 10 + i;
        src[i].ThemeTransitionInitialWeight = 0.1f * i;
        src[i].ThemeTransitionActive = i == 2;
        src[i].ThemeTransitionStartTime = 100.0 + i;
        src[i].ThemeTransitionDuration = 2.0f;
    }
    CEnvironmentThemePriorityStackEntry* raw =
        (CEnvironmentThemePriorityStackEntry*)malloc(3 * sizeof(CEnvironmentThemePriorityStackEntry));
    CEnvironmentThemePriorityStackEntry* end = UninitCopy(src, src + 3, raw, __false_type());
    if (end != raw + 3 || g_vectorCopies != 3) ok = false;
    for (int i = 0; i < 3; ++i) {
        if (raw[i].BaseWeight != src[i].BaseWeight || raw[i].BaseWeightLocked != src[i].BaseWeightLocked) ok = false;
        if (raw[i].ThemeTransitionTargetID != 10 + i || raw[i].ThemeTransitionStartTime != 100.0 + i) ok = false;
        if (raw[i].ThemeTransitionActive != (i == 2) || raw[i].ThemeTransitionDuration != 2.0f) ok = false;
        int n = (int)(raw[i].Themes._M_finish - raw[i].Themes._M_start);
        if (n != (i == 1 ? 0 : 2)) ok = false;
        if (n && (raw[i].Themes._M_start == weights || raw[i].Themes._M_start[1].ThemeID != 2)) ok = false;
    }
    if (UninitCopy(src, src, raw, __false_type()) != raw) ok = false;
    if (!ok) { std::printf("00889300_TEST FAIL\n"); return 1; }
    std::printf("00889300_TEST PASS\n");
    return 0;
}