// std::__unguarded_insertion_sort_aux(pair<unsigned long, CSoundPair>* first, pair<unsigned long, CSoundPair>* last, pair<..>*, CKeyPairCompareLess<unsigned long, CSoundPair> comp) -- STLport unguarded insertion sort of [first, last) by key
// Leaf: __unguarded_insertion_sort_aux
// Retail: Fable.exe @ 0x00470D77, 69 bytes, `ret 8`: __fastcall (first ecx, last edx; the null value-type pointer and
// the empty comparator by value on the stack). Only call site 0x00470D64 in STLport __final_insertion_sort at 0x00470D36
// (catalogued make_heap<pair<unsigned long, CSoundPair>*, CKeyPairCompareLess<unsigned long, CSoundPair> >): for more than
// 16 elements it runs __insertion_sort 0x00470DBC over the first 16 and then this over [first + 16, last), pushing
// `0` for the value-type pointer and the comparator word.
// Identity: the catalog's `_Fill<CEngineCamera::CDebugScriptElement*>` is propagated; the element is the 12-byte
// pair<unsigned long, CSoundPair> (CSoundPair = FirstSound, LastSound) ordered by CKeyPairCompareLess on the key
// (`cmp ebx,[eax] ; jb`: unsigned).
// Shape: each trip inlines __unguarded_linear_insert(i, T(*i), comp): the by-value copy of *i in a 12-byte frame local
// (key kept in ebx), the shifting `*last = *next` as `movsd` x3, then `*last = val`. The size-optimised frame form
// (`push ebp ; mov ebp,esp ... leave`, unaligned entry, loop test reached by `jmp`) is `optimize("y", off)` +
// `optimize("s", on)` (GOTCHAS: frame body that still uses esi/edi freely).
#pragma optimize("y", off)
#pragma optimize("s", on)
class CSoundPair { public:                             // 8 bytes
    long FirstSound;                            // +0x00
    long LastSound;                             // +0x04
};

namespace std {
template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
    pair(const pair<T1, T2>& o) : first(o.first), second(o.second) {}
};
}

template <class K, class V>
class CKeyPairCompareLess { public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

namespace std {
template <class RandomAccessIter, class T, class Compare>
__forceinline void __unguarded_linear_insert(RandomAccessIter last, T val, Compare comp)
{
    RandomAccessIter next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}
template <class RandomAccessIter, class T, class Compare>
void __fastcall __unguarded_insertion_sort_aux(RandomAccessIter first, RandomAccessIter last, T*, Compare comp)
{
    for (RandomAccessIter i = first; i != last; ++i)
        __unguarded_linear_insert(i, T(*i), comp);
}
typedef pair<unsigned long, CSoundPair> CSoundEntry;
template void __fastcall __unguarded_insertion_sort_aux(CSoundEntry*, CSoundEntry*, CSoundEntry*,
                                                        CKeyPairCompareLess<unsigned long, CSoundPair>);
}