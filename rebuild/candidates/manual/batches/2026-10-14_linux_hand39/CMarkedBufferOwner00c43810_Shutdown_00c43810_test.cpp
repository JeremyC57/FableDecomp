#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[16] = ""; static void* g_owner = 0; static long g_mark[5]; static void* g_ctx = 0;
struct SBufferMark_00c43810 { long m_words[5]; };
struct CMarkedBuffer_00c43810 { void* m_pBlock; void* m_pCursor; void* m_pBuffer; void ClearMarks() { std::strcat(g_seq, "M"); } void GetMark(SBufferMark_00c43810& out) { std::strcat(g_seq, "G"); for (int i = 0; i < 5; ++i) out.m_words[i] = 51 + i; } void Release() { std::strcat(g_seq, "R"); } };
struct CMarkedBufferOwner_00c43810;
typedef void (*PFNMARK_00c43810)(CMarkedBufferOwner_00c43810* owner, SBufferMark_00c43810* mark, void* context);
static void OnMark(CMarkedBufferOwner_00c43810* owner, SBufferMark_00c43810* mark, void* context) { std::strcat(g_seq, "K"); g_owner = owner; std::memcpy(g_mark, mark->m_words, sizeof g_mark); g_ctx = context; }
struct CMarkedBufferOwner_00c43810 { unsigned char _pad[0x54]; CMarkedBuffer_00c43810 m_buffer; unsigned char _pad60[0x74]; PFNMARK_00c43810 m_pfnMark; void* m_pMarkContext; void Cleanup() { std::strcat(g_seq, "C"); }
    void Shutdown() { Cleanup(); m_buffer.ClearMarks(); if (m_pfnMark) { SBufferMark_00c43810 mark; m_buffer.GetMark(mark); m_pfnMark(this, &mark, m_pMarkContext); } m_buffer.Release(); } };
int main() {
    CMarkedBufferOwner_00c43810 o; std::memset(&o, 0, sizeof o); int ctx; o.m_pfnMark = OnMark; o.m_pMarkContext = &ctx; o.Shutdown();
    if (std::strcmp(g_seq, "CMGKR") || g_owner != &o || g_mark[4] != 55 || g_ctx != &ctx) { std::printf("00c43810_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; o.m_pfnMark = 0; o.Shutdown(); if (std::strcmp(g_seq, "CMR")) { std::printf("00c43810_TEST FAIL\n"); return 1; }
    std::printf("00c43810_TEST PASS\n"); return 0;
}
