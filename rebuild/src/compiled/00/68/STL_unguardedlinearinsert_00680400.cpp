// std::__unguarded_linear_insert(pair<EReactionSpeechType, long>* last, pair<EReactionSpeechType, long> val, CKeyPairCompareLess<EReactionSpeechType, long> comp) -- STLport insertion step over CSimVoice's voice entries: shift every larger entry up one slot and drop val into the hole
// Leaf: ??$__unguarded_linear_insert@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00680400, length 51 bytes (+ int3 padding), `ret 0Ch`.
// Callers: __linear_insert 0x00680CB0 (0x00680D08), in the sort of CSimVoice::Voices (sort 0x006813A0, landed).
// Callees: none.
// Label: catalog "std::lower_bound_insert" is descriptive; the body is STLport 4.x `__unguarded_linear_insert` verbatim
//   (`__next = __last; --__next; while (__comp(__val, *__next)) { *__last = *__next; __last = __next; --__next; }
//   *__last = __val;`).
// Layout: Ego_r.pdb CSimVoice::Voices = CVectorMap<EReactionSpeechType, long, CKeyPairCompareLess<EReactionSpeechType,
//   long> >, entries pair<EReactionSpeechType, long> (8 bytes; STLport's pair declares its copy constructor); the key
//   compare is `a.first < b.first` (signed). The same model as the landed __unguarded_insertion_sort_aux 0x00680700.
// Shape: /Gr algorithm TU (`__fastcall` template: last in ecx, edx unused, the 8-byte value and the empty comparator on
//   the stack); the value's key rides in edx, its second word is read from the argument slot for the final store.
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
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
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
}

template void __fastcall std::__unguarded_linear_insert<TVoiceEntry*, TVoiceEntry, TVoiceLess>(TVoiceEntry*, TVoiceEntry, TVoiceLess);