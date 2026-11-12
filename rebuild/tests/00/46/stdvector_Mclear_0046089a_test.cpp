#include <cstdio>
static void* g_rel = 0; static int g_calls = 0;
static void Release_0046089a(void* p) { g_rel = p; ++g_calls; }
struct CFirstMemberOwner_0046089a { void* m_pFirst; void ReleaseFirstMember_0046089a() { if (m_pFirst) Release_0046089a(m_pFirst); } };
int main() { int v; CFirstMemberOwner_0046089a o; o.m_pFirst = &v; o.ReleaseFirstMember_0046089a(); bool ok = g_rel == &v && g_calls == 1; o.m_pFirst = 0; o.ReleaseFirstMember_0046089a(); ok = ok && g_calls == 1; std::printf(ok ? "0046089a_TEST PASS\n" : "0046089a_TEST FAIL\n"); return ok ? 0 : 1; }
