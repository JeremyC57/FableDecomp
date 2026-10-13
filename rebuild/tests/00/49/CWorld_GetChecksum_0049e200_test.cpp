#include <cstdio>
static unsigned long g_salt = 7; struct TM { unsigned long Get() { return 100; } };
struct W { unsigned char pad[0x50]; TM* tm; unsigned long Checksum() { return tm->Get() + g_salt; } };
int main()
{
    TM tm; W w; w.tm = &tm; if (w.Checksum() != 107) { std::printf("0049e200_TEST FAIL\n"); return 1; }

    std::printf("0049e200_TEST PASS\n");
    return 0;
}