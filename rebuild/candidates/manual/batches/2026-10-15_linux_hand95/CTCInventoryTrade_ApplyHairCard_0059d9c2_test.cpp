#include <cstdio>
// Behaviour of CTCInventoryTrade::ApplyHairCard (0x0059D9C2): only a thing with interface 0x5E gets the modifier.
struct CTCBase { long m_applied; long m_last; };
struct CPresenceBits { unsigned long m_bits[4]; bool test(long bit) { return (m_bits[bit >> 5] >> (bit & 31)) & 1; } };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};
struct CThing {
    CPresenceBits m_present; CInterfaceMap m_interfaces;
    template <class T> bool GetInterface(long type, T** out)
    {
        if (m_present.test(type)) {
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
};
struct CTCHeroAttachableAppearanceModifiers : CTCBase {
    void AddAppearanceModifierWithReplace(long index) { ++m_applied; m_last = index; }
};
struct CHairCardDef { long AppearanceModifier; };
struct CTCInventoryTrade {
    CThing* ParentThing;
    void ApplyHairCard(const CHairCardDef* card)
    {
        CTCHeroAttachableAppearanceModifiers* modifiers;
        if (ParentThing->GetInterface(0x5e, &modifiers))
            modifiers->AddAppearanceModifierWithReplace(card->AppearanceModifier);
    }
};
int main()
{
    CTCHeroAttachableAppearanceModifiers mods; mods.m_applied = 0; mods.m_last = 0;
    CTCBase other; other.m_applied = 0;
    CInterfaceMap::SEntry entries[3] = { { 3, &other }, { 0x5e, &mods }, { 0x70, &other } };
    CThing thing; thing.m_present.m_bits[0] = 0; thing.m_present.m_bits[1] = 0; thing.m_present.m_bits[2] = 1UL << (0x5e - 64); thing.m_present.m_bits[3] = 0;
    thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 3;
    CTCInventoryTrade t; t.ParentThing = &thing;
    CHairCardDef card; card.AppearanceModifier = 42;
    t.ApplyHairCard(&card);
    int bad = (mods.m_applied != 1 || mods.m_last != 42 || other.m_applied != 0);
    thing.m_present.m_bits[2] = 0; t.ApplyHairCard(&card);
    bad |= (mods.m_applied != 1);
    if (bad) { std::printf("0059d9c2_TEST FAIL\n"); return 1; }
    std::printf("0059d9c2_TEST PASS\n");
    return 0;
}
