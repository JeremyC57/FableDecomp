#include <cstdio>
static void* g_rel = 0; static int g_calls = 0;
static void Release_00cd2d0b(void* p) { g_rel = p; ++g_calls; }
struct CFirstMemberOwner_00cd2d0b { void* m_pFirst; void ReleaseFirstMember_00cd2d0b() { if (m_pFirst) Release_00cd2d0b(m_pFirst); } };
int main() { int v; CFirstMemberOwner_00cd2d0b o; o.m_pFirst = &v; o.ReleaseFirstMember_00cd2d0b(); bool ok = g_rel == &v && g_calls == 1; o.m_pFirst = 0; o.ReleaseFirstMember_00cd2d0b(); ok = ok && g_calls == 1; std::printf(ok ? "00cd2d0b_TEST PASS\n" : "00cd2d0b_TEST FAIL\n"); return ok ? 0 : 1; }
