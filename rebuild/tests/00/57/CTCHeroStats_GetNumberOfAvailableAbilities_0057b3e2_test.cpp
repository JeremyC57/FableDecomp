#include <cstdio>
// Behaviour of CTCHeroStats::GetNumberOfAvailableAbilities (0x0057B3E2): finds the parent thing's abilities inventory
// (interface 0x6F: presence bit, then the sorted interface map) and counts abilities 1..19 whose level is not -1.
class CTCBase { public: int id; };
class CTCInventoryAbilities : public CTCBase { public:
    long levels[20];
    long GetAbilityLevel(long ability) const { return levels[ability]; }
};
struct CPresenceBits {
    bool bits[200];
    bool test(unsigned long bit) const { return bits[bit]; }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const
    {
        SEntry* it = first;
        while (it != last && it->type < *type) ++it;
        return it;
    }
};
class CThing { public:
    CPresenceBits UsingComponents;
    CInterfaceMap Components;
    template <class T> bool GetInterface(T** out, long type)
    {
        if (UsingComponents.test(type)) {
            long key = type;
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CTCHeroStats_Retail {
    CThing* ParentThing;
    long GetNumberOfAvailableAbilities() const
    {
        CTCInventoryAbilities* abilities = 0;
        ParentThing->GetInterface(&abilities, 0x6F);
        long available = 0;
        for (long ability = 1; ability != 20; ++ability) {
            if (abilities->GetAbilityLevel(ability) != -1)
                ++available;
        }
        return available;
    }
};
int main()
{
    CTCInventoryAbilities abilities;
    for (int i = 0; i < 20; ++i) abilities.levels[i] = -1;
    abilities.levels[0] = 5;   // ability 0 is never counted
    abilities.levels[1] = 0; abilities.levels[7] = 2; abilities.levels[19] = 3;
    CTCBase other;
    CInterfaceMap::SEntry entries[3] = { { 0x10, &other }, { 0x6F, &abilities }, { 0x80, &other } };
    CThing thing;
    for (int i = 0; i < 200; ++i) thing.UsingComponents.bits[i] = false;
    thing.UsingComponents.bits[0x6F] = true;
    thing.Components.first = entries; thing.Components.last = entries + 3;
    CTCHeroStats_Retail s; s.ParentThing = &thing;
    if (s.GetNumberOfAvailableAbilities() != 3) { std::printf("0057b3e2_TEST FAIL\n"); return 1; }
    abilities.levels[7] = -1;
    if (s.GetNumberOfAvailableAbilities() != 2) { std::printf("0057b3e2_TEST FAIL\n"); return 1; }
    std::printf("0057b3e2_TEST PASS\n");
    return 0;
}