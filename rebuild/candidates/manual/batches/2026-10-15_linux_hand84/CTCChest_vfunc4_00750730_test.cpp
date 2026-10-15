#include <cstdio>
struct CTCBase { bool m_flag; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};
struct CThing {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    bool GetInterface(long type, CTCBase** out)
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
struct CTCChest {
    CThing* ParentThing; bool ChestOpen;
    void vfunc_4()
    {
        if (ChestOpen) {
            CTCBase* component;
            if (ParentThing->GetInterface(9, &component))
                component->m_flag = false;
        }
    }
};
int main()
{
    CTCBase comp = { true };
    CInterfaceMap::SEntry entries[2] = { { 9, &comp }, { 48, 0 } };
    CThing thing;
    for (int i = 0; i < 9; ++i) thing.m_presentBits[i] = 0;
    thing.m_presentBits[0] = 0x200;                        /* interface 9 present */
    thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 2;
    CTCChest chest; chest.ParentThing = &thing; chest.ChestOpen = false;
    chest.vfunc_4();
    if (!comp.m_flag) { std::printf("00750730_TEST FAIL\n"); return 1; }
    chest.ChestOpen = true; thing.m_presentBits[0] = 0;
    chest.vfunc_4();
    if (!comp.m_flag) { std::printf("00750730_TEST FAIL\n"); return 1; }
    thing.m_presentBits[0] = 0x200;
    chest.vfunc_4();
    if (comp.m_flag) { std::printf("00750730_TEST FAIL\n"); return 1; }
    std::printf("00750730_TEST PASS\n");
    return 0;
}
