// Behaviour test for 0x0070F410 _Temporary_buffer::_M_allocate_buffer (clamp to INT_MAX/4, halve until malloc succeeds).
#include <cstdio>
static int g_limit = 0;                                     // fake malloc: fails above this many bytes
static int g_calls = 0;
static void* FakeMalloc(unsigned int bytes) { ++g_calls; static int store[64]; return bytes <= (unsigned int)g_limit ? store : 0; }
struct Buf {
    int OriginalLen; int Len; void* Buffer;
    void Allocate()
    {
        OriginalLen = Len; Buffer = 0;
        if (Len > 0x1FFFFFFF) Len = 0x1FFFFFFF;
        while (Len > 0) { Buffer = FakeMalloc(Len * 4); if (Buffer) break; Len /= 2; }
    }
};
int main()
{
    bool ok = true;
    Buf a; a.Len = 10; g_limit = 256; a.Allocate();
    ok = ok && a.OriginalLen == 10 && a.Len == 10 && a.Buffer != 0 && g_calls == 1;
    Buf b; b.Len = 100; g_limit = 100; g_calls = 0; b.Allocate();       // 400, 200 fail; 25*4 = 100 fits
    ok = ok && b.OriginalLen == 100 && b.Len == 25 && b.Buffer != 0 && g_calls == 3;
    Buf c; c.Len = 0x7FFFFFFF; g_limit = 0; g_calls = 0; c.Allocate();  // clamped, then halved to 0
    ok = ok && c.OriginalLen == 0x7FFFFFFF && c.Len == 0 && c.Buffer == 0 && g_calls == 29;
    Buf d; d.Len = 0; g_calls = 0; d.Allocate();
    ok = ok && d.Len == 0 && d.Buffer == 0 && g_calls == 0;
    if (!ok) { std::printf("0070f410_TEST FAIL\n"); return 1; }
    std::printf("0070f410_TEST PASS\n");
    return 0;
}