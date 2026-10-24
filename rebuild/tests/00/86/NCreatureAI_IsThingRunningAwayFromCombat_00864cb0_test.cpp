// Behaviour test for 0x00864CB0: IsThingRunningAwayFromCombat is true only with a stealth component (type 0x36)
// whose RunningAwayFromCombatTimer is >= 0.
#include <cstdio>

class CTCBase;

struct CInterfaceMap
{
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long& type) const
    {
        SEntry* it = first;
        while (it != last && it->type < type) ++it;
        return it;
    }
};

class CTCStealth
{
public:
    unsigned char _pad0[0x30];
    long          RunningAwayFromCombatTimer;
};

class CThing
{
public:
    unsigned char _pad0[0x20];
    unsigned int  m_presentBits[9];
    CInterfaceMap m_interfaces;

    template <class T> bool GetInterface(long type, T** out) const
    {
        if (m_presentBits[type >> 5] & (1U << (type & 31)))
        {
            long key = type;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};

bool IsThingRunningAwayFromCombat(const CThing& thing)
{
    const CTCStealth* stealth;
    if (thing.GetInterface(0x36, &stealth) && stealth->RunningAwayFromCombatTimer >= 0)
        return true;
    return false;
}

int main()
{
    CTCStealth stealth;
    CInterfaceMap::SEntry entries[2];
    entries[0].type = 0x10; entries[0].component = 0;
    entries[1].type = 0x36; entries[1].component = (CTCBase*)&stealth;
    CThing thing;
    for (int i = 0; i < 9; ++i) thing.m_presentBits[i] = 0;
    thing.m_interfaces.first = entries;
    thing.m_interfaces.last = entries + 2;
    int fails = 0;
    stealth.RunningAwayFromCombatTimer = 5;
    if (IsThingRunningAwayFromCombat(thing)) ++fails;          // bit not set: false
    thing.m_presentBits[1] = 0x400000;                         // type 0x36 present
    if (!IsThingRunningAwayFromCombat(thing)) ++fails;
    stealth.RunningAwayFromCombatTimer = 0;
    if (!IsThingRunningAwayFromCombat(thing)) ++fails;
    stealth.RunningAwayFromCombatTimer = -1;
    if (IsThingRunningAwayFromCombat(thing)) ++fails;
    if (fails) { std::printf("00864cb0_TEST FAIL\n"); return 1; }
    std::printf("00864cb0_TEST PASS\n");
    return 0;
}