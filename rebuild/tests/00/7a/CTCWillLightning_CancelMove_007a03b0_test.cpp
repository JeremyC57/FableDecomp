#include <cstdio>

// Model: a thing with an interface bitset + sorted {type, component} map, the special-abilities component and a lightning
// power that counts its deactivations.
class CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* it = first;
        while (it != last && it->type < *type)
            ++it;
        return it;
    }
};

class CThing {
public:
    unsigned int  m_presentBits[9];
    CInterfaceMap m_interfaces;
    template <class T> bool GetInterface(T** out, long wanted)
    {
        if (m_presentBits[wanted >> 5] & (1U << (wanted & 31))) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || wanted < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};

class CTCSpecialAbilities { public:
    bool AbilityMutexLock;
    long CurrentAbility;
};

class CTCWillLightning {
public:
    CThing* ParentThing;
    int     Deactivations;
    void Deactivate() { ++Deactivations; }
    void CancelMove();
};

void CTCWillLightning::CancelMove()
{
    CTCSpecialAbilities* abilities;
    if (!ParentThing->GetInterface(&abilities, 0xa7) || abilities->CurrentAbility == 0xb)
        Deactivate();
}

int main()
{
    CTCSpecialAbilities abilities;
    abilities.AbilityMutexLock = false;
    abilities.CurrentAbility = 0xb;
    CInterfaceMap::SEntry entries[2];
    entries[0].type = 0x10; entries[0].component = 0;
    entries[1].type = 0xa7; entries[1].component = (CTCBase*)&abilities;

    CThing hero;
    for (int i = 0; i < 9; ++i)
        hero.m_presentBits[i] = 0;
    hero.m_interfaces.first = entries;
    hero.m_interfaces.last = entries + 2;

    CTCWillLightning power;
    power.ParentThing = &hero;
    power.Deactivations = 0;

    bool ok = true;
    power.CancelMove();                                  // no special-abilities component: deactivates
    ok = ok && power.Deactivations == 1;
    hero.m_presentBits[0xa7 >> 5] |= 1U << (0xa7 & 31);
    power.CancelMove();                                  // component busy with lightning (0xB): deactivates
    ok = ok && power.Deactivations == 2;
    abilities.CurrentAbility = 12;
    power.CancelMove();                                  // component busy with another ability: left alone
    ok = ok && power.Deactivations == 2;
    abilities.CurrentAbility = 0;
    power.CancelMove();
    ok = ok && power.Deactivations == 2;

    if (!ok) {
        std::printf("007a03b0_TEST FAIL\n");
        return 1;
    }
    std::printf("007a03b0_TEST PASS\n");
    return 0;
}