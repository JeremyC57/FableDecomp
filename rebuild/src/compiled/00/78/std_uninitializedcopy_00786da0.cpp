// std::__uninitialized_copy(const SNamedRecord0C* first, const SNamedRecord0C* last, SNamedRecord0C* result, const __false_type&) -- 12-byte {word, CCharString, word} records: word, string copy, word, return the end
// Leaf: ?__uninitialized_copy@std@@YIPAUSNamedRecord0C@FableCopy_00786da0@@PBU23@0PAU23@ABU__false_type@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00786DA0, length 60 bytes, `ret 8`.
// Element: 12 bytes = a dword +0, a CCharString +4 and a dword +8 (implicit copy constructor: the first word, the
// out-of-line string copy, the last word). Several PDB records share the layout; the owner is not established
// (caller 0x00787380, a vector reallocation): descriptive names.
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30.
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` from a /Gr TU (`__fastcall`: first
// in ecx, last in edx, result and the empty tag reference on the stack, `ret 8`): `for (; first != last; ++first,
// ++cur) _Construct(&*cur, *first);` -- a bottom-tested loop behind the `first == last` test, the placement new
// null-tests the destination, the advanced result is returned in eax.
// Label correction: the catalog's `_Uninit_copy<_Vector_const_iterator<_Vector_val<unsigned int,allocator<unsigned int> > >,
// unsigned int*,allocator<unsigned int> >` is a Dinkumware name propagated over all 40 rows of this chunk; retail's STL
// is STLport 4.x and the `ret 8`, the element stride and the placement null test name this body.
inline void* operator new(unsigned int, void* where) { return where; }
struct __false_type { __false_type() { } };
namespace FableCopy_00786da0 {
struct CCharString {                                    // 4 bytes: the shared string-data pointer
    void* m_pData;
    CCharString(const CCharString& other);              // 0x0099EC30 (out of line)
};
struct SNamedRecord0C {                                 // 12 bytes
    long        m_word0;                                // +0x00
    CCharString m_name;                                 // +0x04
    long        m_word8;                                // +0x08
};
}
namespace std {
FableCopy_00786da0::SNamedRecord0C* __fastcall __uninitialized_copy(const FableCopy_00786da0::SNamedRecord0C* first, const FableCopy_00786da0::SNamedRecord0C* last,
    FableCopy_00786da0::SNamedRecord0C* result, const __false_type&)
{
    FableCopy_00786da0::SNamedRecord0C* cur = result;
    for (; first != last; ++first, ++cur)
        new ((void*)cur) FableCopy_00786da0::SNamedRecord0C(*first);
    return cur;
}
}