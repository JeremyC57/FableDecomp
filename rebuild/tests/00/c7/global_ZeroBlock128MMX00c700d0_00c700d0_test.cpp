// Behaviour test for 0x00C700D0 (standalone copy of the model and logic; the retail inline-asm block is modelled in C++)
#include <cstdio>
#include <cstring>

static void ZeroBlock128(void* dst) { std::memset(dst, 0, 128); }
int main()
{
    unsigned char buf[160]; std::memset(buf, 0x33, sizeof(buf));
    ZeroBlock128(buf + 16);
    for (int i = 0; i < 160; ++i) {
        unsigned char want = (i >= 16 && i < 144) ? 0 : 0x33;
        if (buf[i] != want) { std::printf("00c700d0_TEST FAIL\n"); return 1; }
    }
    std::printf("00c700d0_TEST PASS\n");
    return 0;
}