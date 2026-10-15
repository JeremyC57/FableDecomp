// Behaviour test for CAIStateGroup_TavernServer::IsServingPointValid (0x00967250): copy of the model + logic.
#include <cstdio>

struct CTCBase { long seatLimit; };

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
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CTCTavernTable : CTCBase {
    bool IsSeatValid(long seatIndex) { return seatIndex >= 0 && seatIndex < seatLimit; }
};

struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};

struct CAIStateGroup_TavernServer {
    CBaseIntelligentPointer PTable;
    long SeatIndex;
    bool IsServingPointValid()
    {
        CTCTavernTable* table = 0;
        PTable.GetPItem()->GetInterface(&table, 0xB8);
        if (!table->IsSeatValid(SeatIndex)) {
            SeatIndex = -1;
            return false;
        }
        return true;
    }
};

int main()
{
    CTCTavernTable table; table.seatLimit = 4;
    CInterfaceMap::SEntry entries[3] = { { 5, 0 }, { 0xB8, &table }, { 0 /*end sentinel*/, 0 } };
    CThing thing;
    for (int i = 0; i < 9; ++i) thing.m_presentBits[i] = 0;
    thing.m_presentBits[0xB8 >> 5] = 1UL << (0xB8 & 31);
    thing.m_interfaces.first = entries;
    thing.m_interfaces.last = entries + 2;
    CAIStateGroup_TavernServer g;
    g.PTable.m_pItem = &thing;
    g.SeatIndex = 2;
    if (!g.IsServingPointValid() || g.SeatIndex != 2) { std::printf("00967250_TEST FAIL\n"); return 1; }
    g.SeatIndex = 7;
    if (g.IsServingPointValid() || g.SeatIndex != -1) { std::printf("00967250_TEST FAIL\n"); return 1; }
    std::printf("00967250_TEST PASS\n");
    return 0;
}
