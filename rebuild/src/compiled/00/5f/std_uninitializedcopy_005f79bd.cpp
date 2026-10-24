// std::__uninitialized_copy(const SValue04* first, const SValue04* last, SValue04* result, const __false_type&) -- a 4-byte value record (STLport __false_type path): copy one dword per element, return the end
// Leaf: ?__uninitialized_copy@std@@YIPAUSValue04@FableCopy_005f79bd@@PBU23@0PAU23@ABU__false_type@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F79BD, length 31 bytes, `ret 8`.
// Element: a 4-byte class-type value (one dword copied through esi), taking STLport's `__false_type` placement path.
// Its class is not established (callers 0x005F9495 / 0x005F9633 vector reallocation helpers): descriptive name.
// Size TU: unaligned body, no loop-head padding -- `#pragma optimize("s", on)`.
// Byte twin: 0x005BA146 (separate COMDAT copy; distinct element namespace keeps one symbol per address).
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
namespace FableCopy_005f79bd {
struct SValue04 {                                       // 4-byte class-type value
    unsigned long m_value;
};
}
namespace std {
FableCopy_005f79bd::SValue04* __fastcall __uninitialized_copy(const FableCopy_005f79bd::SValue04* first, const FableCopy_005f79bd::SValue04* last,
    FableCopy_005f79bd::SValue04* result, const __false_type&)
{
    FableCopy_005f79bd::SValue04* cur = result;
    for (; first != last; ++first, ++cur)
        new ((void*)cur) FableCopy_005f79bd::SValue04(*first);
    return cur;
}
}