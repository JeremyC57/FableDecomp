// Behaviour test for 0x00C57AF0 (standalone copy of the model and logic)
#include <cstdio>
#include <cstring>

static int g_processed = 0; static int g_param = 0; static unsigned int g_source = 0; static int g_resolves = 0;
typedef void (*PFN)(unsigned int, int, int*);
static void FakeGetSourcei(unsigned int s, int p, int* v) { g_source = s; g_param = p; *v = g_processed; }
struct CAudioContext { PFN pfnGetSourcei;
    void GetSourcei(unsigned int s, int p, int* v) { if (!pfnGetSourcei) { pfnGetSourcei = FakeGetSourcei; g_resolves++; } pfnGetSourcei(s, p, v); } };
CAudioContext g_AudioContext;
struct Buf { unsigned int m_source, m_bufferCount, m_queuedCount;
    bool IsChunkReady()
    {
        if (m_queuedCount < m_bufferCount)
            return true;
        int processed;
        g_AudioContext.GetSourcei(m_source, 0x1016, &processed);
        return processed > 0;
    } };
int main()
{
    g_AudioContext.pfnGetSourcei = 0;
    Buf b; b.m_source = 9; b.m_bufferCount = 4; b.m_queuedCount = 2;
    if (!b.IsChunkReady() || g_resolves != 0) { std::printf("00c57af0_TEST FAIL\n"); return 1; }
    b.m_queuedCount = 4; g_processed = 0;
    if (b.IsChunkReady() || g_resolves != 1 || g_param != 0x1016 || g_source != 9) { std::printf("00c57af0_TEST FAIL\n"); return 1; }
    g_processed = 2;
    if (!b.IsChunkReady() || g_resolves != 1) { std::printf("00c57af0_TEST FAIL\n"); return 1; }
    std::printf("00c57af0_TEST PASS\n");
    return 0;
}