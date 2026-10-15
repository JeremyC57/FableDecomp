// Behaviour test for CTCStealth::StartSneaking (0x00747560): once only, mode 7 when the mode manager exists.
#include <cstdio>

struct CTCBase;
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

struct CThing {
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
};

static int g_addMode = 0;
static unsigned long g_mode = 0;
struct CTCCreatureModeManager {
    bool AddMode(unsigned long mode) { ++g_addMode; g_mode = mode; return true; }
};

struct CTCStealth {
    CThing* ParentThing;
    bool    Sneaking;
    void StartSneaking();
};

void CTCStealth::StartSneaking()
{
    if (!Sneaking) {
        CTCCreatureModeManager* modes;
        if (ParentThing->GetInterface(0x31, &modes))
            modes->AddMode(7);
        Sneaking = true;
    }
}

int main()
{
    CTCCreatureModeManager modes;
    CInterfaceMap::SEntry entries[2] = { { 0x31, (CTCBase*)&modes }, { 0, 0 } };
    CThing t;
    for (int i = 0; i < 9; ++i) t.m_presentBits[i] = 0;
    t.m_interfaces.first = entries; t.m_interfaces.last = entries + 1;
    CTCStealth s; s.ParentThing = &t; s.Sneaking = false;
    s.StartSneaking();                                      // no mode manager: flag only
    if (!s.Sneaking || g_addMode != 0) { std::printf("00747560_TEST FAIL\n"); return 1; }
    t.m_presentBits[0x31 >> 5] |= 1UL << (0x31 & 31);
    s.StartSneaking();                                      // already sneaking
    s.Sneaking = false;
    s.StartSneaking();
    s.StartSneaking();
    if (!s.Sneaking || g_addMode != 1 || g_mode != 7) { std::printf("00747560_TEST FAIL\n"); return 1; }
    std::printf("00747560_TEST PASS\n");
    return 0;
}