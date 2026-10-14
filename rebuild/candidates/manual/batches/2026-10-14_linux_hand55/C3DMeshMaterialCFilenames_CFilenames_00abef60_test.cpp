#include <cstdio>
#include <cstdlib>
#include <cstring>
static long g_strings = 0; struct CCharString { void* m_pStringData; CCharString() : m_pStringData(0) { ++g_strings; } };
struct CCountedBase { virtual void v00() { } long m_references; CCountedBase() : m_references(1) { } };
struct CFilenames : CCountedBase { CCharString m_diffuse; CCharString m_normal; CCharString m_specular; CCharString m_environment; CFilenames() : CCountedBase() { } };

int main()
{
    CFilenames f; if (f.m_references != 1 || g_strings != 4 || f.m_diffuse.m_pStringData || f.m_normal.m_pStringData || f.m_specular.m_pStringData || f.m_environment.m_pStringData) { std::printf("00abef60_TEST FAIL\n"); return 1; }
    std::printf("00abef60_TEST PASS\n");
    return 0;
}
