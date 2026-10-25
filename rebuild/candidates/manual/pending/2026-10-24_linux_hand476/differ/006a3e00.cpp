// std::__introsort_loop(pair<long, CIntelligentPointer<CThing> >* first, pair* last, pair*, int depth_limit, CKeyPairCompareLess comp) -- STLport introsort loop of the (key, intelligent pointer) vector map: median-of-three partition and recurse on the right part while more than 16 entries remain, falling back to partial_sort when the depth limit runs out
// Grade: DIFFER(277v277) -- register colouring only: retail binds first->ebx and last->edi, ours first->edi and last->ebx (4 grades: hoisted cut, same-TU noinline partition/partial_sort bodies, /Gr flag instead of __fastcall)
// Leaf: ??$__introsort_loop@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A3E00, 275 bytes (+ int3 padding to 288), `ret 0Ch`.
// Identity: the catalog's "Heap_SiftDown" is a descriptive auto label. The landed sort 0x006A3F20 calls this body as
//   `__introsort_loop(first, last, (T*)0, __lg(n) * 2, comp)`; it recurses into itself (0x006A3ED4). Element
//   pair<long, CIntelligentPointer<CThing> > (12 bytes) and comparator CKeyPairCompareLess<long, CIntelligentPointer<
//   CThing> > (Ego_r.pdb: the sibling _Adjust_heap/_Push_heap/_Make_heap instantiations), comparing the keys.
// Callees: __unguarded_partition(first, last, T pivot, comp) 0x006A3440, __partial_sort(first, middle, last, T*, comp)
//   0x006A3BF0 (partial_sort's out-of-line worker), CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 /
//   SetPItem 0x00A01B90 (the pivot copy), CIntelligentPointer<CThing> vtable 0x01238C6C.
// Shape: STLport 4.6 _algo.c `__introsort_loop` verbatim from a /Gr TU (`__fastcall`: first in ecx -> ebx, last in edx ->
//   edi; the value-type pointer, the depth limit and the 4-byte comparator on the stack). `__median` is inlined (three
//   key compares choosing a reference), the pivot `_Tp(__median(..))` is built in the argument area by STLport pair's
//   memberwise copy constructor (key word, then the intelligent pointer's base ctor / vtable store /
//   SetPItem(GetPItem())) after the comparator push; partial_sort is the inline front end that passes `(_Tp*)0`.
inline void* operator new(unsigned int, void* p) { return p; }

class CThing;
class CBaseObject;

class CBaseIntelligentPointer {                             // 8 bytes: vtable + item
public:
    void* PData;                                            // +0x04
    CBaseIntelligentPointer();                              // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
};

template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer { // vtable 0x01238C6C for CIntelligentPointer<CThing>
public:
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair& __o) : first(__o.first), second(__o.second) { }
};
}

template <class K, class V>
class CKeyPairCompareLess {                                 // empty comparator, passed by value
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

namespace std {

const int __stl_threshold = 16;

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

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot,
                                                   _Compare __comp);                         // 0x006A3440
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*,
                               _Compare __comp);                                             // 0x006A3BF0

template <class _Tp, class _Compare>
inline void partial_sort(_Tp* __first, _Tp* __middle, _Tp* __last, _Compare __comp)
{
    __partial_sort(__first, __middle, __last, (_Tp*)0, __comp);
}

template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit,
                                 _Compare __comp)
{
    while (__last - __first > __stl_threshold) {
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

typedef pair<long, CIntelligentPointer<CThing> > _ThingEntry;
template void __fastcall __introsort_loop<_ThingEntry*, _ThingEntry, int, CKeyPairCompareLess<long, CIntelligentPointer<CThing> > >(
    _ThingEntry*, _ThingEntry*, _ThingEntry*, int, CKeyPairCompareLess<long, CIntelligentPointer<CThing> >);

} // namespace std
