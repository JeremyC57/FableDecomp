#include <cstdio>
#include <cstring>
struct CThing { int id; };
static int g_sets, g_live;
struct CBaseIntelligentPointer {
    virtual long Kind() { return 0; }
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { ++g_live; }
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CThing* item) { m_pItem = item; ++g_sets; }
    ~CBaseIntelligentPointer() { --g_live; }
};
struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual long Kind() { return 1; }
    CIntelligentPointer() {}
    CIntelligentPointer(CThing* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
struct COccupantVector {
    CIntelligentPointer* _M_start; CIntelligentPointer* _M_finish; CIntelligentPointer* _M_end_of_storage;
    CIntelligentPointer* begin() { return _M_start; }
    CIntelligentPointer& operator[](long n) { return *(begin() + n); }
};
struct CSeatTable {
    unsigned char _pad00[0x28];
    COccupantVector m_occupants;
    void SetOccupant(long seat, CThing* occupant) { m_occupants[seat] = CIntelligentPointer(occupant); }
};
int main()
{
    int ok = 1;
    CIntelligentPointer seats[3];
    CSeatTable table; table.m_occupants._M_start = seats; table.m_occupants._M_finish = seats + 3; table.m_occupants._M_end_of_storage = seats + 3;
    CThing diner;
    table.SetOccupant(1, &diner);
    if (seats[0].GetPItem() != 0 || seats[1].GetPItem() != &diner || seats[2].GetPItem() != 0) ok = 0;
    table.SetOccupant(1, 0);
    if (seats[1].GetPItem() != 0 || g_live != 3) ok = 0;
    if (!ok) { std::printf("007b85f0_TEST FAIL\n"); return 1; }
    std::printf("007b85f0_TEST PASS\n");
    return 0;
}