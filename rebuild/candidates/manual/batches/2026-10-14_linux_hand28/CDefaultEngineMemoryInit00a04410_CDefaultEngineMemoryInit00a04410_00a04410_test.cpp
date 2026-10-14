#include <cstdio>
#include <string.h>
#include <new>
struct CDefaultEngineMemoryInit_00a04410 {
    long m_block[8]; long m_unset; long m_a; long m_b;
    CDefaultEngineMemoryInit_00a04410() { m_b = 0; m_a = 0; memset(m_block, 0, sizeof(m_block)); }
};
int main()
{
    unsigned char raw[sizeof(CDefaultEngineMemoryInit_00a04410)]; memset(raw, 0xee, sizeof(raw));
    CDefaultEngineMemoryInit_00a04410* o = new (raw) CDefaultEngineMemoryInit_00a04410;
    for (int i = 0; i < 8; ++i) if (o->m_block[i] != 0) { std::printf("00a04410_TEST FAIL\n"); return 1; }
    if (o->m_a != 0 || o->m_b != 0 || o->m_unset != (long)0xeeeeeeee || sizeof(*o) != 0x2c) { std::printf("00a04410_TEST FAIL\n"); return 1; }
    std::printf("00a04410_TEST PASS\n"); return 0;
}
