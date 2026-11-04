// Behaviour test for CAudioSoundBuffer::KeepShadowCopy 0x00C4DE00: mono 16-bit and 8-bit data duplicated into stereo,
// -5 on allocation failure, -14 for another format, the shadow buffer generated once (flag 2), an AL error deletes and
// poisons it.
#include <cstdio>
#include <cstring>
#include <cstdlib>
static bool g_allocFail; static int g_gens, g_deletes; static long g_alError; static unsigned char g_up[64]; static unsigned long g_upSize;
struct Wave { unsigned long size; long format; const void* data; };
struct Buf {
    unsigned long buffer, shadow, flags; const Wave* wave;
    long KeepShadowCopy()
    {
        if (g_allocFail) return -5;
        unsigned char* d = (unsigned char*)std::malloc(wave->size * 2);
        if (wave->format == 1) { const unsigned short* s = (const unsigned short*)wave->data;
            for (unsigned long i = 0; i < wave->size / 2; ++i) { ((unsigned short*)d)[i * 2] = s[i]; ((unsigned short*)d)[i * 2 + 1] = s[i]; } }
        else if (wave->format == 2) { const unsigned char* s = (const unsigned char*)wave->data;
            for (unsigned long i = 0; i < wave->size; ++i) { d[i * 2] = s[i]; d[i * 2 + 1] = s[i]; } }
        else { std::free(d); return -14; }
        if (!(flags & 2)) { ++g_gens; shadow = 77; flags |= 2; }
        std::memcpy(g_up, d, wave->size * 2); g_upSize = wave->size * 2;
        long r = g_alError ? -1 : 0; std::free(d);
        if (r < 0) { ++g_deletes; flags &= ~2u; shadow = 0xC0DAF00D; return r; }
        return 0;
    }
};
int main()
{
    bool ok = true;
    unsigned short s16[2] = { 0x1234, 0xABCD }; unsigned char s8[3] = { 1, 2, 3 };
    Wave w16 = { 4, 1, s16 }, w8 = { 3, 2, s8 }, wx = { 4, 7, s8 };
    Buf b = { 1, 0, 1, &w16 };
    ok = ok && b.KeepShadowCopy() == 0 && g_upSize == 8 && ((unsigned short*)g_up)[1] == 0x1234 && ((unsigned short*)g_up)[2] == 0xABCD && (b.flags & 2) && g_gens == 1;
    b.wave = &w8; ok = ok && b.KeepShadowCopy() == 0 && g_up[4] == 3 && g_up[5] == 3 && g_gens == 1;
    b.wave = &wx; ok = ok && b.KeepShadowCopy() == -14;
    g_allocFail = true; ok = ok && b.KeepShadowCopy() == -5; g_allocFail = false;
    b.wave = &w8; g_alError = 1; ok = ok && b.KeepShadowCopy() < 0 && b.shadow == 0xC0DAF00D && !(b.flags & 2) && g_deletes == 1;
    if (ok) std::printf("00c4de00_TEST PASS\n"); else std::printf("00c4de00_TEST FAIL\n");
    return ok ? 0 : 1;
}
