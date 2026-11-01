// Behaviour test for 0x009CEC60 CBankFile::Update: the per-entry CRC fold over the source-file checksums and the changed-entry decision.
#include <cstdio>
static unsigned long table[256];
static void init()
{
    for (unsigned long n = 0; n < 256; ++n) {
        unsigned long c = n;
        for (int k = 0; k < 8; ++k) c = (c & 1) ? 0xEDB88320UL ^ (c >> 1) : c >> 1;
        table[n] = c;
    }
}
static unsigned long fold(const unsigned long* crcs, int n)
{
    unsigned long checksum = 0;
    for (int i = 0; i < n; ++i) {
        unsigned long crc = crcs[i];
        for (int b = 0; b < 4; ++b)
            checksum = table[((crc >> (8 * b)) & 0xff) ^ (checksum & 0xff)] ^ (checksum >> 8);
    }
    return checksum;
}
int main()
{
    init();
    unsigned long a[2] = { 0x12345678UL, 0x9abcdef0UL };
    unsigned long b[2] = { 0x12345678UL, 0x9abcdef1UL };
    unsigned long stored = fold(a, 2);
    bool ok = fold(a, 2) == stored && fold(b, 2) != stored && fold(a, 0) == 0;
    std::printf(ok ? "009cec60_TEST PASS\n" : "009cec60_TEST FAIL\n");
    return ok ? 0 : 1;
}
