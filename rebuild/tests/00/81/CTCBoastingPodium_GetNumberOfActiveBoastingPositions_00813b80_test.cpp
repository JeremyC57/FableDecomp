// Behaviour test for CTCBoastingPodium::GetNumberOfActiveBoastingPositions (0x00813B80).
#include <cstdio>

struct CTCBaseInterface;
struct CInterfaceMap {
    struct SEntry { long type; CTCBaseInterface* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) const { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
struct CThing {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    template<class T> bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type) it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
struct SBoastingPositionEntry { long first; CBaseIntelligentPointer second; };
template <class T> struct CRetailVector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T& operator[](unsigned long n) const { return *(begin() + n); }
};
class CTCBoastingPosition {
public:
    bool Activated;
    bool IsPositionActivated(long& cost) const { cost = 5; return Activated; }
};
class CTCBoastingPodium {
public:
    CRetailVector<SBoastingPositionEntry> BoastingPositions;
    long GetNumberOfActiveBoastingPositions() const
    {
        long count = 0;
        for (unsigned long i = 0; i < BoastingPositions.size(); ++i) {
            CTCBoastingPosition* position = 0;
            BoastingPositions[i].second.GetPItem()->GetInterface(&position, 0x71);
            long cost;
            if (position->IsPositionActivated(cost))
                ++count;
        }
        return count;
    }
};

int main()
{
    CTCBoastingPosition positions[3] = { { true }, { false }, { true } };
    CThing things[3];
    CInterfaceMap::SEntry entries[3][2];
    SBoastingPositionEntry list[3];
    for (int i = 0; i < 3; ++i) {
        for (int w = 0; w < 9; ++w) things[i].m_presentBits[w] = 0;
        things[i].m_presentBits[3] = 1UL << 17;
        entries[i][0].type = 0x20; entries[i][0].component = 0;
        entries[i][1].type = 0x71; entries[i][1].component = (CTCBaseInterface*)&positions[i];
        things[i].m_interfaces.first = entries[i]; things[i].m_interfaces.last = entries[i] + 2;
        list[i].first = i; list[i].second.m_pItem = &things[i];
    }
    CTCBoastingPodium podium;
    podium.BoastingPositions._M_start = list; podium.BoastingPositions._M_finish = list + 3; podium.BoastingPositions._M_end_of_storage = list + 3;
    bool ok = podium.GetNumberOfActiveBoastingPositions() == 2;
    positions[1].Activated = true;
    ok = ok && podium.GetNumberOfActiveBoastingPositions() == 3;
    podium.BoastingPositions._M_finish = list;
    ok = ok && podium.GetNumberOfActiveBoastingPositions() == 0;
    if (!ok) { std::printf("00813b80_TEST FAIL\n"); return 1; }
    std::printf("00813b80_TEST PASS\n");
    return 0;
}