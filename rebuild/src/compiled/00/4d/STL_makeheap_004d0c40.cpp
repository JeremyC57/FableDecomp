// _STL::__make_heap(pair<float, CCharString>* first, pair<float, CCharString>* last, CKeyPairCompareLess comp, pair<float, CCharString>*, int*) -- STLport heapify of a subtitled message's animation tags (time -> animation name): sift every parent down, from the last one to the root
// Leaf: ??$__make_heap@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D0C40, 80 bytes, `ret 0Ch` (/Gr algorithm TU, explicit __fastcall here: first in
//   ecx, last in edx, then the comparator slot and the two tag pointers).
// Callers: 0x004D10A9 (make_heap front end) and the __partial_sort 0x004D1BC0 (this chunk).
// Callees: CCharString copy 0x0099EC30 (building T(*(first + parent)) in the argument area), __adjust_heap 0x004D0590
//   (landed).
// Identity: CSubtitledMessage::AnimationTags, a CVectorMap<float, CCharString, CKeyPairCompareLess<...> > (Ego_r.pdb;
//   the landed __linear_insert 0x004D0F80 and __adjust_heap 0x004D0590 share the 8-byte element).
// Shape: STLport 4.x `__make_heap` verbatim: the by-value pair is built in place below the return address (float as a
//   dword, string by its copy constructor) and destroyed by the callee, so no destructor call appears here.
class CCharString {
public:
    void* PStringData;                                  // the shared string block
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

namespace _STL {
template <class _T1, class _T2> struct pair {          // STLport pair: user constructors, implicit operator=
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x04
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class K, class V> class CKeyPairCompareLess { // Ego_r.pdb: empty, passed by value (4-byte slot)
public:
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const { return a.first < b.first; }
};

typedef _STL::pair<float, CCharString> TTextPair;                           // 8 bytes
typedef CKeyPairCompareLess<float, CCharString> TTextLess;

namespace _STL {
template <class _RandomAccessIter, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIter __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp);   // 0x004D0590 (landed)

template <class _RandomAccessIter, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp, _Tp*, _Distance*)
{
    if (__last - __first < 2)
        return;
    _Distance __len = __last - __first;
    _Distance __parent = (__len - 2) / 2;
    for (;;) {
        __adjust_heap(__first, __parent, __len, _Tp(*(__first + __parent)), __comp);
        if (__parent == 0)
            return;
        __parent--;
    }
}
template void __fastcall __make_heap<TTextPair*, TTextLess, TTextPair, int>(TTextPair*, TTextPair*, TTextLess, TTextPair*, int*);
}