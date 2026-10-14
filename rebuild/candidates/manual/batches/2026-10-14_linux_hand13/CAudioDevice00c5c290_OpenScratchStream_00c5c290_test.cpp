#include <cstdio>
unsigned char g_audioScratch[0x100];
static int g_prepared = 0; static unsigned char* g_buf = 0; static unsigned long g_size = 0, g_fmt = 0; static long g_mode = 0; static void* g_cb = (void*)1;
struct CAudioStream { unsigned char _body[0x20]; void Open(unsigned char* b, unsigned long s, long m, unsigned long f, void* cb, void* ctx) { g_buf = b; g_size = s; g_mode = m; g_fmt = f; g_cb = cb ? cb : ctx; } };
struct CAudioDevice {
    unsigned char _pad0[0x5c]; unsigned long Format; unsigned char _pad1[0x78]; CAudioStream* ActiveStream; unsigned char _pad2[0x114]; CAudioStream ScratchStream;
    void Prepare() { ++g_prepared; }
    void OpenScratchStream() { Prepare(); ScratchStream.Open((unsigned char*)(((unsigned long)g_audioScratch + 15) & ~15u), 0x80, 2, Format, 0, 0); ActiveStream = &ScratchStream; }
};
int main()
{
    CAudioDevice d; d.Format = 0x1234; d.ActiveStream = 0;
    d.OpenScratchStream();
    unsigned long lo = (unsigned long)g_audioScratch, hi = lo + 15;
    if (g_prepared != 1 || !g_buf || ((unsigned long)g_buf & 15) || (unsigned long)g_buf < lo || (unsigned long)g_buf > hi || g_size != 0x80 || g_mode != 2 || g_fmt != 0x1234 || g_cb != 0 || d.ActiveStream != &d.ScratchStream) { std::printf("00c5c290_TEST FAIL\n"); return 1; }
    std::printf("00c5c290_TEST PASS\n");
    return 0;
}
