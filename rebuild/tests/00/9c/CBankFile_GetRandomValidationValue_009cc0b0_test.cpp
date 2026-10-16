// Behaviour test for 009cc0b0: CBankFile::GetRandomValidationValue -- CRC32 (init 0, no final xor) of the system time, retried while <= 1.
#include <cstdio>
static unsigned long g_table[256];
static int g_calls = 0;
static void FakeGetSystemTime(unsigned char* t) { for (int i = 0; i < 16; ++i) t[i] = (unsigned char)(g_calls ? i + 1 : 0); ++g_calls; }
static unsigned long GetRandomValidationValue()
{
    unsigned long value;
    do {
        unsigned char time[16];
        FakeGetSystemTime(time);
        value = 0;
        for (unsigned long i = 0; i < 16; ++i) value = g_table[time[i] ^ (value & 0xff)] ^ (value >> 8);
    } while (value <= 1);
    return value;
}
int main()
{
    for (unsigned long n = 0; n < 256; ++n) { unsigned long c = n; for (int k = 0; k < 8; ++k) c = (c & 1) ? 0xEDB88320UL ^ (c >> 1) : c >> 1; g_table[n] = c & 0xffffffffUL; }
    if (g_table[1] != 0x77073096UL) { std::printf("009cc0b0_TEST FAIL\n"); return 1; }
    unsigned long v = GetRandomValidationValue();
    // reference: bitwise CRC of bytes 1..16, init 0, no final xor
    unsigned long ref = 0;
    for (int i = 1; i <= 16; ++i) { ref ^= (unsigned long)i; for (int k = 0; k < 8; ++k) ref = (ref & 1) ? 0xEDB88320UL ^ (ref >> 1) : ref >> 1; }
    if (g_calls != 2 || v != ref || v <= 1) { std::printf("009cc0b0_TEST FAIL\n"); return 1; }
    std::printf("009cc0b0_TEST PASS\n");
    return 0;
}