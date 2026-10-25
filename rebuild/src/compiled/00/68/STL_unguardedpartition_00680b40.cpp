// _STL::__unguarded_partition(pair<EReactionSpeechType, long>* first, pair<EReactionSpeechType, long>* last, pair<EReactionSpeechType, long> pivot, CKeyPairCompareLess<EReactionSpeechType, long> comp) -- STLport quicksort partition of CSimVoice's voice entries around a pivot key, swapping out-of-place pairs
// Leaf: ??$__unguarded_partition@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00680B40, length 85 bytes, `ret 0Ch`.
// Callers: __introsort_loop 0x00681270 (0x006812EA).
// Callees: none (comparator and iter_swap inline).
// Label: catalog "std::partition_pairs" is descriptive; STLport spelling __unguarded_partition.
// Layout: Ego_r.pdb CSimVoice::Voices = CVectorMap<EReactionSpeechType, long, CKeyPairCompareLess<EReactionSpeechType,
//   long> >, entries pair<EReactionSpeechType, long> (8 bytes; STLport's pair declares its copy constructor); the key
//   compare is `a.first < b.first` (signed). The same model as the landed __unguarded_insertion_sort_aux 0x00680700.
// Shape: STLport `_algo.c` verbatim (`for (;;) { while (__comp(*__first, __pivot)) ++__first; --__last; while
//   (__comp(__pivot, *__last)) --__last; if (!(__first < __last)) return __first; iter_swap(__first, __last);
//   ++__first; }`) with iter_swap's `_Tp __tmp = *__a; *__a = *__b; *__b = __tmp;` inline (the copy constructor builds
//   the temporary in esi/edi). /Gr algorithm TU (`__fastcall` template: first in ecx, last in edx, the 8-byte pivot and
//   the empty comparator on the stack); only the pivot key is read (held in ecx).
enum EReactionSpeechType { REACTION_SPEECH_TYPE_NONE = -1, REACTION_SPEECH_TYPE_0 = 0 };

namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __p) : first(__p.first), second(__p.second) { }
};
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const { return a.first < b.first; }
};

typedef _STL::pair<EReactionSpeechType, long> TVoiceEntry;
typedef CKeyPairCompareLess<EReactionSpeechType, long> TVoiceLess;

namespace _STL {
template <class _Tp> inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }

template <class _ForwardIter1, class _ForwardIter2, class _Tp>
inline void __iter_swap(_ForwardIter1 __a, _ForwardIter2 __b, _Tp*)
{
    _Tp __tmp = *__a;
    *__a = *__b;
    *__b = __tmp;
}
template <class _ForwardIter1, class _ForwardIter2>
inline void iter_swap(_ForwardIter1 __a, _ForwardIter2 __b)
{
    __iter_swap(__a, __b, __value_type(__a));
}

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp)
{
    for (;;) {
        while (__comp(*__first, __pivot))
            ++__first;
        --__last;
        while (__comp(__pivot, *__last))
            --__last;
        if (!(__first < __last))
            return __first;
        iter_swap(__first, __last);
        ++__first;
    }
}
}

template TVoiceEntry* __fastcall _STL::__unguarded_partition<TVoiceEntry*, TVoiceEntry, TVoiceLess>(TVoiceEntry*, TVoiceEntry*, TVoiceEntry, TVoiceLess);