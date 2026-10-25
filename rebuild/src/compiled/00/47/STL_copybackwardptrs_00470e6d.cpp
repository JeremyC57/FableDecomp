// std::__copy_backward_ptrs(pair<unsigned long,CSoundPair>* first, pair<unsigned long,CSoundPair>* last, pair<unsigned long,CSoundPair>* result, const __false_type&) -- STLport copy_backward for the sound map's 12-byte (key, sound pair) entries: counted backward member-wise copy, returns the new result start
// Instantiation: std::__copy_backward_ptrs<pair<unsigned long,CSoundPair>*,pair<unsigned long,CSoundPair>*>
// Leaf: ??$__copy_backward_ptrs@PAU?$pair@KVCSoundPair@@@std@@PAU12@@std@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00470E6D, 49 bytes (size-optimised def TU, no padding), __fastcall
//   (first ecx, last edx; result and the tag reference on the stack), `ret 8`.
// Identity: the only caller is __linear_insert @ 0x00470E08 (the sound map's insertion-sort step), which builds the
//   `_IsOKToMemCpy(...)._Answer()` __false_type tag in a zeroed byte local and pushes &tag and last + 1. The 8-byte twin
//   is the landed 0x0047130E. The catalog name `std::sort_shift_backward_12byte` is descriptive.
// Shape: STLport 4.6 `__copy_backward_ptrs(first, last, result, const __false_type&)` with the SGI random-access
//   `__copy_backward(.., random_access_iterator_tag(), (ptrdiff_t*)0)` inlined (`__forceinline` under "s"): the count is a
//   signed divide by 12 (`push 0Ch ; pop ecx ; idiv`), the source addressed as result + (last - result), movsd x3 per step.
#pragma optimize("s", on)

class CSoundPair {                                          // Ego_r.pdb, 8 bytes (a class: mangled V)
public:
    long FirstSound;                                        // +0x0
    long LastSound;                                         // +0x4
};

namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };
struct __false_type { __false_type() { } };

template <class _T1, class _T2> struct pair {
    _T1 first;                                              // +0x0
    _T2 second;                                             // +0x4
};

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__forceinline _BidirectionalIter __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result,
                                                 const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template <class _BidirectionalIter1, class _BidirectionalIter2>
_BidirectionalIter2 __fastcall __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                    _BidirectionalIter2 __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

typedef pair<unsigned long, CSoundPair> _SoundEntry;
template _SoundEntry* __fastcall __copy_backward_ptrs(_SoundEntry*, _SoundEntry*, _SoundEntry*, const __false_type&);
}