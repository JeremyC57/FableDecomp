#include <cstdio>
// Behaviour model of CAIStateGroup_JackOfBladesBase::OnActivate @ 0x009720C0: clear +0x18 of interface 0xCE when present.
struct CTCBase { int x; };
struct CInterfaceMap { struct SEntry { long type; CTCBase* component; }; SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; } };
struct CThing { unsigned char _pad0[0x20]; unsigned long m_presentBits[9]; CInterfaceMap m_interfaces;
    template<class T> bool GetInterface(T** ppOut, long interfaceType) {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) { long type = interfaceType; CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type); if (it == map->last || interfaceType < it->type) it = map->last; *ppOut = (T*)it->component; return true; }
        return false; } };
struct CTCJackOfBlades : CTCBase { unsigned char _pad[0x18]; bool m_defeated; };
struct CAIBrain { unsigned char _pad[0x18]; CThing* PCreature; };
struct CAIStateGroup_JackOfBladesBase { CAIBrain* PBrain;
    void OnActivate() { CTCJackOfBlades* p = 0; if (PBrain->PCreature->GetInterface(&p, 0xCE)) p->m_defeated = false; } };
int main()
{
    CTCJackOfBlades jack; jack.m_defeated = true; CInterfaceMap::SEntry entries[1] = { { 0xCE, &jack } };
    CThing thing; for (int i = 0; i < 9; ++i) thing.m_presentBits[i] = 0; thing.m_presentBits[6] |= 1UL << 14;
    thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 1;
    CAIBrain brain; brain.PCreature = &thing; CAIStateGroup_JackOfBladesBase g; g.PBrain = &brain;
    g.OnActivate(); bool ok = jack.m_defeated == false;
    jack.m_defeated = true; thing.m_presentBits[6] = 0; g.OnActivate(); ok = ok && jack.m_defeated == true;
    if (!ok) { std::printf("009720c0_TEST FAIL\n"); return 1; }
    std::printf("009720c0_TEST PASS\n");
    return 0;
}