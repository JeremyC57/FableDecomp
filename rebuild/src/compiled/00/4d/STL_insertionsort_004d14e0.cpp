// std::__insertion_sort(pair<float, CCharString>* first, pair<float, CCharString>* last, CKeyPairCompareLess comp) -- STLport insertion sort of a subtitled message's animation tags (time -> animation name): linear-insert every entry after the first into the sorted prefix
// Leaf: ??$__insertion_sort@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D14E0, 68 bytes, `ret 4` (/Gr algorithm TU, explicit __fastcall here: first in
//   ecx, last in edx, the empty comparator slot on the stack).
// Callers: 0x004D1AB0 / 0x004D1ACE (the __final_insertion_sort halves) and 0x004D253D / 0x004D2557 (landed sort
//   front ends).
// Callees: CCharString copy 0x0099EC30 (the by-value *i argument), __linear_insert 0x004D0F80 (landed).
// Identity: CSubtitledMessage::AnimationTags, a CVectorMap<float, CCharString, CKeyPairCompareLess<...> > (Ego_r.pdb;
//   the landed __linear_insert 0x004D0F80 and __adjust_heap 0x004D0590 share the 8-byte element).
// Shape: STLport 4.x `__insertion_sort` verbatim: each pass copies *i into the outgoing argument block (float as a dword,
//   string by its copy constructor) before `call 0x4D0F80`.
class CCharString {
public:
    void* PStringData;                                  // the shared string block
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

namespace std {
template <class _T1, class _T2> struct pair {          // STLport pair: user constructors, implicit operator=
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x04
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class K, class V> class CKeyPairCompareLess { // Ego_r.pdb: empty, passed by value (4-byte slot)
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

typedef std::pair<float, CCharString> TTextPair;                           // 8 bytes
typedef CKeyPairCompareLess<float, CCharString> TTextLess;

namespace std {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x004D0F80 (landed)

template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first == __last)
        return;
    for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)
        __linear_insert(__first, __i, *__i, __comp);
}
template void __fastcall __insertion_sort<TTextPair*, TTextLess>(TTextPair*, TTextPair*, TTextLess);
}