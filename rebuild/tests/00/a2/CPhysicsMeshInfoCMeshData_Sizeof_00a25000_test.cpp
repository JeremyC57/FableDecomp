#include <cstdio>
// Behaviour model: the record size plus each present pointee's size and the pointer objects.
static unsigned long size(bool m, unsigned long ms, bool f, unsigned long fs) { return (m ? ms + 8 : 8) + (f ? fs + 8 : 8) + 0x34; }
int main()
{
    if (size(false, 0, false, 0) != 0x44 || size(true, 100, true, 10) != 0x34 + 108 + 18) { std::printf("00a25000_TEST FAIL\n"); return 1; }
    std::printf("00a25000_TEST PASS\n");
    return 0;
}