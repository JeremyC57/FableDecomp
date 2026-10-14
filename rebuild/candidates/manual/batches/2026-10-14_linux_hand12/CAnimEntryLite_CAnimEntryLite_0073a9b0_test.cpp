// Behaviour test for CAnimEntryLite::CAnimEntryLite(const CAnimationEntry&) @ 0x0073a9b0: bank index copied, times and flags fetched.
#include <cstdio>
#include <cstddef>
struct CAnimComponentFlags { unsigned char _pad[8]; long Flags; };
static CAnimComponentFlags g_component;
struct CAnimationEntry {
    long BankIndex; long AnimName; long GroupName; void* Components;
    long GetAnimationTransitionInTime() const { return 300; }
    long GetAnimationDelayTime() const { return 40; }
    void GetFlagsComponent(CAnimComponentFlags** ppComponent) const { *ppComponent = &g_component; }
    long GetAnimationFlags() const { CAnimComponentFlags* p; GetFlagsComponent(&p); return p->Flags; }
};
struct CAnimEntryLite {
    long BankIndex; long DelayTime; long TransitionTime; long AnimFlags;
    CAnimEntryLite(const CAnimationEntry& entry)
    { BankIndex = entry.BankIndex; TransitionTime = entry.GetAnimationTransitionInTime(); DelayTime = entry.GetAnimationDelayTime(); AnimFlags = entry.GetAnimationFlags(); }
};
int main()
{
    g_component.Flags = 0x55; CAnimationEntry e; e.BankIndex = 77;
    CAnimEntryLite lite(e);
    bool ok = lite.BankIndex == 77 && lite.DelayTime == 40 && lite.TransitionTime == 300 && lite.AnimFlags == 0x55
        && offsetof(CAnimEntryLite, TransitionTime) == 8 && sizeof(lite) == 0x10;
    if (!ok) { std::printf("0073a9b0_TEST FAIL\n"); return 1; }
    std::printf("0073a9b0_TEST PASS\n");
    return 0;
}
