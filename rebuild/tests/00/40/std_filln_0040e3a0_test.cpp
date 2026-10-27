// behaviour test for 0x0040e3a0 std::fill_n<unsigned int>(unsigned char* first, n, const unsigned char& v): memset n bytes, return first + n
#include <cstdio>
#include <cstring>
static unsigned char* FillN(unsigned char* first, unsigned int n, const unsigned char& v)
{
    unsigned char tmp = v;
    std::memset(first, tmp, n);
    return first + n;
}
int main()
{
    int fails = 0;
    unsigned char buf[10];
    std::memset(buf, 0, sizeof(buf));
    unsigned char v = 0xAB;
    unsigned char* e = FillN(buf + 2, 5, v);
    if (e != buf + 7) fails |= 1;
    for (int i = 0; i < 10; ++i) if (buf[i] != ((i >= 2 && i < 7) ? 0xAB : 0)) fails |= 2;
    if (FillN(buf, 0, v) != buf || buf[0] != 0) fails |= 4;
    if (fails) { std::printf("0040e3a0_TEST FAIL %d\n", fails); return 1; }
    std::printf("0040e3a0_TEST PASS\n");
    return 0;
}