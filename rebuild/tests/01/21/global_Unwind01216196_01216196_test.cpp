// Behaviour test for CAudioSource::CreateProcessor (0x00C55270): detect the CPU once, then build the SSE2 or the MMX/C variant
// of the processor for the speaker mode (0x100000 HRTF, 0x200000 four-tap, 0x400000 gain ramp, else stereo); null when the
// allocation fails.
#include <cstdio>
#include <cstdlib>
static bool g_cpuDetected = false, g_cpuHasSSE2 = false, g_failAlloc = false;
static int g_detects = 0; static unsigned g_lastSize = 0;
static void DetectCpu() { ++g_detects; g_cpuDetected = true; }
struct CBase {
    int kind;
    static void* operator new(size_t n) throw() { g_lastSize = (unsigned)n; return g_failAlloc ? 0 : std::malloc(n); }
    static void operator delete(void* p) { std::free(p); }
    virtual ~CBase() {}
};
template <int K, int N> struct CProc : CBase { unsigned char body[N - 16]; CProc() { kind = K; } };
static CBase* CreateProcessor(unsigned long mode)
{
    if (!g_cpuDetected) DetectCpu();
    if (g_cpuHasSSE2) {
        switch (mode) {
        case 0x100000: return new CProc<1, 0xc6c>;
        case 0x200000: return new CProc<2, 0x214>;
        case 0x400000: return new CProc<3, 0x78>;
        default:       return new CProc<4, 0x38>;
        }
    } else {
        switch (mode) {
        case 0x100000: return new CProc<5, 0x66c>;
        case 0x200000: return new CProc<6, 0x214>;
        case 0x400000: return new CProc<7, 0x78>;
        default:       return new CProc<8, 0x38>;
        }
    }
}
int main()
{
    bool ok = true;
    CBase* p = CreateProcessor(0x100000);
    ok = ok && g_detects == 1 && p && p->kind == 5; delete p;
    g_cpuHasSSE2 = true;
    p = CreateProcessor(0x100000); ok = ok && p->kind == 1 && g_detects == 1; delete p;
    p = CreateProcessor(0x200000); ok = ok && p->kind == 2; delete p;
    p = CreateProcessor(0x400000); ok = ok && p->kind == 3; delete p;
    p = CreateProcessor(7); ok = ok && p->kind == 4; delete p;
    g_cpuHasSSE2 = false;
    p = CreateProcessor(0x400000); ok = ok && p->kind == 7; delete p;
    p = CreateProcessor(0); ok = ok && p->kind == 8; delete p;
    g_failAlloc = true;
    p = CreateProcessor(0x200000); ok = ok && p == 0;
    if (!ok) { std::printf("00c55270_TEST FAIL\n"); return 1; }
    std::printf("01216196_TEST PASS\n");
    return 0;
}