#include <cstdio>
#include <cstdlib>
#include <cstring>
// behaviour test for 0x00C432F0: exercises the same logic through a copy of the model (compiled separately from the source)
struct SBufferMark_00c43810 { long m_words[5]; };
struct CMarkedBuffer_00c43810 { void* m_pBlock; void* m_pCursor; void* m_pBuffer; void GetMark(SBufferMark_00c43810& out) { for (int i = 0; i < 5; ++i) out.m_words[i] = 100 + i; } };
struct CMarkedBufferOwner_00c43810;
typedef void (*PFNMARK_00c43810)(CMarkedBufferOwner_00c43810* owner, SBufferMark_00c43810* mark, void* context);
struct CMarkedBufferOwner_00c43810 {
    CMarkedBuffer_00c43810 m_buffer; PFNMARK_00c43810 m_pfnMark; void* m_pMarkContext;
    void ReportMark() { if (m_pfnMark) { SBufferMark_00c43810 mark; m_buffer.GetMark(mark); m_pfnMark(this, &mark, m_pMarkContext); } }
};
static int g_calls = 0; static long g_sum = 0; static void* g_ctx = 0; static CMarkedBufferOwner_00c43810* g_owner = 0;
static void OnMark(CMarkedBufferOwner_00c43810* owner, SBufferMark_00c43810* mark, void* context) { ++g_calls; g_owner = owner; g_ctx = context; for (int i = 0; i < 5; ++i) g_sum += mark->m_words[i]; }
static bool Run()
{
    CMarkedBufferOwner_00c43810 o; int ctx; o.m_pfnMark = 0; o.m_pMarkContext = &ctx;
    o.ReportMark();
    if (g_calls != 0) return false;
    o.m_pfnMark = OnMark; o.ReportMark();
    return g_calls == 1 && g_sum == 510 && g_ctx == &ctx && g_owner == &o;
}
int main()
{
    if (!Run()) { std::printf("00c432f0_TEST FAIL\n"); return 1; }
    std::printf("00c432f0_TEST PASS\n");
    return 0;
}
