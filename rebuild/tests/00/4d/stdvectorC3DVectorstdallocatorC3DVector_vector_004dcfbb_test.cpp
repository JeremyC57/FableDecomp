#include <cstdio>
static void* g_rel = 0; static int g_calls = 0;
static void Release_004dcfbb(void* p) { g_rel = p; ++g_calls; }
struct CFirstMemberOwner_004dcfbb { void* m_pFirst; void ReleaseFirstMember_004dcfbb() { if (m_pFirst) Release_004dcfbb(m_pFirst); } };
int main() { int v; CFirstMemberOwner_004dcfbb o; o.m_pFirst = &v; o.ReleaseFirstMember_004dcfbb(); bool ok = g_rel == &v && g_calls == 1; o.m_pFirst = 0; o.ReleaseFirstMember_004dcfbb(); ok = ok && g_calls == 1; std::printf(ok ? "004dcfbb_TEST PASS\n" : "004dcfbb_TEST FAIL\n"); return ok ? 0 : 1; }
