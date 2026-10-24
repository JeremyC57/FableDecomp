#include <cstdio>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
static int g_ctor = 0;
struct CThing { int id; };
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { ++g_ctor; }
    virtual ~CBaseIntelligentPointer() { }
    virtual int Kind() const { return 1; }
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CThing* item) { m_pItem = item; }
};
struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual int Kind() const { return 2; }
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
struct CGroupMemberData {
    CIntelligentPointer m_member;
    bool m_isLeader;
    long m_role;
    long m_joinTime;
};
struct SGroupMemberPairMutable_00813280 {
    u64 first;
    CGroupMemberData second;
    SGroupMemberPairMutable_00813280(const u64& a, const CGroupMemberData& b) : first(a), second(b) { }
};
SGroupMemberPairMutable_00813280 MakePair_GroupMemberData_00813280(const u64& a, const CGroupMemberData& b)
{
    return SGroupMemberPairMutable_00813280(a, b);
}
int main()
{
    CThing thing; thing.id = 5;
    CGroupMemberData data;
    data.m_member.SetPItem(&thing);
    data.m_isLeader = false; data.m_role = 8; data.m_joinTime = 12;
    u64 key = ((u64)7u << 32) | 11u;
    SGroupMemberPairMutable_00813280 p = MakePair_GroupMemberData_00813280(key, data);
    if (p.first != key || p.second.m_member.GetPItem() != &thing || p.second.m_isLeader || p.second.m_role != 8 || p.second.m_joinTime != 12) { std::printf("00813280_TEST FAIL\n"); return 1; }
    std::printf("00813280_TEST PASS\n");
    return 0;
}