// Behaviour test for CTCSpecialAbilities::ToggleUsingAbility (0x0072AC90): only ability 4 with an active physical-shield
// move (interface 0x93) cancels the move and returns true.
#include <cstdio>

class CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* it = first;
        while (it != last && it->type < *type) ++it;
        return it;
    }
};

class CThing { public:
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    template <class T>
    bool GetInterface(long type, T** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
    void SetBit(long type) { m_presentBits[type >> 5] |= 1UL << (type & 31); }
};

static int g_cancels = 0;

class CTCPhysicalShieldSpell { public:
    bool m_active;
    bool IsMoveActive() { return m_active; }
    void CancelMove() { ++g_cancels; m_active = false; }
};

class CTCSpecialAbilities { public:
    CThing* ParentThing;
    bool ToggleUsingAbility(long ability);
};

bool CTCSpecialAbilities::ToggleUsingAbility(long ability)
{
    if (ability == 4) {
        CTCPhysicalShieldSpell* shield;
        if (ParentThing->GetInterface(0x93, &shield)) {
            if (shield->IsMoveActive()) {
                shield->CancelMove();
                return true;
            }
        }
    }
    return false;
}

int main()
{
    CTCPhysicalShieldSpell shield; shield.m_active = true;
    CInterfaceMap::SEntry entries[3] = { { 0x10, 0 }, { 0x93, (CTCBase*)&shield }, { 0 , 0 } };
    CThing t;
    for (int i = 0; i < 9; ++i) t.m_presentBits[i] = 0;
    t.m_interfaces.first = entries; t.m_interfaces.last = entries + 2;
    CTCSpecialAbilities s; s.ParentThing = &t;
    bool r1 = s.ToggleUsingAbility(4);                      // no interface bit
    t.SetBit(0x93);
    bool r2 = s.ToggleUsingAbility(3);                      // wrong ability
    bool r3 = s.ToggleUsingAbility(4);                      // cancels
    bool r4 = s.ToggleUsingAbility(4);                      // no longer active
    if (r1 || r2 || !r3 || r4 || g_cancels != 1) { std::printf("0072ac90_TEST FAIL\n"); return 1; }
    std::printf("0072ac90_TEST PASS\n");
    return 0;
}