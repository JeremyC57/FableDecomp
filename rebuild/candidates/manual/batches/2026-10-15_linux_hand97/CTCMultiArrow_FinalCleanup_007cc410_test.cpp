// Behaviour test for CTCMultiArrow::FinalCleanup @ 0x007CC410: the type-0x13 component's weapon is passed to RemoveEffectOnWeapon.
#include <cstdio>
struct CThing;
struct CWeaponHolder { CThing* m_weapon; CThing* GetWeapon() const { return m_weapon; } };
struct SEntry { long type; void* component; };
struct CThing {
    unsigned long m_presentBits[9];
    SEntry* first; SEntry* last;
    SEntry* LowerBound(long type) { SEntry* p = first; while (p != last && p->type < type) ++p; return p; }
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
struct CTCMultiArrow {
    CThing* ParentThing;
    CThing* m_removedFrom; int m_calls;
    void RemoveEffectOnWeapon(CThing* weapon) { m_removedFrom = weapon; ++m_calls; }
    void FinalCleanup()
    {
        CWeaponHolder* holder = 0;                    // retail leaves it uninitialised on the absent path
        ParentThing->GetInterface(&holder, 0x13);
        RemoveEffectOnWeapon(holder->GetWeapon());
    }
};
int main()
{
    CThing bow; bow.first = bow.last = 0;
    for (int i = 0; i < 9; ++i) bow.m_presentBits[i] = 0;
    CWeaponHolder holder; holder.m_weapon = &bow;
    SEntry entries[3] = { { 5, 0 }, { 0x13, &holder }, { 0x40, 0 } };
    CThing owner; for (int j = 0; j < 9; ++j) owner.m_presentBits[j] = 0;
    owner.m_presentBits[0] = 1UL << 0x13; owner.first = entries; owner.last = entries + 3;
    CTCMultiArrow m; m.ParentThing = &owner; m.m_removedFrom = 0; m.m_calls = 0;
    m.FinalCleanup();
    if (m.m_calls != 1 || m.m_removedFrom != &bow) { std::printf("007cc410_TEST FAIL\n"); return 1; }
    std::printf("007cc410_TEST PASS\n");
    return 0;
}
