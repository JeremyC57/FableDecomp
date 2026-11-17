// Behaviour test for 0x009F63B0 std::__copy_ptrs<CInputEvent*,CInputEvent*>: element-wise assignment of [first, last) onto
// result (forward), returning result + (last - first); an empty range returns result untouched.
#include <cstdio>

class CInputEvent {
public:
    long m_words[13];
};

struct __false_type { __false_type() {} };
struct random_access_iterator_tag { random_access_iterator_tag() {} };

template <class RandomAccessIter, class OutputIter, class Distance>
OutputIter __copy(RandomAccessIter first, RandomAccessIter last, OutputIter result, const random_access_iterator_tag&, Distance*)
{
    for (Distance n = (Distance)(last - first); n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

template <class InputIter, class OutputIter>
OutputIter __copy_ptrs(InputIter first, InputIter last, OutputIter result, const __false_type&)
{
    return __copy(first, last, result, random_access_iterator_tag(), (int*)0);
}

int main()
{
    CInputEvent src[3], dst[4];
    for (int i = 0; i < 3; ++i) for (int w = 0; w < 13; ++w) src[i].m_words[w] = i * 100 + w;
    for (int i = 0; i < 4; ++i) for (int w = 0; w < 13; ++w) dst[i].m_words[w] = -1;
    CInputEvent* end = __copy_ptrs(src, src + 3, dst, __false_type());
    bool ok = end == dst + 3 && dst[0].m_words[0] == 0 && dst[2].m_words[12] == 212 && dst[3].m_words[0] == -1;
    ok = ok && __copy_ptrs(src, src, dst + 1, __false_type()) == dst + 1;
    if (!ok) { std::printf("009f63b0_TEST FAIL\n"); return 1; }
    std::printf("009f63b0_TEST PASS\n");
    return 0;
}