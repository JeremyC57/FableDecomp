#include <cstdio>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#endif
// Test copy of the 0x009fffc0 model: STLport __uninitialized_fill_n over a trivially copyable 11-dword record.
struct CGraphicBankRecord {
    unsigned long Words[11];
};
CGraphicBankRecord* __fastcall __uninitialized_fill_n(CGraphicBankRecord* first, unsigned int n, const CGraphicBankRecord& x)
{
    for (; n > 0; --n, ++first)
        if (first != 0)
            new (first) CGraphicBankRecord(x);
    return first;
}
int main()
{
    CGraphicBankRecord value;
    CGraphicBankRecord slots[5];
    unsigned int i = 0;
    for (i = 0; i < 11; ++i) value.Words[i] = 100 + i;
    for (i = 0; i < 11; ++i) { slots[0].Words[i] = 0; slots[1].Words[i] = 0; slots[2].Words[i] = 0; slots[3].Words[i] = 7; slots[4].Words[i] = 7; }
    CGraphicBankRecord* end = __uninitialized_fill_n(slots, 3, value);
    bool ok = end == slots + 3;
    for (i = 0; i < 11; ++i) {
        if (slots[0].Words[i] != 100 + i || slots[2].Words[i] != 100 + i) ok = false;
        if (slots[3].Words[i] != 7) ok = false;                 // nothing written past n
    }
    if (__uninitialized_fill_n(slots + 4, 0, value) != slots + 4 || slots[4].Words[0] != 7) ok = false;   // n == 0 writes nothing
    if (!ok) { std::printf("009fffc0_TEST FAIL\n"); return 1; }
    std::printf("009fffc0_TEST PASS\n");
    return 0;
}