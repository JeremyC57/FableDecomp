// _STL::__unguarded_insertion_sort_aux(pair<unsigned long, CSmallVector<NAnimationEvents::CEvent,32> >* first, pair<unsigned long, CSmallVector<NAnimationEvents::CEvent,32> >* last, pair<unsigned long, CSmallVector<NAnimationEvents::CEvent,32> >*, CKeyPairCompareLess comp) -- unguarded insertion-sort pass over the (animation, event list) entries: insert a copy of every entry of [first, last)
// Leaf: ??$__unguarded_insertion_sort_aux@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006FBD10, 69 bytes (+11 int3), `ret 8` (link-time __fastcall /Gr algorithm TU,
//   written as explicit __fastcall: first in ecx, last in edx, the value-type pointer and the empty comparator's 4-byte
//   slot on the stack).
// Identity: the tail of the landed __final_insertion_sort 0x006FCE50 (and the sibling partition COMDAT 0x006FBEE0) over
//   the 12-byte pair<unsigned long, CSmallVector<NAnimationEvents::CEvent,32> > entries of the animation-event manager's
//   key-sorted vector map.
// Callees: CSmallVector<CEvent,32>::Copy 0x006FB5C0 (landed; inside the event list's copy constructor),
//   __unguarded_linear_insert 0x006FB960 (landed; it destroys the by-value pair).
// Layout: Ego_r.pdb CSmallVector<NAnimationEvents::CEvent,32>: Start +0, Size (int) +4 (the landed Copy model).
// Shape: STLport 4.x `for (i = first; i != last; ++i) __unguarded_linear_insert(i, T(*i), comp);`: the by-value pair is
//   built in its outgoing slot (`sub esp,0Ch ; mov eax,esp`): the key dword, then CSmallVector's copy constructor
//   `Start(0), Size(0) { Copy(other); }` inline (two immediate zero stores, the Copy call).
// Label correction: catalogued as `vector<std::pair<CCharString...>::_Destroy` (propagated); the body is STLport
//   `__unguarded_insertion_sort_aux`.
namespace NAnimationEvents {
class CEvent {                                            // Ego_r.pdb: 8 bytes
public:
    unsigned long Index : 30;                             // +0x00
    unsigned long ExternallyImported : 1;
    unsigned long StopEvent : 1;
    float Time;                                           // +0x04
};
}

template <class T, int N> class CSmallVector {
public:
    T*  Start;                                            // +0x00
    int Size;                                             // +0x04
    CSmallVector(const CSmallVector& other) : Start(0), Size(0) { Copy(other); }
    ~CSmallVector();
protected:
    void Copy(const CSmallVector& other);                 // 0x006FB5C0
};

namespace _STL {
template <class _T1, class _T2> struct pair {             // STLport pair: user constructors, implicit operator=
    _T1 first;                                             // +0x00
    _T2 second;                                            // +0x04
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class K, class V> class CKeyPairCompareLess {    // empty, passed by value (4-byte slot)
public:
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const { return a.first < b.first; }
};

typedef CSmallVector<NAnimationEvents::CEvent, 32> TEventList;
typedef _STL::pair<unsigned long, TEventList> TAnimEventEntry;                 // 12 bytes
typedef CKeyPairCompareLess<unsigned long, TEventList> TAnimEventLess;

namespace _STL {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x006FB960

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}

template void __fastcall __unguarded_insertion_sort_aux<TAnimEventEntry*, TAnimEventEntry, TAnimEventLess>(TAnimEventEntry*, TAnimEventEntry*, TAnimEventEntry*, TAnimEventLess);
}