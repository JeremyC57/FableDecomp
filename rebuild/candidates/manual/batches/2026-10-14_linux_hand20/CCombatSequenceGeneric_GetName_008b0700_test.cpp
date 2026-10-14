#include <cstdio>
static int g_live = 0;
struct CCharString { long value; CCharString(long v) : value(v) { ++g_live; } CCharString(const CCharString& o) : value(o.value) { ++g_live; } ~CCharString() { --g_live; } };
struct CDefStringId { long m_id; CDefStringId(long id) : m_id(id) { } };
struct CDefStringTable { CCharString GetString(long id) { return CCharString(id * 10); } };
static CDefStringTable g_DefStringTable;
struct CDef { virtual CDefStringId GetNameStringId() const { return CDefStringId(7); } };
struct CSeq { const CDef* Def_Object; CCharString GetName() { CCharString name = g_DefStringTable.GetString(Def_Object->GetNameStringId().m_id); return name; } };
int main()
{
    CDef d; CSeq s; s.Def_Object = &d;
    { CCharString n = s.GetName(); if (n.value != 70) { std::printf("008b0700_TEST FAIL\n"); return 1; } }
    if (g_live != 0) { std::printf("008b0700_TEST FAIL\n"); return 1; }
    std::printf("008b0700_TEST PASS\n");
    return 0;
}
