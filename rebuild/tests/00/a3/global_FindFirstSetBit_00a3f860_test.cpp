#include <cstdio>

int FindFirstSetBit(unsigned long mask, unsigned long numBytes)
{
    for (unsigned long i = 0; i < numBytes * 8; ++i)
        if (mask & (1 << i))
            return (int)i;
    return -1;
}

int main()
{
    if (FindFirstSetBit(0x10, 2) != 4 || FindFirstSetBit(0, 4) != -1 || FindFirstSetBit(1, 1) != 0
        || FindFirstSetBit(0x100, 1) != -1 || FindFirstSetBit(0x100, 2) != 8 || FindFirstSetBit(0x40000000, 4) != 30
        || FindFirstSetBit(0x0C00, 4) != 10) {
        std::printf("00a3f860_TEST FAIL\n");
        return 1;
    }
    std::printf("00a3f860_TEST PASS\n");
    return 0;
}