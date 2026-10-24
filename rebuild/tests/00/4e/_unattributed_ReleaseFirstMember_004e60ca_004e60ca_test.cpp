#include <cstdio>
static void* g_rel = 0; static int g_calls = 0;
static void Release_004e60ca(void* p) { g_rel = p; ++g_calls; }
struct CFirstMemberOwner_004e60ca { void* m_pFirst; void ReleaseFirstMember_004e60ca() { if (m_pFirst) Release_004e60ca(m_pFirst); } };
int main() { int v; CFirstMemberOwner_004e60ca o; o.m_pFirst = &v; o.ReleaseFirstMember_004e60ca(); bool ok = g_rel == &v && g_calls == 1; o.m_pFirst = 0; o.ReleaseFirstMember_004e60ca(); ok = ok && g_calls == 1; std::printf(ok ? "004e60ca_TEST PASS\n" : "004e60ca_TEST FAIL\n"); return ok ? 0 : 1; }
