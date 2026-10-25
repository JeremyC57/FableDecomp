// Behaviour test for 0x00CB0940 strmbase::AMGetWideString: E_POINTER on a null out pointer, E_OUTOFMEMORY when the
// allocation fails, else a CoTaskMemAlloc'd copy (terminator included) of the wide string.
#include <cstdio>
#include <cstdlib>
#include <cstring>

static int g_failAlloc = 0;
static unsigned long g_lastCb = 0;
static int lstrlenW_(const unsigned short* s) { int n = 0; while (s[n]) ++n; return n; }
static void* CoTaskMemAlloc_(unsigned long cb) { g_lastCb = cb; return g_failAlloc ? 0 : std::malloc(cb); }

static long AMGetWideString(const unsigned short* psz, unsigned short** ppszReturn)
{
    if (ppszReturn == 0)
        return 0x80004003L;
    unsigned long nameLen = sizeof(unsigned short) * (lstrlenW_(psz) + 1);
    *ppszReturn = (unsigned short*)CoTaskMemAlloc_(nameLen);
    if (*ppszReturn == 0)
        return 0x8007000EL;
    std::memcpy(*ppszReturn, psz, nameLen);
    return 0;
}

int main()
{
    const unsigned short src[] = { 'F', 'a', 'b', 'l', 'e', 0 };
    unsigned short* out = 0;
    if (AMGetWideString(src, 0) != (long)0x80004003L) { std::printf("00cb0940_TEST FAIL\n"); return 1; }
    if (AMGetWideString(src, &out) != 0 || g_lastCb != 12 || out == 0) { std::printf("00cb0940_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 6; ++i)
        if (out[i] != src[i]) { std::printf("00cb0940_TEST FAIL\n"); return 1; }
    std::free(out);
    g_failAlloc = 1;
    out = (unsigned short*)1;
    if (AMGetWideString(src, &out) != (long)0x8007000EL || out != 0) { std::printf("00cb0940_TEST FAIL\n"); return 1; }
    std::printf("00cb0940_TEST PASS\n");
    return 0;
}