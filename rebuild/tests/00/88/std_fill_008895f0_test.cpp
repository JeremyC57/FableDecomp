// behaviour test for 0x008895f0 (std::fill over CEnvironmentThemePriorityStackEntry)
#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#endif
static int g_stringCopies = 0, g_stringAssigns = 0;
static int g_arrayAssigns = 0;
namespace NEnvironmentInternal {
template <class T>
struct CArray {                                 // the engine's 12-byte vector
    T* _M_start;                                // +0x00
    T* _M_finish;                               // +0x04
    T* _M_end_of_storage;                       // +0x08
    CArray<T>& operator=(const CArray<T>& x) { _M_start = x._M_start; _M_finish = x._M_finish; _M_end_of_storage = x._M_end_of_storage; ++g_arrayAssigns; return *this; }
};
class CEnvironmentThemePriorityStackEntry { public:    // 48 bytes
    struct CThemeWeight {                       // 12 bytes
        long  ThemeID;                          // +0x00
        float Weight;                           // +0x04
        bool  LockWeight;                       // +0x08
    };
    float                BaseWeight;                     // +0x00
    bool                 BaseWeightLocked;               // +0x04
    CArray<CThemeWeight> Themes;                         // +0x08
    long                 ThemeTransitionTargetID;        // +0x14
    float                ThemeTransitionInitialWeight;   // +0x18
    bool                 ThemeTransitionActive;          // +0x1C
    double               ThemeTransitionStartTime;       // +0x20
    float                ThemeTransitionDuration;        // +0x28
};
}

namespace std {
template <class ForwardIter, class T>
void __fastcall fill(ForwardIter first, ForwardIter last, const T& val)
{
    for (; first != last; ++first)
        *first = val;
}
template void __fastcall fill(NEnvironmentInternal::CEnvironmentThemePriorityStackEntry*,
                              NEnvironmentInternal::CEnvironmentThemePriorityStackEntry*,
                              const NEnvironmentInternal::CEnvironmentThemePriorityStackEntry&);
}

int main()
{
    typedef NEnvironmentInternal::CEnvironmentThemePriorityStackEntry Entry;
    Entry v[3];
    Entry::CThemeWeight w[2];
    for (int i = 0; i < 3; ++i) {
        v[i].BaseWeight = 0.0f; v[i].BaseWeightLocked = false; v[i].Themes._M_start = 0; v[i].Themes._M_finish = 0; v[i].Themes._M_end_of_storage = 0;
        v[i].ThemeTransitionTargetID = -1; v[i].ThemeTransitionInitialWeight = 0.0f; v[i].ThemeTransitionActive = false;
        v[i].ThemeTransitionStartTime = 0.0; v[i].ThemeTransitionDuration = 0.0f;
    }
    Entry val;
    val.BaseWeight = 0.75f; val.BaseWeightLocked = true; val.Themes._M_start = w; val.Themes._M_finish = w + 1; val.Themes._M_end_of_storage = w + 2;
    val.ThemeTransitionTargetID = 17; val.ThemeTransitionInitialWeight = 0.25f; val.ThemeTransitionActive = true;
    val.ThemeTransitionStartTime = 123.5; val.ThemeTransitionDuration = 4.0f;
    std::fill(v, v + 2, val);
    int ok = v[0].BaseWeight == 0.75f && v[1].BaseWeightLocked && v[0].Themes._M_finish == w + 1 && v[1].Themes._M_end_of_storage == w + 2
          && v[1].ThemeTransitionTargetID == 17 && v[0].ThemeTransitionInitialWeight == 0.25f && v[1].ThemeTransitionActive
          && v[0].ThemeTransitionStartTime == 123.5 && v[1].ThemeTransitionDuration == 4.0f && g_arrayAssigns == 2
          && v[2].ThemeTransitionTargetID == -1 && v[2].Themes._M_start == 0 && !v[2].BaseWeightLocked;
#ifdef _MSC_VER
    ok = ok && sizeof(Entry) == 0x30;   // retail layout (32-bit)
#endif
    if (!ok) { printf("008895f0_TEST FAIL\n"); return 1; }
    printf("008895f0_TEST PASS\n");
    return 0;
}