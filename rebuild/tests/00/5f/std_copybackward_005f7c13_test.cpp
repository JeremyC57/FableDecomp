// Behaviour test for 0x005F7C13 std::__copy_backward(EHeroAbility*, EHeroAbility*, EHeroAbility*, random_access_iterator_tag, int*).
#include <cstdio>

enum EHeroAbility { ABILITY_NULL = 0, ABILITY_A = 3, ABILITY_B = 7, ABILITY_C = 11, EHeroAbility_Force32 = 0x7fffffff };

namespace stl {
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class RandomAccessIter, class BidirectionalIter, class Distance>
BidirectionalIter __copy_backward(RandomAccessIter first, RandomAccessIter last, BidirectionalIter result,
                                  const random_access_iterator_tag&, Distance*)
{
    for (Distance n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}
}

int main()
{
    // insert-style overlapping move: shift [0, 3) up by two into [2, 5)
    EHeroAbility v[5] = { ABILITY_A, ABILITY_B, ABILITY_C, ABILITY_NULL, ABILITY_NULL };
    EHeroAbility* begin = stl::__copy_backward(v, v + 3, v + 5, stl::random_access_iterator_tag(), (int*)0);
    int ok = begin == v + 2 && v[2] == ABILITY_A && v[3] == ABILITY_B && v[4] == ABILITY_C && v[0] == ABILITY_A;
    EHeroAbility d[2] = { ABILITY_NULL, ABILITY_NULL };
    if (stl::__copy_backward(v, v, d + 2, stl::random_access_iterator_tag(), (int*)0) != d + 2) ok = 0;
    if (!ok) { std::printf("005f7c13_TEST FAIL\n"); return 1; }
    std::printf("005f7c13_TEST PASS\n");
    return 0;
}