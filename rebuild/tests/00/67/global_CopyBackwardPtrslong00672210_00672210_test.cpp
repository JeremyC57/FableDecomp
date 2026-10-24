#include <cstdio>

struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

static long* __copy_backward(const long* first, const long* last, long* result, const random_access_iterator_tag&, int*)
{
    for (int n = int(last - first); n > 0; --n)
        *--result = *--last;
    return result;
}
static long* CopyBackwardPtrs(const long* first, const long* last, long* result, const __false_type&)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0);
}

int main()
{
    long a[6] = { 1, 2, 3, 4, 5, 0 };
    __false_type tag;
    long* r = CopyBackwardPtrs(a + 1, a + 5, a + 6, tag);      // shift [1,5) up one (the insert step's use)
    bool ok = r == a + 2 && a[0] == 1 && a[1] == 2 && a[2] == 2 && a[3] == 3 && a[4] == 4 && a[5] == 5;
    if (CopyBackwardPtrs(a, a, a + 3, tag) != a + 3 || a[2] != 2) ok = false;   // empty range
    if (!ok) { std::printf("00672210_TEST FAIL\n"); return 1; }
    std::printf("00672210_TEST PASS\n");
    return 0;
}