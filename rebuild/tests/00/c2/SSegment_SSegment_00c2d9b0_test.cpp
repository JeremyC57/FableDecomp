// Test: SSegment::~SSegment -- destroying an entry releases exactly its +8 name string, once.
#include <cstdio>
#include <cstddef>
static int g_dtors = 0; static const void* g_where = 0;
struct CString { char* m_pchData; ~CString() { ++g_dtors; g_where = this; } };
struct SSegment { unsigned long m_first, m_count; CString m_name; };
int main()
{
    bool ok = offsetof(SSegment, m_name) == 8 && sizeof(SSegment) == 12;
    const void* where = 0;
    { SSegment s; s.m_first = 1; s.m_count = 2; s.m_name.m_pchData = 0; where = &s.m_name; }
    ok = ok && g_dtors == 1 && g_where == where;
    std::printf(ok ? "00c2d9b0_TEST PASS\n" : "00c2d9b0_TEST FAIL\n");
    return ok ? 0 : 1;
}