#include <cstdio>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#endif
// Test copy of the 0x00b6bba0 model: STLport __uninitialized_fill_n over a trivially copyable 14-dword record.
struct CLandscapeRecord {
    unsigned long Words[14];
};
CLandscapeRecord* __fastcall __uninitialized_fill_n(CLandscapeRecord* first, unsigned int n, const CLandscapeRecord& x)
{
    for (; n > 0; --n, ++first)
        if (first != 0)
            new (first) CLandscapeRecord(x);
    return first;
}
int main()
{
    CLandscapeRecord value;
    CLandscapeRecord slots[5];
    unsigned int i = 0;
    for (i = 0; i < 14; ++i) value.Words[i] = 100 + i;
    for (i = 0; i < 14; ++i) { slots[0].Words[i] = 0; slots[1].Words[i] = 0; slots[2].Words[i] = 0; slots[3].Words[i] = 7; slots[4].Words[i] = 7; }
    CLandscapeRecord* end = __uninitialized_fill_n(slots, 3, value);
    bool ok = end == slots + 3;
    for (i = 0; i < 14; ++i) {
        if (slots[0].Words[i] != 100 + i || slots[2].Words[i] != 100 + i) ok = false;
        if (slots[3].Words[i] != 7) ok = false;                 // nothing written past n
    }
    if (__uninitialized_fill_n(slots + 4, 0, value) != slots + 4 || slots[4].Words[0] != 7) ok = false;   // n == 0 writes nothing
    if (!ok) { std::printf("00b6bba0_TEST FAIL\n"); return 1; }
    std::printf("00b6bba0_TEST PASS\n");
    return 0;
}