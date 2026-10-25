// _STL::__partial_sort(pair<EReactionSpeechType, long>* first, pair<EReactionSpeechType, long>* middle, pair<EReactionSpeechType, long>* last, pair<EReactionSpeechType, long>*, CKeyPairCompareLess<EReactionSpeechType, long> comp) -- STLport partial_sort core over CSimVoice's voice entries: heapify [first, middle), pop every smaller later entry into the heap, then sort the heap
// Leaf: ??$__partial_sort@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006810E0, length 107 bytes (+ int3 padding), `ret 0Ch`.
// Callers: partial_sort 0x006811AC and __introsort_loop 0x00681270 (0x00681328, the depth-limit fallback).
// Callees: __make_heap(first, last, comp, T*, int*) @ 0x00680C20, __adjust_heap(first, holeIndex, len, T value, comp)
//   @ 0x00680750 (landed), sort_heap(first, last, comp) @ 0x00680E00 (landed).
// Label: catalog "std::partial_sort_heap" is descriptive; the body is STLport 4.x `__partial_sort` verbatim
//   (`make_heap(__first, __middle, __comp); for (__i = __middle; __i < __last; ++__i) if (__comp(*__i, *__first))
//   __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, __DISTANCE_TYPE(__first, _RandomAccessIter));
//   sort_heap(__first, __middle, __comp);`).
// Layout: Ego_r.pdb CSimVoice::Voices = CVectorMap<EReactionSpeechType, long, CKeyPairCompareLess<EReactionSpeechType,
//   long> >, entries pair<EReactionSpeechType, long> (8 bytes; STLport's pair declares its copy constructor); the key
//   compare is `a.first < b.first` (signed). The same model as the landed __unguarded_insertion_sort_aux 0x00680700.
// Shape: /Gr algorithm TU (`__fastcall` template: first in ecx, middle in edx; last, the value-type tag and the empty
//   comparator on the stack); make_heap is the inline forwarder (two null tags pushed before the comparator),
//   __pop_heap is inline (`__forceinline`: `*__result = *__first; __adjust_heap(__first, 0, __last - __first, __val,
//   __comp)`) with the value's two words read before the store and rebuilt in the argument area.
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
template <class _RandomAccessIter, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp, _Tp*, _Distance*);   // 0x00680C20
template <class _RandomAccessIter, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIter __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp);   // 0x00680750
template <class _RandomAccessIter, class _Compare>
void __fastcall sort_heap(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x00680E00

template <class _Tp> inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }
template <class _Tp> inline int* __distance_type(const _Tp*) { return (int*)0; }

template <class _RandomAccessIter, class _Compare>
inline void make_heap(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    __make_heap(__first, __last, __comp, __value_type(__first), __distance_type(__first));
}

template <class _RandomAccessIter, class _Tp, class _Compare, class _Distance>
__forceinline void __pop_heap(_RandomAccessIter __first, _RandomAccessIter __last, _RandomAccessIter __result, _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    make_heap(__first, __middle, __comp);
    for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
        if (__comp(*__i, *__first))
            __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, __distance_type(__first));
    sort_heap(__first, __middle, __comp);
}
}

template void __fastcall _STL::__partial_sort<TVoiceEntry*, TVoiceEntry, TVoiceLess>(TVoiceEntry*, TVoiceEntry*, TVoiceEntry*, TVoiceEntry*, TVoiceLess);