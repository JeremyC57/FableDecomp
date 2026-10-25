// std::__introsort_loop(pair<EReactionSpeechType, long>* first, pair<EReactionSpeechType, long>* last, pair<EReactionSpeechType, long>*, int depth_limit, CKeyPairCompareLess<EReactionSpeechType, long> comp) -- STLport introsort recursion over CSimVoice's voice entries: median-of-three partition, recurse on the right part until ranges reach 16 entries, partial_sort at depth zero
// Leaf: ??$__introsort_loop@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00681270, length 196 bytes (+ int3 padding), `ret 0Ch`.
// Callers: sort 0x006813A0 (landed; 0x006813D8, depth 2*lg(n)) and itself (0x006812FD).
// Callees: __unguarded_partition(first, last, T pivot, comp) @ 0x00680B40, __partial_sort(first, middle, last, T*, comp)
//   @ 0x006810E0 (behind the inline partial_sort), itself.
// Label: catalog "std::IntroSort" is descriptive; STLport spelling __introsort_loop.
// Layout: Ego_r.pdb CSimVoice::Voices = CVectorMap<EReactionSpeechType, long, CKeyPairCompareLess<EReactionSpeechType,
//   long> >, entries pair<EReactionSpeechType, long> (8 bytes; STLport's pair declares its copy constructor); the key
//   compare is `a.first < b.first` (signed). The same model as the landed __unguarded_insertion_sort_aux 0x00680700.
// Shape: STLport 4.6 `_algo.c` verbatim (`while (__last - __first > __stl_threshold) { if (__depth_limit == 0) {
//   partial_sort(__first, __last, __last, __comp); return; } --__depth_limit; __cut = __unguarded_partition(__first,
//   __last, _Tp(__median(*__first, *(__first + (__last - __first)/2), *(__last - 1), __comp)), __comp);
//   __introsort_loop(__cut, __last, (_Tp*)0, __depth_limit, __comp); __last = __cut; }`) with __median inline (it
//   returns a const reference: the pick is an address whose pair is copied into the argument area); the threshold
//   test is the byte form `and eax,-8 ; cmp eax,80h`. /Gr algorithm TU (`__fastcall` template: first in ecx, last in
//   edx, the value-type tag, the depth limit and the empty comparator on the stack).
enum EReactionSpeechType { REACTION_SPEECH_TYPE_NONE = -1, REACTION_SPEECH_TYPE_0 = 0 };

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __p) : first(__p.first), second(__p.second) { }
};
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

typedef std::pair<EReactionSpeechType, long> TVoiceEntry;
typedef CKeyPairCompareLess<EReactionSpeechType, long> TVoiceLess;

namespace std {
template <class _Tp> inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp);   // 0x00680B40
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*, _Compare __comp);   // 0x006810E0

template <class _RandomAccessIter, class _Compare>
inline void partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Compare __comp)
{
    __partial_sort(__first, __middle, __last, __value_type(__first), __comp);
}

template <class _Tp, class _Compare>
inline const _Tp& __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp)
{
    if (__comp(__a, __b))
        if (__comp(__b, __c))
            return __b;
        else if (__comp(__a, __c))
            return __c;
        else
            return __a;
    else if (__comp(__a, __c))
        return __a;
    else if (__comp(__b, __c))
        return __c;
    else
        return __b;
}

template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit, _Compare __comp)
{
    while (__last - __first > 16) {
        if (__depth_limit == 0) {
            partial_sort(__first, __last, __last, __comp);
            return;
        }
        --__depth_limit;
        _RandomAccessIter __cut =
            __unguarded_partition(__first, __last,
                                  _Tp(__median(*__first, *(__first + (__last - __first) / 2), *(__last - 1), __comp)),
                                  __comp);
        __introsort_loop(__cut, __last, (_Tp*)0, __depth_limit, __comp);
        __last = __cut;
    }
}
}

template void __fastcall std::__introsort_loop<TVoiceEntry*, TVoiceEntry, int, TVoiceLess>(TVoiceEntry*, TVoiceEntry*, TVoiceEntry*, int, TVoiceLess);