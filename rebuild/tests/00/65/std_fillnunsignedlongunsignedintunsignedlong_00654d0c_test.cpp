// behaviour test for 0x00654D0C
#include <stdio.h>
namespace std {
template <class OutputIter, class Size, class T> inline
OutputIter __fastcall fill_n(OutputIter first, Size n, const T& val)
{
    for (; n > 0; --n, ++first)
        *first = val;
    return first;
}
template unsigned long* __fastcall fill_n<unsigned long*, unsigned int, unsigned long>(unsigned long*, unsigned int, const unsigned long&);
}

int main()
{
    unsigned long buf[6] = {1, 1, 1, 1, 1, 1}; unsigned long v = 0xDEADBEEF; int ok = 1;
    unsigned long* e = std::fill_n(buf + 1, 3u, v);
    ok &= e == buf + 4 && buf[0] == 1 && buf[1] == v && buf[2] == v && buf[3] == v && buf[4] == 1;
    e = std::fill_n(buf, 0u, v); ok &= e == buf && buf[0] == 1;
    printf(ok ? "00654d0c_TEST PASS\n" : "00654d0c_TEST FAIL\n");
    return ok ? 0 : 1;
}