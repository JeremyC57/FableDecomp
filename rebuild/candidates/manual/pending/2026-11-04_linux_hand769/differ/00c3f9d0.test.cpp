// Behaviour test for InitSubSystemB 0x00C3F9D0: no audio -> false; device by type (1 / 4 / 3, 0 falls back to 3 when the
// enumeration or DirectSound fails, 2 / 5 through their init paths); a refused device init deletes it; the success path
// publishes the stream window (end = start + length unless either is -1) and rounds a larger buffer size up to 128.
#include <cstdio>
struct P { unsigned long start, length, size; long c, kind, rate, type; };
static bool g_audio = true; static int g_devs; static bool g_devInit = true, g_ds = true; static long g_enum = 1;
static unsigned long g_bufSize; static long g_end; static unsigned long g_block = 256;
static bool Init(const P* p, int* created)
{
    if (!g_audio) return false;
    *created = 0;
    switch (p->type) {
    case 1: *created = 1; break;
    case 4: *created = 4; break;
    case 2: if (g_ds) goto ready; return false;
    case 5: goto ready;
    case 0: if (g_enum > 0 && g_ds) goto ready;
    case 3: *created = 3; break;
    default: return false;
    }
    ++g_devs;
    if (!g_devInit) { --g_devs; *created = 0; return false; }
ready:
    g_bufSize = g_block;
    g_end = (p->start != (unsigned long)-1 && p->length != (unsigned long)-1) ? (long)(p->start + p->length) : -1;
    if (p->size > g_block) { unsigned long s = p->size; if (s & 0x7f) s = (s / 128 + 1) * 128; g_bufSize = s; }
    return true;
}
int main()
{
    bool ok = true; int c;
    P p = { 100, 50, 300, 0, 0, 0, 1 };
    ok = ok && Init(&p, &c) && c == 1 && g_end == 150 && g_bufSize == 384;
    p.type = 0; g_ds = false; ok = ok && Init(&p, &c) && c == 3;
    p.type = 2; ok = ok && !Init(&p, &c);
    g_ds = true; p.type = 0; ok = ok && Init(&p, &c) && c == 0;
    p.type = 9; ok = ok && !Init(&p, &c);
    p.type = 4; g_devInit = false; ok = ok && !Init(&p, &c) && c == 0; g_devInit = true;
    p.type = 5; p.start = (unsigned long)-1; p.size = 100; ok = ok && Init(&p, &c) && g_end == -1 && g_bufSize == 256;
    g_audio = false; ok = ok && !Init(&p, &c);
    if (ok) std::printf("00c3f9d0_TEST PASS\n"); else std::printf("00c3f9d0_TEST FAIL\n");
    return ok ? 0 : 1;
}
