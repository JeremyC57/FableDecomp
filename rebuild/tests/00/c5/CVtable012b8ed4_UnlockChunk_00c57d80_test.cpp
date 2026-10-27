// Behaviour test for 0x00C57D80 CVtable_012b8ed4::UnlockChunk
#include <cstdio>
static int g_processed = 0, g_state = 0, g_unqueued = 0, g_queued = 0, g_plays = 0, g_lookups = 0;
typedef void (*PFN)(unsigned int, int, int*);
static void GetSourceiImpl(unsigned int, int param, int* value) { *value = param == 0x1016 ? g_processed : g_state; }
struct Ctx {
    PFN pfnGetSourcei;
    void GetSourcei(unsigned int s, int p, int* v) { if (!pfnGetSourcei) { ++g_lookups; pfnGetSourcei = GetSourceiImpl; } pfnGetSourcei(s, p, v); }
};
static Ctx g_ctx = { 0 };
struct Voice {
    unsigned int m_source;
    void UnlockChunk()
    {
        int processed;
        g_ctx.GetSourcei(m_source, 0x1016, &processed);
        while (processed-- > 0) ++g_unqueued;
        ++g_queued;
        int state;
        g_ctx.GetSourcei(m_source, 0x1010, &state);
        if (state != 0x1012) ++g_plays;
    }
};
static int Fail() { std::printf("00c57d80_TEST FAIL\n"); return 1; }
int main()
{
    Voice v; v.m_source = 7;
    g_processed = 3; g_state = 0x1012;
    v.UnlockChunk();
    if (g_unqueued != 3 || g_queued != 1 || g_plays != 0 || g_lookups != 1) return Fail();
    g_processed = 0; g_state = 0x1014;            // stopped: restart playback
    v.UnlockChunk();
    if (g_unqueued != 3 || g_plays != 1 || g_lookups != 1) return Fail();
    std::printf("00c57d80_TEST PASS\n");
    return 0;
}