// behaviour test for 0x005E9BC0: the text is empty
#include <cstdio>
static int Widen(const char* s, unsigned short* out) { int n = 0; while (s[n]) { out[n] = (unsigned char)s[n]; ++n; } out[n] = 0; return n; }
int main()
{
    unsigned short buf[4];
    if (Widen("", buf) != 0 || buf[0] != 0) { std::printf("005e9bc0_TEST FAIL\n"); return 1; }
    std::printf("005e9bc0_TEST PASS\n");
    return 0;
}