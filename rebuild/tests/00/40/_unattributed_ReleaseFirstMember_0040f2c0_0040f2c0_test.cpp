#include <cstdio>
static void* g_rel = 0; static int g_calls = 0;
static void Release_0040f2c0(void* p) { g_rel = p; ++g_calls; }
struct CFirstMemberOwner_0040f2c0 { void* m_pFirst; void ReleaseFirstMember_0040f2c0() { if (m_pFirst) Release_0040f2c0(m_pFirst); } };
int main() { int v; CFirstMemberOwner_0040f2c0 o; o.m_pFirst = &v; o.ReleaseFirstMember_0040f2c0(); bool ok = g_rel == &v && g_calls == 1; o.m_pFirst = 0; o.ReleaseFirstMember_0040f2c0(); ok = ok && g_calls == 1; std::printf(ok ? "0040f2c0_TEST PASS\n" : "0040f2c0_TEST FAIL\n"); return ok ? 0 : 1; }
