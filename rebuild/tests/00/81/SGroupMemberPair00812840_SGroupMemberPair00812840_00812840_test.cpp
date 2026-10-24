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
struct SGroupMemberPairMutable_00812840 {
    u64 first;
    CGroupMemberData second;
};
struct SGroupMemberPair_00812840 {
    const u64 first;
    CGroupMemberData second;
    SGroupMemberPair_00812840(const SGroupMemberPairMutable_00812840& p);
};
SGroupMemberPair_00812840::SGroupMemberPair_00812840(const SGroupMemberPairMutable_00812840& p)
    : first(p.first), second(p.second)
{
}
int main()
{
    CThing thing; thing.id = 5;
    SGroupMemberPairMutable_00812840 src;
    src.first = ((u64)0x12345678u << 32) | 0x9ABCDEF0u;
    src.second.m_member.SetPItem(&thing);
    src.second.m_isLeader = true; src.second.m_role = 3; src.second.m_joinTime = 4400;
    int before = g_ctor;
    SGroupMemberPair_00812840 dst(src);
    if (g_ctor != before + 1 || dst.first != src.first || dst.second.m_member.GetPItem() != &thing || dst.second.m_member.Kind() != 2
        || !dst.second.m_isLeader || dst.second.m_role != 3 || dst.second.m_joinTime != 4400) { std::printf("00812840_TEST FAIL\n"); return 1; }
    std::printf("00812840_TEST PASS\n");
    return 0;
}