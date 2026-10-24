#include <cstdio>
static void* g_rel = 0; static int g_calls = 0;
static void Release_00440d60(void* p) { g_rel = p; ++g_calls; }
struct CFirstMemberOwner_00440d60 { void* m_pFirst; void ReleaseFirstMember_00440d60() { if (m_pFirst) Release_00440d60(m_pFirst); } };
int main() { int v; CFirstMemberOwner_00440d60 o; o.m_pFirst = &v; o.ReleaseFirstMember_00440d60(); bool ok = g_rel == &v && g_calls == 1; o.m_pFirst = 0; o.ReleaseFirstMember_00440d60(); ok = ok && g_calls == 1; std::printf(ok ? "00440d60_TEST PASS\n" : "00440d60_TEST FAIL\n"); return ok ? 0 : 1; }
