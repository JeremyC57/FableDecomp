// std::__unguarded_insertion_sort_aux(pair<EReactionSpeechType, long>* first, pair<EReactionSpeechType, long>* last, pair<EReactionSpeechType, long>*, CKeyPairCompareLess<EReactionSpeechType, long> comp) -- STLport unguarded insertion pass over CSimVoice's voice entries, ordered by speech type, the linear insert inline
// Leaf: ??$__unguarded_insertion_sort_aux@PAV?$pair@W4EReactionSpeechType@@J@std@@
//
// Retail 0x00680700, 73 bytes (+ int3 padding to 80), `ret 8` (/Gr: first in ecx, last in edx; the _Tp* tag and the
//   empty comparator on the stack); the value's two words ride in edi/ebx.
// Callers: 0x006813F9 in sort<pair<EReactionSpeechType, long>*, CKeyPairCompareLess<..> > 0x006813A0 (landed; the
//   __final_insertion_sort tail), which the landed CSimVoice::Add 0x00680A40 calls on CSimVoice::Voices; also
//   0x00680BA7 and 0x006810BC.
// Layout: Ego_r.pdb CSimVoice::Voices = CVectorMap<EReactionSpeechType, long, CKeyPairCompareLess<EReactionSpeechType,
//   long> >, entries pair<EReactionSpeechType, long> (8 bytes); the key compare is `a.first < b.first` (signed `jl`).
// Shape: STLport _algo.c `for (i = first; i != last; ++i) __unguarded_linear_insert(i, _Tp(*i), comp);` with the
//   linear insert (`next = last; --next; while (comp(val, *next)) { *last = *next; last = next; --next; } *last = val;`)
//   inline (__forceinline); STLport's pair declares its own copy constructor.  /Gr TU written as explicit __fastcall.
// Label: catalog "_Copy_impl<CNavigationPosition*,CNavigationPosition*>" is propagated.
enum EReactionSpeechType { REACTION_SPEECH_TYPE_NONE = -1, REACTION_SPEECH_TYPE_0 = 0 };

namespace std {
template <class _T1, class _T2> class pair {
public:
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    pair(const pair& __p) : first(__p.first), second(__p.second) {}
};
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

namespace std {
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

typedef std::pair<EReactionSpeechType, long> CVoiceEntry;
template void __fastcall std::__unguarded_insertion_sort_aux<CVoiceEntry*, CVoiceEntry, CKeyPairCompareLess<EReactionSpeechType, long> >(
    CVoiceEntry*, CVoiceEntry*, CVoiceEntry*, CKeyPairCompareLess<EReactionSpeechType, long>);