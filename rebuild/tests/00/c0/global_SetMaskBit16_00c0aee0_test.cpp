// behaviour test for 0x00C0AEE0 SetMaskBit16: SetMaskBit over a 16-bit mask
#include <cstdio>

static int g_calls = 0;
static unsigned long g_lastCount = 0;
void SetMaskBit(unsigned long index, bool set, unsigned short* pMask, unsigned short* pTouched, unsigned long bitCount)
{
    ++g_calls;
    g_lastCount = bitCount;
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

void SetMaskBit16(unsigned long index, bool set, unsigned short* pMask, unsigned short* pTouched)
{
    SetMaskBit(index, set, pMask, pTouched, 16);
}

int main()
{
    unsigned short mask = 0, touched = 0;
    SetMaskBit16(2, true, &mask, &touched);
    if (g_calls != 1 || g_lastCount != 16 || mask != 4 || touched != 4) { std::printf("00c0aee0_TEST FAIL\n"); return 1; }
    SetMaskBit16(16 - 1, true, &mask, &touched);
    if (mask != (unsigned short)(4 | (1 << (16 - 1)))) { std::printf("00c0aee0_TEST FAIL\n"); return 1; }
    SetMaskBit16(16, true, &mask, &touched);                     // first index past the mask: ignored
    if (mask != (unsigned short)(4 | (1 << (16 - 1)))) { std::printf("00c0aee0_TEST FAIL\n"); return 1; }
    SetMaskBit16(2, false, &mask, &touched);
    if (mask != (unsigned short)(1 << (16 - 1)) || g_calls != 4) { std::printf("00c0aee0_TEST FAIL\n"); return 1; }
    std::printf("00c0aee0_TEST PASS\n");
    return 0;
}