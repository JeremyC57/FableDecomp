#include <cstdio>
static void* g_rel = 0; static int g_calls = 0;
static void Release_0045019a(void* p) { g_rel = p; ++g_calls; }
struct CFirstMemberOwner_0045019a { void* m_pFirst; void ReleaseFirstMember_0045019a() { if (m_pFirst) Release_0045019a(m_pFirst); } };
int main() { int v; CFirstMemberOwner_0045019a o; o.m_pFirst = &v; o.ReleaseFirstMember_0045019a(); bool ok = g_rel == &v && g_calls == 1; o.m_pFirst = 0; o.ReleaseFirstMember_0045019a(); ok = ok && g_calls == 1; std::printf(ok ? "0045019a_TEST PASS\n" : "0045019a_TEST FAIL\n"); return ok ? 0 : 1; }
