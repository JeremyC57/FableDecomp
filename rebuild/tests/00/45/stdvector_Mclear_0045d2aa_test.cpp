#include <cstdio>
static void* g_rel = 0; static int g_calls = 0;
static void Release_0045d2aa(void* p) { g_rel = p; ++g_calls; }
struct CFirstMemberOwner_0045d2aa { void* m_pFirst; void ReleaseFirstMember_0045d2aa() { if (m_pFirst) Release_0045d2aa(m_pFirst); } };
int main() { int v; CFirstMemberOwner_0045d2aa o; o.m_pFirst = &v; o.ReleaseFirstMember_0045d2aa(); bool ok = g_rel == &v && g_calls == 1; o.m_pFirst = 0; o.ReleaseFirstMember_0045d2aa(); ok = ok && g_calls == 1; std::printf(ok ? "0045d2aa_TEST PASS\n" : "0045d2aa_TEST FAIL\n"); return ok ? 0 : 1; }
