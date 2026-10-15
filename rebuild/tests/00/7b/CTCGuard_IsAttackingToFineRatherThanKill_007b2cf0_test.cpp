#include <cstdio>

// Model of CTCGuard::IsAttackingToFineRatherThanKill @ 0x007B2CF0: look up the village-member interface (0x23) in the
// owner's interface map (presence bit + lower bound), then true only for an existing village with a positive fine.
struct CTCVillage { long Fine; long GetFineAmountForCurrentCrimes() { return Fine; } };
struct CTCVillageMember { CTCVillage* Village; CTCVillage* GetPVillage() const { return Village; } };
struct SEntry { long type; void* component; };
struct CThing {
    unsigned long m_presentBits[9];
    SEntry* first; SEntry* last;
    SEntry* LowerBound(long type) { SEntry* it = first; while (it != last && it->type < type) ++it; return it; }
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            SEntry* it = LowerBound(interfaceType);
            if (it == last || interfaceType < it->type) it = last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CTCGuard {
    CThing* ParentThing;
    bool IsAttackingToFineRatherThanKill() const
    {
        bool result = false;
        CTCVillageMember* member = 0;                // retail leaves it uninitialised; the test always sets the bit
        ParentThing->GetInterface(&member, 0x23);
        CTCVillage* village = member->GetPVillage();
        if (village != 0 && village->GetFineAmountForCurrentCrimes() > 0)
            result = true;
        return result;
    }
};

int main()
{
    CTCVillage village; village.Fine = 25;
    CTCVillageMember member; member.Village = &village;
    SEntry entries[3] = { { 0x10, 0 }, { 0x23, &member }, { 0x40, 0 } };
    CThing thing; for (int i = 0; i < 9; ++i) thing.m_presentBits[i] = 0;
    thing.m_presentBits[0x23 >> 5] |= 1UL << (0x23 & 31);
    thing.first = entries; thing.last = entries + 3;
    CTCGuard guard; guard.ParentThing = &thing;
    if (!guard.IsAttackingToFineRatherThanKill()) { std::printf("007b2cf0_TEST FAIL\n"); return 1; }
    village.Fine = 0;
    if (guard.IsAttackingToFineRatherThanKill()) { std::printf("007b2cf0_TEST FAIL\n"); return 1; }
    member.Village = 0;
    if (guard.IsAttackingToFineRatherThanKill()) { std::printf("007b2cf0_TEST FAIL\n"); return 1; }
    std::printf("007b2cf0_TEST PASS\n");
    return 0;
}