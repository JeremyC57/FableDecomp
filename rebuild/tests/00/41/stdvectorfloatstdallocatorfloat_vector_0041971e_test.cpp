#include <cstdio>
static void* g_rel = 0; static int g_calls = 0;
static void Release_0041971e(void* p) { g_rel = p; ++g_calls; }
struct CFirstMemberOwner_0041971e { void* m_pFirst; void ReleaseFirstMember_0041971e() { if (m_pFirst) Release_0041971e(m_pFirst); } };
int main() { int v; CFirstMemberOwner_0041971e o; o.m_pFirst = &v; o.ReleaseFirstMember_0041971e(); bool ok = g_rel == &v && g_calls == 1; o.m_pFirst = 0; o.ReleaseFirstMember_0041971e(); ok = ok && g_calls == 1; std::printf(ok ? "0041971e_TEST PASS\n" : "0041971e_TEST FAIL\n"); return ok ? 0 : 1; }
