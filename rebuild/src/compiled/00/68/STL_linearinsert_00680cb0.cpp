// _STL::__linear_insert(pair<EReactionSpeechType, long>* first, pair<EReactionSpeechType, long>* last, pair<EReactionSpeechType, long> val, CKeyPairCompareLess<EReactionSpeechType, long> comp) -- STLport insertion-sort step over CSimVoice's voice entries: a value below the first entry shifts the run up one slot and goes first, anything else goes through the unguarded insert
// Leaf: ??$__linear_insert@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00680CB0, length 99 bytes, `ret 0Ch`.
// Callers: __insertion_sort 0x00680DC0 (landed; 0x00680DEB), in the sort of CSimVoice::Voices (sort 0x006813A0, landed).
// Callees: __unguarded_linear_insert(last, T val, comp) @ 0x00680400 (out of line; the value copy built in the argument
//   area by the pair copy constructor).
// Label: catalog "std::insertion_sort_pairs" is descriptive; the body is STLport 4.x `__linear_insert` verbatim:
//   `if (__comp(__val, *__first)) { copy_backward(__first, __last, __last + 1); *__first = __val; } else
//   __unguarded_linear_insert(__last, __val, __comp);`.
// Layout: Ego_r.pdb CSimVoice::Voices = CVectorMap<EReactionSpeechType, long, CKeyPairCompareLess<EReactionSpeechType,
//   long> >, entries pair<EReactionSpeechType, long> (8 bytes; STLport's pair declares its copy constructor); the key
//   compare is `a.first < b.first` (signed). The same model as the landed __unguarded_insertion_sort_aux 0x00680700.
// Shape: /Gr algorithm TU (`__fastcall` template: first in ecx, last in edx, the 8-byte value and the empty comparator
//   on the stack). copy_backward -> __copy_backward_aux -> __copy_backward_ptrs (pair is not memcpy-safe) ->
//   __copy_backward (`for (__n = __last - __first; __n > 0; --__n) *--__result = *--__last;`) all inline here
//   (`__forceinline` on the loop body; plain `inline` leaves it a call).
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
struct __true_type { };
struct __false_type { };
struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };

template <class _Tp1, class _Tp2> struct _BothPtrType {
    static __false_type _Ret() { return __false_type(); }
};
template <class _Tp1, class _Tp2> struct _BothPtrType<_Tp1*, _Tp2*> {
    static __true_type _Ret() { return __true_type(); }
};
template <class _Tp1, class _Tp2> struct _OKToMemCpy {
    typedef __false_type _Type;                            // __type_traits<pair>: no trivial assignment
    static _Type _Ret() { return _Type(); }
};
template <class _Tp1, class _Tp2> inline _OKToMemCpy<_Tp1, _Tp2> _IsOKToMemCpy(_Tp1*, _Tp2*) { return _OKToMemCpy<_Tp1, _Tp2>(); }
template <class _Tp> inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__forceinline _BidirectionalIter __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _BI1, class _BI2>
inline _BI2 __copy_backward_aux(_BI1 __first, _BI1 __last, _BI2 __result, const __true_type&)
{
    return __copy_backward_ptrs(__first, __last, __result, _IsOKToMemCpy(__value_type(__first), __value_type(__result))._Ret());
}
template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result)
{
    return __copy_backward_aux(__first, __last, __result, _BothPtrType<_BidirectionalIter1, _BidirectionalIter2>::_Ret());
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x00680400

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    if (__comp(__val, *__first)) {
        copy_backward(__first, __last, __last + 1);
        *__first = __val;
    } else
        __unguarded_linear_insert(__last, __val, __comp);
}
}

template void __fastcall _STL::__linear_insert<TVoiceEntry*, TVoiceEntry, TVoiceLess>(TVoiceEntry*, TVoiceEntry*, TVoiceEntry, TVoiceLess);