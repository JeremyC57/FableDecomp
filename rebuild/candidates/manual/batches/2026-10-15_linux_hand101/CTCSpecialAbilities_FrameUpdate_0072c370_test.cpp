// Behaviour test for CTCSpecialAbilities::FrameUpdate (0x0072C370): abilities are cancelled while the interface-0x29
// component exists with its +0x0d flag clear.
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
    void SetBit(long type) { m_presentBits[type >> 5] |= 1UL << (type & 31); }
};

static int g_cancelAll = 0;

struct CTCInterface29 {
    bool m_flag0d;
};

struct CTCSpecialAbilities {
    CThing* ParentThing;
    void CancelAllAbilities() { ++g_cancelAll; }
    void FrameUpdate();
};

void CTCSpecialAbilities::FrameUpdate()
{
    CTCInterface29* component;
    if (ParentThing->GetInterface(0x29, &component)) {
        if (!component->m_flag0d)
            CancelAllAbilities();
    }
}

int main()
{
    CTCInterface29 comp; comp.m_flag0d = true;
    CInterfaceMap::SEntry entries[2] = { { 0x29, (CTCBase*)&comp }, { 0, 0 } };
    CThing t;
    for (int i = 0; i < 9; ++i) t.m_presentBits[i] = 0;
    t.m_interfaces.first = entries; t.m_interfaces.last = entries + 1;
    CTCSpecialAbilities s; s.ParentThing = &t;
    s.FrameUpdate();                                        // no bit
    t.SetBit(0x29);
    s.FrameUpdate();                                        // flag set
    comp.m_flag0d = false;
    s.FrameUpdate();                                        // cancels
    if (g_cancelAll != 1) { std::printf("0072c370_TEST FAIL\n"); return 1; }
    std::printf("0072c370_TEST PASS\n");
    return 0;
}
