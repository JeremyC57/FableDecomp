// std::__uninitialized_copy(const STripleStringRecord10* first, const STripleStringRecord10* last, STripleStringRecord10* result, const __false_type&) -- 16-byte {three CCharStrings, two flags} records: three string copies, two bytes, return the end
// Leaf: ?__uninitialized_copy@std@@YIPAUSTripleStringRecord10@FableCopy_00566430@@PBU23@0PAU23@ABU__false_type@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00566430, length 82 bytes, `ret 8`.
// Element: 16 bytes = three CCharStrings (+0, +4, +8) and two byte flags (+0xC, +0xD); implicit copy constructor
// (three out-of-line string copies, then the two bytes). No PDB record of this shape; the owner is not established
// (callers: the landed vector fill-insert 0x00568FC0 and resize 0x00569550, manifest "vector_QuestPair_"):
// descriptive names.
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
namespace FableCopy_00566430 {
struct CCharString {                                    // 4 bytes: the shared string-data pointer
    void* m_pData;
    CCharString(const CCharString& other);              // 0x0099EC30 (out of line)
};
struct STripleStringRecord10 {                          // 16 bytes
    CCharString m_string0;                              // +0x00
    CCharString m_string4;                              // +0x04
    CCharString m_string8;                              // +0x08
    bool        m_flagC;                                // +0x0C
    bool        m_flagD;                                // +0x0D
};
}
namespace std {
FableCopy_00566430::STripleStringRecord10* __fastcall __uninitialized_copy(const FableCopy_00566430::STripleStringRecord10* first, const FableCopy_00566430::STripleStringRecord10* last,
    FableCopy_00566430::STripleStringRecord10* result, const __false_type&)
{
    FableCopy_00566430::STripleStringRecord10* cur = result;
    for (; first != last; ++first, ++cur)
        new ((void*)cur) FableCopy_00566430::STripleStringRecord10(*first);
    return cur;
}
}