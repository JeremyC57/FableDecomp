#include <cstdio>
static void* g_rel = 0; static int g_calls = 0;
static void Release_00462e04(void* p) { g_rel = p; ++g_calls; }
struct CFirstMemberOwner_00462e04 { void* m_pFirst; void ReleaseFirstMember_00462e04() { if (m_pFirst) Release_00462e04(m_pFirst); } };
int main() { int v; CFirstMemberOwner_00462e04 o; o.m_pFirst = &v; o.ReleaseFirstMember_00462e04(); bool ok = g_rel == &v && g_calls == 1; o.m_pFirst = 0; o.ReleaseFirstMember_00462e04(); ok = ok && g_calls == 1; std::printf(ok ? "00462e04_TEST PASS\n" : "00462e04_TEST FAIL\n"); return ok ? 0 : 1; }
