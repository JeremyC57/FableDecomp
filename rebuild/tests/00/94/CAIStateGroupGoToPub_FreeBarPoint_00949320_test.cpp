#include <cstdio>
// Behaviour model of CAIStateGroup_GoToPub::FreeBarPoint @ 0x00949320: interface 0xB9 lookup, free the index, reset to -1.
struct CTCBase { int x; };
struct CInterfaceMap { struct SEntry { long type; CTCBase* component; }; SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; } };
struct CThing { unsigned char _pad0[0x20]; unsigned long m_presentBits[9]; CInterfaceMap m_interfaces;
    template<class T> bool GetInterface(T** ppOut, long interfaceType) {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) { long type = interfaceType; CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type); if (it == map->last || interfaceType < it->type) it = map->last; *ppOut = (T*)it->component; return true; }
        return false; } };
static long g_freed = -99;
struct CTCBarPoints : CTCBase { void FreeBarPoint(long i) { g_freed = i; } };
struct CBaseIntelligentPointer { virtual void v00() {} CThing* m_pItem; CThing* GetPItem() { return m_pItem; } };
struct CAIStateGroup_GoToPub { CBaseIntelligentPointer m_pBar; long m_barPointIndex;
    void FreeBarPoint() { CTCBarPoints* p = 0; m_pBar.GetPItem()->GetInterface(&p, 0xB9); p->FreeBarPoint(m_barPointIndex); m_barPointIndex = -1; } };
int main()
{
    CTCBarPoints bar; CInterfaceMap::SEntry entries[2] = { { 0x10, 0 }, { 0xB9, &bar } };
    CThing thing; for (int i = 0; i < 9; ++i) thing.m_presentBits[i] = 0; thing.m_presentBits[5] |= 1UL << 25;
    thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 2;
    CAIStateGroup_GoToPub g; g.m_pBar.m_pItem = &thing; g.m_barPointIndex = 4;
    g.FreeBarPoint();
    bool ok = g_freed == 4 && g.m_barPointIndex == -1;
    if (!ok) { std::printf("00949320_TEST FAIL\n"); return 1; }
    std::printf("00949320_TEST PASS\n");
    return 0;
}