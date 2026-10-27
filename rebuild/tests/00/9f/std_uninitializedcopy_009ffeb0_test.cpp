#include <cstdio>
#include <new>
// Behaviour model: placement copy-construct {4-byte scalar, counted member} records into raw storage, return the end.
struct M { int v; static int copies; M(int x) : v(x) { } M(const M& o) : v(o.v) { ++copies; } };
int M::copies = 0;
struct R { int first; M second; R(int a, int b) : first(a), second(b) { } };
static R* uninit_copy(const R* first, const R* last, R* result)
{
    R* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) R(*first);
    return cur;
}
int main()
{
    R src[3] = { R(1, 10), R(2, 20), R(3, 30) };
    char raw[sizeof(R) * 3];
    R* dst = (R*)raw;
    M::copies = 0;
    R* e = uninit_copy(src, src + 3, dst);
    if (e != dst + 3 || M::copies != 3 || dst[0].first != 1 || dst[2].second.v != 30) { std::printf("009ffeb0_TEST FAIL\n"); return 1; }
    if (uninit_copy(src, src, dst) != dst || M::copies != 3) { std::printf("009ffeb0_TEST FAIL\n"); return 1; }
    std::printf("009ffeb0_TEST PASS\n");
    return 0;
}