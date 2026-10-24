// std::sort_heap(THitLocationIntersectionElement* first, THitLocationIntersectionElement* last, THitLocationIntersectionElementPred comp) -- order a heap of hit-location intersections by key by repeatedly popping the front to the back
// Flags: /Gr
// Leaf: sort_heap
// Retail: Fable.exe @ 0x008182E0, 212 bytes (/O2). Callee: STLport __adjust_heap<THitLocationIntersectionElement*,
// int,THitLocationIntersectionElement,THitLocationIntersectionElementPred> 0x008175D0 (`__fastcall`: first ecx,
// hole edx, the count, the 32-byte value and the empty comparator on the stack; signed key compare; its
// catalog label `_Adjust_heap<CDebugBox*,...>` is propagated).
// Shape: the 0x0067F040 STLport source (`while (last - first > 1) pop_heap(first, last--, comp);` with
// pop_heap -> __pop_heap_aux -> __pop_heap inline, __adjust_heap out of line) over the anonymous-namespace
// THitLocationIntersectionElement (Ego_r.pdb, 32 bytes: long key, CHitLocation* hit_location, C3DVector
// intersection_pos +8, C3DVector outward_normal +0x14). The engine C3DVector has a user copy ctor and no
// operator=, so `value = *(last - 1)` and the by-value argument copy are memberwise (two components through
// the x87 stack, the rest as dwords) while `*(last - 1) = *first` stays a trivial `rep movsd` x8;
// `last - first` is the byte count `and -20h ; cmp 20h`.
// Label correction: catalog `_Sort_heap<THitLocationIntersectionElement*,THitLocationIntersectionElementPred>`
// is the Ego_r (Dinkumware) spelling; retail is STLport sort_heap.
namespace N008182e0 {
struct CHitLocation;
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }      // user copy ctor, implicit operator=
};
struct THitLocationIntersectionElement {                  // _anon_5EBD2A70, 0x20 bytes
    long key;                                             // +0x00
    CHitLocation* hit_location;                           // +0x04
    C3DVector intersection_pos;                           // +0x08
    C3DVector outward_normal;                             // +0x14
};
struct THitLocationIntersectionElementPred {              // _anon_5EBD2A70
    bool operator()(const THitLocationIntersectionElement& a, const THitLocationIntersectionElement& b) const { return a.key < b.key; }
};
template <class _Tp> inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }
template <class _Tp> inline int* __distance_type(const _Tp*) { return (int*)0; }
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp);   // 0x008175D0
template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
__forceinline void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result, _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}
template <class _RandomAccessIterator, class _Tp, class _Compare>
__forceinline void __pop_heap_aux(_RandomAccessIterator __first, _RandomAccessIterator __last, _Tp*, _Compare __comp)
{
    __pop_heap(__first, __last - 1, __last - 1, _Tp(*(__last - 1)), __comp, __distance_type(__first));
}
template <class _RandomAccessIterator, class _Compare>
__forceinline void pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __pop_heap_aux(__first, __last, __value_type(__first), __comp);
}
template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    while (__last - __first > 1)
        pop_heap(__first, __last--, __comp);
}
template void __fastcall sort_heap<THitLocationIntersectionElement*, THitLocationIntersectionElementPred>(THitLocationIntersectionElement*, THitLocationIntersectionElement*, THitLocationIntersectionElementPred);
}