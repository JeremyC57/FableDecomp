#include <cstdio>

// Model of CTCHeroExperience::OnComboMultiplierDecrease @ 0x006D7E70: fetch the combat TC (interface 0x30) from the
// owner's interface map and clear its melee-flourish flag.
struct CTCCombat { bool MeleeFlourishValid; void SetMeleeFlourishValid(bool v) { MeleeFlourishValid = v; } };
struct SEntry { long type; void* component; };
struct CThing {
    unsigned long m_presentBits[9]; SEntry* first; SEntry* last;
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            SEntry* it = first; while (it != last && it->type < interfaceType) ++it;
            if (it == last || interfaceType < it->type) it = last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CTCHeroExperience {
    CThing* ParentThing;
    void OnComboMultiplierDecrease()
    {
        CTCCombat* combat = 0;                       // retail leaves it uninitialised; the test always sets the bit
        ParentThing->GetInterface(&combat, 0x30);
        combat->SetMeleeFlourishValid(false);
    }
};

int main()
{
    CTCCombat combat; combat.MeleeFlourishValid = true;
    SEntry entries[2] = { { 0x23, 0 }, { 0x30, &combat } };
    CThing t; for (int i = 0; i < 9; ++i) t.m_presentBits[i] = 0; t.m_presentBits[1] = 1UL << 16; t.first = entries; t.last = entries + 2;
    CTCHeroExperience e; e.ParentThing = &t;
    e.OnComboMultiplierDecrease();
    if (combat.MeleeFlourishValid) { std::printf("006d7e70_TEST FAIL\n"); return 1; }
    std::printf("006d7e70_TEST PASS\n");
    return 0;
}
