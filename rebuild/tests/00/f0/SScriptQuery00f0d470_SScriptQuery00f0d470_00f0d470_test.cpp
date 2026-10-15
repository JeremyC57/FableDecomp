#include <cstdio>
#include <cstring>
static int g_strings;
struct CCharString { void* PStringData; CCharString() : PStringData(0) { ++g_strings; } };
struct SScriptQuery {
    CCharString m_name; long m_unset04; long m_unset08; CCharString m_category; long m_a; long m_b; long m_c;
    SScriptQuery() : m_a(0), m_b(0), m_c(0) {}
};
int main()
{
    int ok = 1;
    SScriptQuery q;
    if (g_strings != 2 || q.m_name.PStringData || q.m_category.PStringData || q.m_a || q.m_b || q.m_c) ok = 0;
    if (!ok) { std::printf("00f0d470_TEST FAIL\n"); return 1; }
    std::printf("00f0d470_TEST PASS\n");
    return 0;
}