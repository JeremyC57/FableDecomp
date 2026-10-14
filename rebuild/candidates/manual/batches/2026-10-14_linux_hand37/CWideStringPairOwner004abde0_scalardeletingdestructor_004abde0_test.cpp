#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_n = 0; static void* g_d[2];
struct CWideString { void* m_pData; ~CWideString() { g_d[g_n++] = m_pData; } };
struct SWideStringPair_004abde0 { CWideString m_first; CWideString m_second; };
struct CWideStringPairOwner_004abde0 { long m_0; SWideStringPair_004abde0 m_pair; ~CWideStringPairOwner_004abde0() { } };
static void Destroy_CWideStringPairOwner_004abde0(CWideStringPairOwner_004abde0* o) { delete o; }
int main() {
    CWideStringPairOwner_004abde0* o = new CWideStringPairOwner_004abde0; o->m_pair.m_first.m_pData = (void*)1; o->m_pair.m_second.m_pData = (void*)2; Destroy_CWideStringPairOwner_004abde0(o);
    if (g_n != 2 || g_d[0] != (void*)2 || g_d[1] != (void*)1) { std::printf("004abde0_TEST FAIL\n"); return 1; }
    std::printf("004abde0_TEST PASS\n"); return 0;
}
