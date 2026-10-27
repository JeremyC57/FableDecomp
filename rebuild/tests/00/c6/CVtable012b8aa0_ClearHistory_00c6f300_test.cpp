// Behaviour test for 0x00C6F300 (standalone copy of the model and logic; the retail inline-asm block is modelled in C++)
#include <cstdio>
#include <cstring>

static void Clear(unsigned char* p, int bytes) { std::memset(p, 0, bytes); }
int main()
{
    unsigned char buf[0x200]; std::memset(buf, 0x77, sizeof(buf));
    unsigned char* aligned = buf + 0x100;
    Clear(aligned - 64, 64);
    for (int i = 0; i < 0x200; ++i) {
        unsigned char want = (i >= 0x100 - 64 && i < 0x100) ? 0 : 0x77;
        if (buf[i] != want) { std::printf("00c6f300_TEST FAIL\n"); return 1; }
    }
    std::printf("00c6f300_TEST PASS\n");
    return 0;
}