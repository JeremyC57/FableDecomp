#include <cstdio>
#include <cstring>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#define __cdecl
#endif
static int g_extCopies = 0;   // out-of-line copy constructors the retail body calls
static int g_links = 0;       // CBaseIntelligentPointer::SetPItem calls
namespace N006b6670 {
struct CThemeWeightArray {                                 // retail 12-byte STLport-vector body (void)
    void* _M_start;
    void* _M_finish;
    void* _M_end_of_storage;
    CThemeWeightArray() { std::memset(this, 0, sizeof(CThemeWeightArray)); }
    CThemeWeightArray(const CThemeWeightArray& other) { std::memcpy(this, &other, sizeof(CThemeWeightArray)); ++g_extCopies; }
};
struct CEnvironmentThemePriorityStackEntry {     // NEnvironmentInternal::CEnvironmentThemePriorityStackEntry (retail sizeof 0x30)
    float             BaseWeight;                // +0x00
    bool              BaseWeightLocked;          // +0x04
    CThemeWeightArray Themes;                    // +0x08 CArray<CThemeWeight>
    long              ThemeTransitionTargetID;   // +0x14
    float             ThemeTransitionInitialWeight; // +0x18
    bool              ThemeTransitionActive;     // +0x1C
    double            ThemeTransitionStartTime;  // +0x20
    float             ThemeTransitionDuration;   // +0x28
    CEnvironmentThemePriorityStackEntry() { }
    __forceinline CEnvironmentThemePriorityStackEntry(const CEnvironmentThemePriorityStackEntry& o)   // the implicit memberwise copy, spelled out so VC7.1 inlines it
        : BaseWeight(o.BaseWeight), BaseWeightLocked(o.BaseWeightLocked), Themes(o.Themes),
          ThemeTransitionTargetID(o.ThemeTransitionTargetID), ThemeTransitionInitialWeight(o.ThemeTransitionInitialWeight),
          ThemeTransitionActive(o.ThemeTransitionActive), ThemeTransitionStartTime(o.ThemeTransitionStartTime),
          ThemeTransitionDuration(o.ThemeTransitionDuration) { }
};

void __fastcall _Construct(CEnvironmentThemePriorityStackEntry* p, const CEnvironmentThemePriorityStackEntry& value)
{
    if (p != 0)
        new (p) CEnvironmentThemePriorityStackEntry(value);
}
}

static double g_storage[64];
int main()
{
    using namespace N006b6670;
    CEnvironmentThemePriorityStackEntry src;
    src.BaseWeight = 0.5f;
    src.BaseWeightLocked = true;
    src.Themes._M_start = reinterpret_cast<void*>(16);
    src.ThemeTransitionTargetID = 9;
    src.ThemeTransitionInitialWeight = 0.25f;
    src.ThemeTransitionActive = true;
    src.ThemeTransitionStartTime = 123.5;
    src.ThemeTransitionDuration = 4.0f;
    CEnvironmentThemePriorityStackEntry* slot = reinterpret_cast<CEnvironmentThemePriorityStackEntry*>(g_storage);
    _Construct(slot, src);
    bool ok = g_extCopies == 1 && g_links == 0;
    if (!(slot->BaseWeight == src.BaseWeight)) ok = false;
    if (!(slot->BaseWeightLocked == src.BaseWeightLocked)) ok = false;
    if (!(slot->Themes._M_start == src.Themes._M_start)) ok = false;
    if (!(slot->ThemeTransitionTargetID == src.ThemeTransitionTargetID)) ok = false;
    if (!(slot->ThemeTransitionInitialWeight == src.ThemeTransitionInitialWeight)) ok = false;
    if (!(slot->ThemeTransitionActive == src.ThemeTransitionActive)) ok = false;
    if (!(slot->ThemeTransitionStartTime == src.ThemeTransitionStartTime)) ok = false;
    if (!(slot->ThemeTransitionDuration == src.ThemeTransitionDuration)) ok = false;
    _Construct(0, src);                                  // a null slot constructs nothing
    if (g_extCopies != 1 || g_links != 0) ok = false;
    if (!ok) { std::printf("006b6670_TEST FAIL\n"); return 1; }
    std::printf("006b6670_TEST PASS\n");
    return 0;
}