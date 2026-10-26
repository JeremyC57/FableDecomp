// Behaviour test for 00728470: body copied from src/00728470.cpp (CTCRandomAppearanceMorph::vfunc_4) with the inline GetInterface;
// the interface map's LowerBound and UpdateMorphSet stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class CTCBase;
static int g_lower = 0; static long g_lowerKey = -1;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        ++g_lower; g_lowerKey = *type;
        SEntry* it = first;
        while (it != last && it->type < *type) ++it;
        return it;
    }
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    __forceinline bool GetInterface(long type, CTCBase** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = it->component;
            return true;
        }
        return false;
    }
};
static int g_update = 0;
class CTCRandomAppearanceMorph { public:
    virtual void v00() {}
    CThing*       ParentThing;
    unsigned char _pad08[0x20];
    CTCBase*      PTCSkeletalMorph;
    void UpdateMorphSet() { ++g_update; }
    void OnCreate();
};
void CTCRandomAppearanceMorph::OnCreate()
{
    CTCBase* morph;
    if (ParentThing->GetInterface(120, &morph))
        PTCSkeletalMorph = morph;
    UpdateMorphSet();
}
int main()
{
    CHECK((char*)&((CTCRandomAppearanceMorph*)0)->PTCSkeletalMorph == (char*)0 + 0x28);
    CThing thing; memset(&thing, 0, sizeof(thing));
    CInterfaceMap::SEntry entries[3];
    entries[0].type = 5;   entries[0].component = (CTCBase*)0x50;
    entries[1].type = 120; entries[1].component = (CTCBase*)0x120;
    entries[2].type = 130; entries[2].component = (CTCBase*)0x130;
    thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 3;
    CTCRandomAppearanceMorph m; m.ParentThing = &thing; m.PTCSkeletalMorph = (CTCBase*)0xdead;
    m.OnCreate();                                            // presence bit clear: no lookup, member untouched
    CHECK(g_lower == 0); CHECK(m.PTCSkeletalMorph == (CTCBase*)0xdead); CHECK(g_update == 1);
    thing.m_presentBits[120 >> 5] |= 1UL << (120 & 31);
    m.OnCreate();                                            // present: the entry's component is stored
    CHECK(g_lower == 1); CHECK(g_lowerKey == 120); CHECK(m.PTCSkeletalMorph == (CTCBase*)0x120); CHECK(g_update == 2);
    entries[1].type = 121;                                  // bit set but no exact entry: the end entry's word is taken
    entries[2].type = 130; thing.m_interfaces.last = entries + 2; entries[2].component = (CTCBase*)0x77;
    m.OnCreate();
    CHECK(g_lower == 2); CHECK(m.PTCSkeletalMorph == (CTCBase*)0x77); CHECK(g_update == 3);
    if (fails == 0) printf("00728470_TEST PASS\n"); else printf("00728470_TEST FAIL\n");
    return fails;
}