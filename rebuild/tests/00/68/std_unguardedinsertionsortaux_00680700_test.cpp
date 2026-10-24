// Behaviour test for 0x00680700 std::__unguarded_insertion_sort_aux<pair<EReactionSpeechType, long>*, ...,
// CKeyPairCompareLess<EReactionSpeechType, long> >: behind a smaller-key guard, [first, last) ends up sorted by speech
// type (signed), entries moved whole, equal keys in their original order.
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif

enum EReactionSpeechType { REACTION_SPEECH_TYPE_NONE = -1, REACTION_SPEECH_TYPE_0 = 0 };

namespace nav {
template <class _T1, class _T2> class pair {
public:
    _T1 first;
    _T2 second;
    pair() {}
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    pair(const pair& __p) : first(__p.first), second(__p.second) {}
};
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const nav::pair<K, V>& a, const nav::pair<K, V>& b) const { return a.first < b.first; }
};

namespace nav {
template <class _RandomAccessIter, class _Tp, class _Compare>
__forceinline void __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    _RandomAccessIter __next = __last;
    --__next;
    while (__comp(__val, *__next)) {
        *__last = *__next;
        __last = __next;
        --__next;
    }
    *__last = __val;
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}
}

typedef nav::pair<EReactionSpeechType, int> Entry;

int main()
{
    Entry a[6];
    a[0] = Entry((EReactionSpeechType)-5, 0);                // guard
    a[1] = Entry((EReactionSpeechType)3, 1); a[2] = Entry((EReactionSpeechType)-1, 2); a[3] = Entry((EReactionSpeechType)3, 3);
    a[4] = Entry((EReactionSpeechType)0, 4); a[5] = Entry((EReactionSpeechType)2, 5);
    nav::__unguarded_insertion_sort_aux(a + 1, a + 6, (Entry*)0, CKeyPairCompareLess<EReactionSpeechType, int>());
    const int keys[6] = { -5, -1, 0, 2, 3, 3 };
    const int vals[6] = { 0, 2, 4, 5, 1, 3 };
    for (int i = 0; i < 6; ++i)
        if (a[i].first != keys[i] || a[i].second != vals[i]) { std::printf("00680700_TEST FAIL\n"); return 1; }
    std::printf("00680700_TEST PASS\n");
    return 0;
}