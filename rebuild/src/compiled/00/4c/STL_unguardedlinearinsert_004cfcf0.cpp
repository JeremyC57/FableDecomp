// std::__unguarded_linear_insert(pair<float, CCharString>* last, pair<float, CCharString> val, CKeyPairCompareLess comp) -- STLport insertion-sort inner loop over a subtitled message's animation tags (time -> animation name): shift every later-timed entry up one slot, then store the value in the hole
// Leaf: ??$__unguarded_linear_insert@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004CFCF0, 72 bytes, `ret 0Ch` (/Gr algorithm TU, explicit __fastcall here: last in
//   ecx, the 8-byte by-value pair and the empty comparator slot on the stack; the callee destroys the by-value pair).
// Callers: the __unguarded_insertion_sort_aux 0x004D0448 and the __linear_insert 0x004D0FE5 (both landed).
// Callees: CCharString::operator= 0x0099EFB0 (twice), CCharString::~CCharString 0x0099EAE0 (the by-value argument).
// Identity: CSubtitledMessage::AnimationTags, a CVectorMap<float, CCharString, CKeyPairCompareLess<...> > (Ego_r.pdb;
//   the landed __linear_insert 0x004D0F80 and __adjust_heap 0x004D0590 share the 8-byte element).
// Shape: STLport 4.x `__unguarded_linear_insert` verbatim: the float key is moved as a dword and the string through its
//   out-of-line operator=, the shifting cursor rides in ecx (`mov ecx,esi ; sub esi,8`), and the argument string is
//   destroyed in place before `ret 0Ch`.
class CCharString {
public:
    void* PStringData;                                  // the shared string block
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
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
template void __fastcall __unguarded_linear_insert<TTextPair*, TTextPair, TTextLess>(TTextPair*, TTextPair, TTextLess);
}