#include <cstdio>

struct SSortedItem { long m_sortKey; };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

static SSortedItem** __copy_backward(SSortedItem** first, SSortedItem** last, SSortedItem** result, const random_access_iterator_tag&, int*)
{
    for (int n = int(last - first); n > 0; --n)
        *--result = *--last;
    return result;
}
static SSortedItem** CopyBackward(SSortedItem** first, SSortedItem** last, SSortedItem** result)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0);
}

int main()
{
    SSortedItem items[3];
    SSortedItem* src[3] = { &items[0], &items[1], &items[2] };
    SSortedItem* dst[5] = { 0, 0, 0, 0, 0 };
    SSortedItem** r = CopyBackward(src, src + 3, dst + 5);
    bool ok = r == dst + 2 && dst[0] == 0 && dst[1] == 0 && dst[2] == &items[0] && dst[3] == &items[1] && dst[4] == &items[2];
    if (CopyBackward(src, src, dst + 1) != dst + 1) ok = false;
    if (!ok) { std::printf("007108a0_TEST FAIL\n"); return 1; }
    std::printf("007108a0_TEST PASS\n");
    return 0;
}