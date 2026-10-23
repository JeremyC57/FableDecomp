// Behaviour test for CTCSpecialAbilities::CancelAllAbilities (0x0072ba40): every present ability component is cancelled once, in order.
#include <cstdio>

static int g_log[16];
static int g_count = 0;

struct CTCBase { int tag; void CancelMove() { g_log[g_count++] = tag; } };

struct CInterfaceMap {
    struct SEntry { int type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const int* type)
    {
        SEntry* it = first;
        while (it != last && it->type < *type) ++it;
        return it;
    }
};

struct CThing {
    unsigned int m_presentBits[9];
    CInterfaceMap m_interfaces;
    bool HasTC(int type) const
    {
        if (m_presentBits[type >> 5] & (1u << (type & 31)))
            return true;
        return false;
    }
    CTCBase* GetTC(int type)
    {
        int key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return ((it == map->last || type < it->type) ? map->last : it)->component;
    }
};

static const int kTypes[] = { 0x92, 0x93, 0x84, 0x91, 0x9a, 0x87, 0x88, 0x85, 0x86 };

struct CTCSpecialAbilities {
    CThing* ParentThing;
    void CancelAllAbilities()
    {
        for (unsigned int i = 0; i < sizeof(kTypes) / sizeof(kTypes[0]); ++i)
            if (ParentThing->HasTC(kTypes[i])) ParentThing->GetTC(kTypes[i])->CancelMove();
    }
};

int main()
{
    CTCBase comps[3];
    comps[0].tag = 0x84; comps[1].tag = 0x86; comps[2].tag = 0x92;
    CInterfaceMap::SEntry entries[4];
    entries[0].type = 0x84; entries[0].component = &comps[0];
    entries[1].type = 0x86; entries[1].component = &comps[1];
    entries[2].type = 0x92; entries[2].component = &comps[2];
    entries[3].type = 0; entries[3].component = 0;
    CThing thing;
    for (int i = 0; i < 9; ++i) thing.m_presentBits[i] = 0;
    thing.m_presentBits[4] = (1u << 4) | (1u << 6) | (1u << 18);
    thing.m_interfaces.first = entries;
    thing.m_interfaces.last = entries + 3;
    CTCSpecialAbilities sa;
    sa.ParentThing = &thing;
    sa.CancelAllAbilities();
    if (g_count != 3 || g_log[0] != 0x92 || g_log[1] != 0x84 || g_log[2] != 0x86) {
        std::printf("0072ba40_TEST FAIL\n");
        return 1;
    }
    std::printf("0072ba40_TEST PASS\n");
    return 0;
}