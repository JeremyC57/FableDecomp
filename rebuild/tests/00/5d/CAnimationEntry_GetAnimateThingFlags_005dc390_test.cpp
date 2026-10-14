// Behaviour test for CAnimationEntry::GetAnimateThingFlags @ 0x005dc390: component bit 3 -> 1, bit 0 -> 2, bit 4 -> 4.
#include <cstdio>
struct CAnimComponentFlags { unsigned char _pad[8]; long Flags; };
static CAnimComponentFlags g_component;
struct CAnimationEntry {
    long BankIndex; long AnimName; long GroupName; void* Components;
    void GetFlagsComponent(CAnimComponentFlags** ppComponent) const { *ppComponent = &g_component; }
    unsigned long GetAnimateThingFlags() const
    {
        CAnimComponentFlags* pComponent; GetFlagsComponent(&pComponent);
        long flags = pComponent->Flags; unsigned long result = 0;
        if (flags & 8) result |= 1; if (flags & 1) result |= 2; if (flags & 0x10) result |= 4;
        return result;
    }
};
int main()
{
    CAnimationEntry e; bool ok = true;
    g_component.Flags = 0;    if (e.GetAnimateThingFlags() != 0) ok = false;
    g_component.Flags = 8;    if (e.GetAnimateThingFlags() != 1) ok = false;
    g_component.Flags = 1;    if (e.GetAnimateThingFlags() != 2) ok = false;
    g_component.Flags = 0x10; if (e.GetAnimateThingFlags() != 4) ok = false;
    g_component.Flags = 0x19; if (e.GetAnimateThingFlags() != 7) ok = false;
    g_component.Flags = 0xE6; if (e.GetAnimateThingFlags() != 0) ok = false;
    if (!ok) { std::printf("005dc390_TEST FAIL\n"); return 1; }
    std::printf("005dc390_TEST PASS\n");
    return 0;
}