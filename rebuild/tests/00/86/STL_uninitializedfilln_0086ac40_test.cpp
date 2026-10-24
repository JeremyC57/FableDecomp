// Behaviour test for 0x0086AC40: __uninitialized_fill_n copies the skeletal-morph map entry into n slots and returns first + n.
#include <cstdio>
#include <new>

struct CSkeletalMorph { int GroupID; int NameTablePos; unsigned int AllowVariationWord; };

class CSkeletalMorphPair
{
public:
    int              Index0;
    CSkeletalMorph   Tail;
    CSkeletalMorphPair() { }
    CSkeletalMorphPair(const CSkeletalMorphPair& o) : Index0(o.Index0), Tail(o.Tail) { }
};

struct false_type_tag { };

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter uninitialized_fill_n_model(_ForwardIter __first, _Size __n, const _Tp& __x, const false_type_tag&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        new (&*__cur) _Tp(__x);
    return __cur;
}

int main()
{
    CSkeletalMorphPair slots[5];
    for (int i = 0; i < 5; ++i) { slots[i].Index0 = -1; slots[i].Tail.GroupID = -1; slots[i].Tail.NameTablePos = 0; slots[i].Tail.AllowVariationWord = 0; }
    CSkeletalMorphPair x; x.Index0 = 7; x.Tail.GroupID = 8; x.Tail.NameTablePos = 0x00020001u; x.Tail.AllowVariationWord = 0x00040003u;
    int fails = 0;
    if (sizeof(CSkeletalMorphPair) != 16u) ++fails;
    CSkeletalMorphPair* end = uninitialized_fill_n_model(slots, 3u, x, false_type_tag());
    if (end != slots + 3) ++fails;
    for (int i = 0; i < 3; ++i)
        if (slots[i].Index0 != 7 || slots[i].Tail.GroupID != 8 || slots[i].Tail.NameTablePos != 0x00020001u || slots[i].Tail.AllowVariationWord != 0x00040003u) ++fails;
    if (slots[3].Index0 != -1) ++fails;
    if (uninitialized_fill_n_model(slots, 0u, x, false_type_tag()) != slots) ++fails;
    if (fails) { std::printf("0086ac40_TEST FAIL\n"); return 1; }
    std::printf("0086ac40_TEST PASS\n");
    return 0;
}