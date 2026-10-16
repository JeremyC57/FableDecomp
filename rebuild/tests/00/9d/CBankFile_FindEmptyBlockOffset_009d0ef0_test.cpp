// Behaviour test for 009d0ef0: CBankFile::FindEmptyBlockOffset -- first aligned gap after the header that fits, else the aligned end.
#include <cstdio>
struct TBank {
    unsigned long Alignment, HeaderSize; unsigned long order[4]; int count; unsigned long offs[8], sizes[8]; int sorts;
    void UpdateBlockOrder() { ++sorts; }
    unsigned long AlignOffset(unsigned long v) const { return (v + Alignment - 1) / Alignment * Alignment; }
    unsigned long FindEmptyBlockOffset(unsigned long size)
    {
        UpdateBlockOrder();
        unsigned long offset = AlignOffset(HeaderSize);
        for (int i = 0; i != count; ++i) {
            unsigned long entry = order[i];
            unsigned long blockOffset = offs[entry], blockSize = sizes[entry];
            if (blockOffset - offset >= size) break;
            offset = AlignOffset(blockSize + blockOffset);
        }
        return offset;
    }
};
int main()
{
    TBank b; b.Alignment = 16; b.HeaderSize = 20; b.sorts = 0; b.count = 3;
    b.order[0] = 1; b.order[1] = 2; b.order[2] = 3;
    b.offs[1] = 32; b.sizes[1] = 10; b.offs[2] = 80; b.sizes[2] = 5; b.offs[3] = 96; b.sizes[3] = 40;
    if (b.FindEmptyBlockOffset(8) != 48) { std::printf("009d0ef0_TEST FAIL\n"); return 1; }   // header aligns to 32 (block 1 there), next free 48, 48..80 fits 8
    if (b.FindEmptyBlockOffset(100) != 144) { std::printf("009d0ef0_TEST FAIL\n"); return 1; }
    if (b.FindEmptyBlockOffset(0) != 32 || b.sorts != 3) { std::printf("009d0ef0_TEST FAIL\n"); return 1; }
    std::printf("009d0ef0_TEST PASS\n");
    return 0;
}