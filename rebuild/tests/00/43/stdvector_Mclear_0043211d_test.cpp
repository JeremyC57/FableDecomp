#include <cstdio>
static void* g_rel = 0; static int g_calls = 0;
static void Release_0043211d(void* p) { g_rel = p; ++g_calls; }
struct CFirstMemberOwner_0043211d { void* m_pFirst; void ReleaseFirstMember_0043211d() { if (m_pFirst) Release_0043211d(m_pFirst); } };
int main() { int v; CFirstMemberOwner_0043211d o; o.m_pFirst = &v; o.ReleaseFirstMember_0043211d(); bool ok = g_rel == &v && g_calls == 1; o.m_pFirst = 0; o.ReleaseFirstMember_0043211d(); ok = ok && g_calls == 1; std::printf(ok ? "0043211d_TEST PASS\n" : "0043211d_TEST FAIL\n"); return ok ? 0 : 1; }
