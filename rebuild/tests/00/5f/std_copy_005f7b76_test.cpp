// Behaviour test for 0x005F7B76 std::__copy(EHeroAbility*, EHeroAbility*, EHeroAbility*, random_access_iterator_tag, int*).
#include <cstdio>

enum EHeroAbility { ABILITY_NULL = 0, ABILITY_A = 3, ABILITY_B = 7, ABILITY_C = 11, EHeroAbility_Force32 = 0x7fffffff };

namespace stl {
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class RandomAccessIter, class OutputIter, class Distance>
OutputIter __copy(RandomAccessIter first, RandomAccessIter last, OutputIter result,
                  const random_access_iterator_tag&, Distance*)
{
    for (Distance n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}
}

int main()
{
    EHeroAbility src[3] = { ABILITY_A, ABILITY_B, ABILITY_C };
    EHeroAbility dst[5] = { ABILITY_NULL, ABILITY_NULL, ABILITY_NULL, ABILITY_NULL, ABILITY_NULL };
    EHeroAbility* end = stl::__copy(src, src + 3, dst + 1, stl::random_access_iterator_tag(), (int*)0);
    int ok = end == dst + 4 && dst[0] == ABILITY_NULL && dst[1] == ABILITY_A && dst[2] == ABILITY_B &&
             dst[3] == ABILITY_C && dst[4] == ABILITY_NULL;
    // erase-style overlapping forward move: shift [2, 5) down to [0, 3)
    EHeroAbility v[5] = { ABILITY_NULL, ABILITY_A, ABILITY_B, ABILITY_C, ABILITY_A };
    end = stl::__copy(v + 2, v + 5, v, stl::random_access_iterator_tag(), (int*)0);
    if (end != v + 3 || v[0] != ABILITY_B || v[1] != ABILITY_C || v[2] != ABILITY_A) ok = 0;
    // empty range leaves the result unmoved
    if (stl::__copy(src, src, dst, stl::random_access_iterator_tag(), (int*)0) != dst) ok = 0;
    if (!ok) { std::printf("005f7b76_TEST FAIL\n"); return 1; }
    std::printf("005f7b76_TEST PASS\n");
    return 0;
}