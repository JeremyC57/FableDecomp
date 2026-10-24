// std::__uninitialized_copy(const SRecord0C* first, const SRecord0C* last, SRecord0C* result, const __false_type&) -- a 12-byte plain record: block-copy three dwords per element (`movsd` x3), return the end
// Leaf: ?__uninitialized_copy@std@@YIPAUSRecord0C@FableCopy_005924d0@@PBU23@0PAU23@ABU__false_type@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005924D0, length 36 bytes, `ret 8`.
// Element: a 12-byte POD of three dwords; its implicit copy is a block copy, which the size-optimised TU emits
// as `mov esi,ecx ; mov edi,eax ; movsd ; movsd ; movsd` (the /O2 build copies through ebx instead). The record's
// class is not established (callers 0x00593F39): descriptive name.
// Size TU: no loop-head alignment and the movsd block copy -- `#pragma optimize("s", on)`.
// Byte twin: 0x005790C3 (separate COMDAT copy; distinct element namespace keeps one symbol per address).
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` from a /Gr TU (`__fastcall`: first
// in ecx, last in edx, result and the empty tag reference on the stack, `ret 8`): `for (; first != last; ++first,
// ++cur) _Construct(&*cur, *first);` -- a bottom-tested loop behind the `first == last` test, the placement new
// null-tests the destination, the advanced result is returned in eax.
// Label correction: the catalog's `_Uninit_copy<_Vector_const_iterator<_Vector_val<unsigned int,allocator<unsigned int> > >,
// unsigned int*,allocator<unsigned int> >` is a Dinkumware name propagated over all 40 rows of this chunk; retail's STL
// is STLport 4.x and the `ret 8`, the element stride and the placement null test name this body.
#pragma optimize("s", on)
inline void* operator new(unsigned int, void* where) { return where; }
struct __false_type { __false_type() { } };
namespace FableCopy_005924d0 {
struct SRecord0C {                                      // 12-byte plain record
    long m_word0;
    long m_word4;
    long m_word8;
};
}
namespace std {
FableCopy_005924d0::SRecord0C* __fastcall __uninitialized_copy(const FableCopy_005924d0::SRecord0C* first, const FableCopy_005924d0::SRecord0C* last,
    FableCopy_005924d0::SRecord0C* result, const __false_type&)
{
    FableCopy_005924d0::SRecord0C* cur = result;
    for (; first != last; ++first, ++cur)
        new ((void*)cur) FableCopy_005924d0::SRecord0C(*first);
    return cur;
}
}