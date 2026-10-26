#include <cstdio>
#include <cstring>
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
class CTCBase { public: unsigned char _pad[0x10]; bool m_flag; };
class CThing { public: unsigned char _pad0[0x20]; unsigned long m_presentBits[9]; CInterfaceMap m_interfaces;
    bool GetInterface(long type, CTCBase** out) {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type; CInterfaceMap* map = &m_interfaces; CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type) it = map->last;
            *out = it->component; return true;
        }
        return false;
    } };
struct CThingOwner_007504e0 { void* __vftable; CThing* m_pThing; void SetInterface9Flag(bool value); };
void CThingOwner_007504e0::SetInterface9Flag(bool value) { CTCBase* component; if (m_pThing->GetInterface(9, &component)) component->m_flag = value; }
int main()
{
    CTCBase tc[2]; tc[0].m_flag = false; tc[1].m_flag = false; CInterfaceMap::SEntry entries[2] = { { 2, &tc[1] }, { 9, &tc[0] } };
    CThing t; std::memset(t.m_presentBits, 0, sizeof(t.m_presentBits)); t.m_interfaces.first = entries; t.m_interfaces.last = entries + 2;
    CThingOwner_007504e0 o; o.m_pThing = &t;
    o.SetInterface9Flag(true); if (tc[0].m_flag || tc[1].m_flag) { std::printf("007504e0_TEST FAIL\n"); return 1; }
    t.m_presentBits[0] = 0x200; o.SetInterface9Flag(true); if (!tc[0].m_flag || tc[1].m_flag) { std::printf("007504e0_TEST FAIL\n"); return 1; }
    o.SetInterface9Flag(false); if (tc[0].m_flag) { std::printf("007504e0_TEST FAIL\n"); return 1; }
    std::printf("007504e0_TEST PASS\n");
    return 0;
}