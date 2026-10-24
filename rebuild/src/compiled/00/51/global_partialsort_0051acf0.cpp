// __partial_sort(CThingBuilding** first, CThingBuilding** middle, CThingBuilding** last, CThingBuilding**, CIsHouseLargerThan comp) -- STLport partial_sort core: heap the first part, push every smaller-ordered later element through it, then sort the heap
// Leaf: ??$__partial_sort@PAPAVCThingBuilding@@
//
// Retail 0x0051ACF0, 104 bytes, `ret 0Ch` (first in ecx, middle in edx; last, the _Tp* tag and comp on the stack).
// Callees: 0x005176D0 __make_heap (`push 0 ; push 0 ; push comp`), 0x00512410 CIsHouseLargerThan::operator() (called
//   on comp's own argument slot), 0x00515570 __adjust_heap (hole 0), 0x00519FA0 sort_heap(first, middle, comp).
// Shape: STLport _algo.c -- `make_heap(first, middle, comp); for (i = middle; i < last; ++i) if (comp(*i, *first))
//   __pop_heap(first, middle, i, _Tp(*i), comp, _Distance*); sort_heap(first, middle, comp);` with make_heap and
//   __pop_heap (`*result = *first; __adjust_heap(first, 0, last - first, value, comp)`) inline; the loop test is an
//   unsigned pointer compare (`jae` / `jb`).  The retail TU is /Gr, spelled here as explicit __fastcall.
// Lever: CIsHouseLargerThan::operator() gets a same-TU __declspec(noinline) body (the landed 0x00512410 source), so VC
//   sees that the call through comp's own slot never writes it and keeps comp's 4-byte slot value in ebp for all three
//   by-value pushes (retail), reloading `last` instead.  With only a declaration the slot counts as escaped: comp is
//   re-read before every push and `last` takes ebp (108 v 104 bytes).
// Label: catalog "std_partial_sort_heap" (propagated); STLport spelling __partial_sort.
struct CRefCounted {                           // reference count base at +4 of a def (landed 0x00512410 model)
    long Count;
    void DecRef() { --Count; }
    long GetRefCount() const { return Count; }
};
struct CBuildingSimDefinition {
    virtual void v00();
    virtual void Destroy();                    // slot 1 (+0x04)
    CRefCounted Ref;                           // +0x04
    unsigned char _pad08[0x44 - 0x08];
    long HouseSize;                            // +0x44
    __forceinline void ReleaseTemporary() { Ref.DecRef(); if (Ref.GetRefCount() == 0) Destroy(); }
};
template <class T> struct CDefPointer {
    T* Object;
    __forceinline ~CDefPointer() { if (Object) { Object->ReleaseTemporary(); Object = 0; } }
    T* operator->() const { return Object; }
};
class CThingBuilding {
public:
    CDefPointer<CBuildingSimDefinition> PeekPSimDefinition() const;   // 0x0082D2D0
};

class CIsHouseLargerThan {
public:
    __declspec(noinline) bool operator()(const CThingBuilding* a, const CThingBuilding* b) const   // 0x00512410 (same-TU body)
    {
        if (a->PeekPSimDefinition().Object && b->PeekPSimDefinition().Object)
            return a->PeekPSimDefinition()->HouseSize > b->PeekPSimDefinition()->HouseSize;
        return false;
    }
};

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator first, _Distance holeIndex, _Distance len, _Tp value,
                              _Compare comp);                                 // 0x00515570
template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator first, _RandomAccessIterator last, _Compare comp, _Tp*,
                            _Distance*);                                      // 0x005176D0
template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator first, _RandomAccessIterator last, _Compare comp);   // 0x00519FA0

template <class _Tp> inline _Tp* __value_type(_Tp* const&) { return (_Tp*)0; }
template <class _Tp> inline int* __distance_type(_Tp* const&) { return (int*)0; }

template <class _RandomAccessIterator, class _Compare>
inline void make_heap(_RandomAccessIterator first, _RandomAccessIterator last, _Compare comp)
{
    __make_heap(first, last, comp, __value_type(first), __distance_type(first));
}

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
inline void __pop_heap(_RandomAccessIterator first, _RandomAccessIterator last, _RandomAccessIterator result,
                       _Tp value, _Compare comp, _Distance*)
{
    *result = *first;
    __adjust_heap(first, _Distance(0), _Distance(last - first), value, comp);
}

template <class _RandomAccessIterator, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIterator first, _RandomAccessIterator middle, _RandomAccessIterator last,
                               _Tp*, _Compare comp)
{
    make_heap(first, middle, comp);
    for (_RandomAccessIterator i = middle; i < last; ++i)
        if (comp(*i, *first))
            __pop_heap(first, middle, i, _Tp(*i), comp, __distance_type(first));
    sort_heap(first, middle, comp);
}
}

template void __fastcall std::__partial_sort<CThingBuilding**, CThingBuilding*, CIsHouseLargerThan>(
    CThingBuilding**, CThingBuilding**, CThingBuilding**, CThingBuilding**, CIsHouseLargerThan);