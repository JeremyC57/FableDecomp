// Behaviour test for 0x00AE5730 std::__uninitialized_copy over 16-byte elements -- copy-construct [first,last) at result, return the end
#include <cstdio>
#include <new>
struct Elem { long a, b, c, d; };
template <class In, class Out> Out uninitialized_copy(In first, In last, Out result)
{
    Out cur = result;
    for (; first != last; ++first, ++cur)
        new ((void*)&*cur) Elem(*first);
    return cur;
}
int main()
{
    Elem src[3];
    for (int i = 0; i < 3; ++i) { src[i].a = i; src[i].b = i * 10; src[i].c = -i; src[i].d = 7; }
    static unsigned char raw[4 * sizeof(Elem)];
    Elem* dst = (Elem*)raw;
    const Elem* csrc = src;
    Elem* end = uninitialized_copy(csrc, csrc + 3, dst);
    if (end != dst + 3) { std::printf("00ae5730_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i)
        if (dst[i].a != i || dst[i].b != i * 10 || dst[i].c != -i || dst[i].d != 7) { std::printf("00ae5730_TEST FAIL\n"); return 1; }
    if (uninitialized_copy(csrc, csrc, dst) != dst) { std::printf("00ae5730_TEST FAIL\n"); return 1; }
    std::printf("00ae5730_TEST PASS\n");
    return 0;
}