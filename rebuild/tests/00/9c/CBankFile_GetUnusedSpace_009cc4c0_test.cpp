#include <cstdio>

struct B { unsigned char pad[0xa0]; unsigned long HeaderSize; unsigned long FileInformationSize; unsigned long FileSize;
    unsigned long GetTotalEntryDataSize() { return 10; }
    unsigned long GetUnusedSpace() { return FileSize - FileInformationSize - HeaderSize - GetTotalEntryDataSize(); } };
int main()
{
    B b; b.FileSize = 100; b.FileInformationSize = 20; b.HeaderSize = 5;
    if (b.GetUnusedSpace() != 65) { std::printf("009cc4c0_TEST FAIL\n"); return 1; }
    std::printf("009cc4c0_TEST PASS\n");
    return 0;
}