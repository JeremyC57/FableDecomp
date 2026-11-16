// Test: SOpenArgs::~SOpenArgs -- destroying the record releases exactly its +0x10 name pointer, once.
#include <cstdio>
#include <cstddef>
static int g_dtors = 0; static const void* g_where = 0;
struct CNamePointer { void* m_p0; void* m_p4; ~CNamePointer() { ++g_dtors; g_where = this; } };
struct SOpenArgs { unsigned char head[0x10]; CNamePointer m_name; unsigned char tail[0x1c - 0x10 - 8]; };
int main()
{
    bool ok = offsetof(SOpenArgs, m_name) == 0x10 && sizeof(SOpenArgs) == 0x1c;
    const void* where = 0;
    { SOpenArgs r; where = &r.m_name; }
    ok = ok && g_dtors == 1 && g_where == where;
    std::printf(ok ? "00c47ea0_TEST PASS\n" : "00c47ea0_TEST FAIL\n");
    return ok ? 0 : 1;
}