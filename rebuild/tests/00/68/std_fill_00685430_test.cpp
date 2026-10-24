// Behaviour test for 0x00685430 std::fill<bool*, bool>: every flag of [first, last) takes the value, the bytes around
// the range are untouched, and an empty range writes nothing.
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif

namespace nav {
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for (; __first != __last; ++__first)
        *__first = __val;
}
}

int main()
{
    bool mask[8] = { false, false, false, false, false, false, false, false };
    bool on = true;
    nav::fill(mask + 2, mask + 6, on);
    for (int i = 0; i < 8; ++i)
        if (mask[i] != (i >= 2 && i < 6)) { std::printf("00685430_TEST FAIL\n"); return 1; }
    bool off = false;
    nav::fill(mask + 3, mask + 3, off);
    if (!mask[3]) { std::printf("00685430_TEST FAIL\n"); return 1; }
    nav::fill(mask, mask + 8, off);
    for (int i = 0; i < 8; ++i) if (mask[i]) { std::printf("00685430_TEST FAIL\n"); return 1; }
    std::printf("00685430_TEST PASS\n");
    return 0;
}