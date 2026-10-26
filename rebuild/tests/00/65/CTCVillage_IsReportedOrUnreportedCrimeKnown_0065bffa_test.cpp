// Behaviour test for 0065bffa: body copied from src/0065bffa.cpp (CTCVillage::IsReportedOrUnreportedCrimeKnown), the callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class CTCBase;
struct CBitset {
    unsigned long w[9];
    bool test(unsigned long position) const { return (w[position >> 5] & (1UL << (position & 31))) != 0; }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
class CBaseObject {
public:
    unsigned char _pad00[0x20];
    CBitset       m_present;
    CInterfaceMap m_interfaces;
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Flags91;
};
class CThing : public CBaseObject {
public:
    bool HasTC(long type) const { return m_present.test(type); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return ((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
static int g_aliveCalls = 0;
bool IS_THING_ALIVE(CBaseObject* const& thing) { ++g_aliveCalls; return thing != 0 && !(thing->Flags91 & 1); }
struct CBaseIntelligentPointer { void* _vfptr; CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
class COpinionDeed;
class COpinionDeedFilter_IsCrimeToRunAndFindAGuard { public:};
static int g_findCalls = 0; static long* g_findIndex = (long*)1; static int g_findReported = -1;
class CTCCreatureOpinionOfHero { public:
    COpinionDeed* result;
    COpinionDeed* FindMostRecentDeed(const COpinionDeedFilter_IsCrimeToRunAndFindAGuard& filter, long* index, bool includeReported) { ++g_findCalls; g_findIndex = index; g_findReported = includeReported; return result; }
};
struct SVillagerNode { SVillagerNode* _M_next; SVillagerNode* _M_prev; CBaseIntelligentPointer _M_data; };
class CTCVillage { public:
    unsigned char  _pad_0x00[0x2c];
    SVillagerNode* VillagePeople;
    unsigned char  _pad_0x30[0x34];
    void*          PlayersCrimesHeader;
    unsigned long  PlayersCrimesCount;
    bool IsReportedOrUnreportedCrimeKnown();
};
bool CTCVillage::IsReportedOrUnreportedCrimeKnown()
{
    if (PlayersCrimesCount != 0)
        return true;
    for (SVillagerNode* node = VillagePeople->_M_next; node != VillagePeople; node = node->_M_next) {
        CThing* villager = node->_M_data.GetPItem();
        if (IS_THING_ALIVE(villager) && villager->HasTC(79)) {
            CTCCreatureOpinionOfHero* opinion = (CTCCreatureOpinionOfHero*)villager->GetTC(79);
            COpinionDeedFilter_IsCrimeToRunAndFindAGuard filter;
            if (opinion->FindMostRecentDeed(filter, 0, true))
                return true;
        }
    }
    return false;
}
int main()
{
    CThing v1, v2, v3; memset(&v1, 0, sizeof(v1)); memset(&v2, 0, sizeof(v2)); memset(&v3, 0, sizeof(v3));
    CTCCreatureOpinionOfHero op2, op3; op2.result = 0; op3.result = 0;
    CInterfaceMap::SEntry e2[2] = { { 0x10, (CTCBase*)0x99 }, { 79, (CTCBase*)&op2 } };
    CInterfaceMap::SEntry e3[2] = { { 0x10, (CTCBase*)0x99 }, { 79, (CTCBase*)&op3 } };
    v2.m_interfaces.first = e2; v2.m_interfaces.last = e2 + 2; v2.m_present.w[79 >> 5] = 1UL << (79 & 31);
    v3.m_interfaces.first = e3; v3.m_interfaces.last = e3 + 2; v3.m_present.w[79 >> 5] = 1UL << (79 & 31);
    SVillagerNode head, n1, n2, n3;
    head._M_next = &n1; n1._M_next = &n2; n2._M_next = &n3; n3._M_next = &head;
    n1._M_data.m_pItem = &v1; n2._M_data.m_pItem = &v2; n3._M_data.m_pItem = &v3;
    CTCVillage vil; memset(&vil, 0, sizeof(vil)); vil.VillagePeople = &head;
    // 1. recorded crimes: true without touching the list
    vil.PlayersCrimesCount = 3; CHECK(vil.IsReportedOrUnreportedCrimeKnown() == true); CHECK(g_aliveCalls == 0);
    vil.PlayersCrimesCount = 0;
    // 2. v1 has no opinion component, v2/v3 find nothing: false, all three visited, two lookups
    CHECK(vil.IsReportedOrUnreportedCrimeKnown() == false); CHECK(g_aliveCalls == 3); CHECK(g_findCalls == 2); CHECK(g_findIndex == 0); CHECK(g_findReported == 1);
    // 3. dead v2 is skipped; v3 finds a deed: true, loop stops there
    v2.Flags91 = 1; op3.result = (COpinionDeed*)0x1234; g_aliveCalls = 0; g_findCalls = 0;
    CHECK(vil.IsReportedOrUnreportedCrimeKnown() == true); CHECK(g_aliveCalls == 3); CHECK(g_findCalls == 1);
    // 4. v2 alive and finds first: stops before v3
    v2.Flags91 = 0; op2.result = (COpinionDeed*)0x5678; g_aliveCalls = 0; g_findCalls = 0;
    CHECK(vil.IsReportedOrUnreportedCrimeKnown() == true); CHECK(g_aliveCalls == 2); CHECK(g_findCalls == 1);
    // 5. empty list: false
    head._M_next = &head; g_aliveCalls = 0;
    CHECK(vil.IsReportedOrUnreportedCrimeKnown() == false); CHECK(g_aliveCalls == 0);
    if (fails == 0) printf("0065bffa_TEST PASS\n"); else printf("0065bffa_TEST FAIL\n");
    return fails;
}