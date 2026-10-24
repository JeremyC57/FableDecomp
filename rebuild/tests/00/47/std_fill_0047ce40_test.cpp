// Behaviour test for 0x0047CE40 std::fill(unsigned char*, unsigned char*, const unsigned char&) (model copy).
#include <cstdio>
#include <string.h>

static void fill_model(unsigned char* first, unsigned char* last, const unsigned char& val)
{
    unsigned char tmp = val;
    memset(first, tmp, last - first);
}

int main()
{
    unsigned char buf[16];
    for (int i = 0; i < 16; ++i) buf[i] = (unsigned char)i;
    unsigned char v = 0xAB;
    fill_model(buf + 1, buf + 12, v);       // 11 bytes: two dwords + three tail bytes
    int ok = buf[0] == 0 && buf[12] == 12 && buf[15] == 15;
    for (int j = 1; j < 12; ++j) if (buf[j] != 0xAB) ok = 0;
    fill_model(buf + 5, buf + 5, v);        // empty range writes nothing
    if (buf[5] != 0xAB) ok = 0;
    if (!ok) { std::printf("0047ce40_TEST FAIL\n"); return 1; }
    std::printf("0047ce40_TEST PASS\n");
    return 0;
}