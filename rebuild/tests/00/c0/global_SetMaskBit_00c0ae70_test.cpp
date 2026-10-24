// behaviour test for 0x00C0AE70 SetMaskBit: set/clear a bit of an optional 16-bit mask, OR it into an optional touched mask
#include <cstdio>

void SetMaskBit(unsigned long index, bool set, unsigned short* pMask, unsigned short* pTouched, unsigned long bitCount)
{
    if (index < bitCount) {
        unsigned short bit = (unsigned short)(1 << index);
        if (pTouched)
            *pTouched |= bit;
        if (pMask) {
            if (set)
                *pMask |= bit;
            else
                *pMask &= ~bit;
        }
    }
}

int main()
{
    unsigned short mask = 0x00F0, touched = 0;
    SetMaskBit(1, true, &mask, &touched, 16);
    if (mask != 0x00F2 || touched != 0x0002) { std::printf("00c0ae70_TEST FAIL\n"); return 1; }
    SetMaskBit(4, false, &mask, &touched, 16);
    if (mask != 0x00E2 || touched != 0x0012) { std::printf("00c0ae70_TEST FAIL\n"); return 1; }
    SetMaskBit(15, true, &mask, 0, 16);
    if (mask != 0x80E2 || touched != 0x0012) { std::printf("00c0ae70_TEST FAIL\n"); return 1; }
    SetMaskBit(8, true, &mask, &touched, 8);                    // out of range: nothing changes
    if (mask != 0x80E2 || touched != 0x0012) { std::printf("00c0ae70_TEST FAIL\n"); return 1; }
    SetMaskBit(3, true, 0, &touched, 8);
    if (touched != 0x001A) { std::printf("00c0ae70_TEST FAIL\n"); return 1; }
    std::printf("00c0ae70_TEST PASS\n");
    return 0;
}