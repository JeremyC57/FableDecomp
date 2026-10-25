// Behaviour test for 0x00710DC0 std::__rotate_adaptive over list<CAnimSet>::const_iterator: the right-run-through-
// buffer branch (len1 > len2, len2 fits), the left-run-through-buffer branch (len1 fits) and the in-place rotate
// fallback all yield the rotation and return the new middle
#include <cstdio>

namespace model {

struct CAnimSet { int Priority; int Tag; };
struct ConstIterator { const CAnimSet* Node; };
typedef ConstIterator It;

int g_rotates = 0;

It* copy(It* first, It* last, It* result) { for (int n = (int)(last - first); n > 0; --n) *result++ = *first++; return result; }
It* copy_backward(It* first, It* last, It* result) { for (int n = (int)(last - first); n > 0; --n) *--result = *--last; return result; }
It* rotate(It* first, It* middle, It* last)          // __rotate 0x007103C0 (behaviour only)
{
    ++g_rotates;
    It tmp[32];
    It* e = copy(middle, last, tmp);
    e = copy(first, middle, e);
    copy(tmp, e, first);
    return first + (last - middle);
}

It* rotate_adaptive(It* first, It* middle, It* last, int len1, int len2, It* buffer, int buffer_size)
{
    It* buffer_end;
    if (len1 > len2 && len2 <= buffer_size) {
        buffer_end = copy(middle, last, buffer);
        copy_backward(first, middle, last);
        return copy(buffer, buffer_end, first);
    }
    else if (len1 <= buffer_size) {
        buffer_end = copy(first, middle, buffer);
        copy(middle, last, first);
        return copy_backward(buffer, buffer_end, last);
    }
    else
        return rotate(first, middle, last);
}

}

int main()
{
    using namespace model;
    CAnimSet sets[12];
    for (int i = 0; i < 12; ++i) { sets[i].Priority = i; sets[i].Tag = 100 + i; }
    int bad = 0;
    int expect_rotates = 0;
    for (int bufsize = 0; bufsize <= 12; bufsize += 3)
        for (int n = 1; n <= 12; ++n)
            for (int k = 0; k <= n; ++k) {
                It v[12], buffer[12];
                for (int i = 0; i < n; ++i) v[i].Node = &sets[i];
                int len1 = k, len2 = n - k;
                if (!((len1 > len2 && len2 <= bufsize) || len1 <= bufsize)) ++expect_rotates;
                It* r = rotate_adaptive(v, v + k, v + n, len1, len2, buffer, bufsize);
                if (r != v + (n - k)) bad = 1;
                for (int i = 0; i < n; ++i)
                    if (v[i].Node != &sets[(i + k) % n]) bad = 2;
            }
    if (g_rotates != expect_rotates || expect_rotates == 0) bad = 3;
    if (bad) { std::printf("00710dc0_TEST FAIL\n"); return 1; }
    std::printf("00710dc0_TEST PASS\n");
    return 0;
}