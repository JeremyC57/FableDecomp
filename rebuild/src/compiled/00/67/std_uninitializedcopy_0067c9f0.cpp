// std::__uninitialized_copy(const CBlurredAttitudesDef* first, const CBlurredAttitudesDef* last, CBlurredAttitudesDef* result, const __false_type&) -- CBlurredAttitudesDef elements: the inline copy-constructor chain of the 0x48-byte def, return the end
// Leaf: ?__uninitialized_copy@std@@YIPAUCBlurredAttitudesDef@FableCopy_0067c9f0@@PBU23@0PAU23@ABU__false_type@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0067C9F0, length 94 bytes, `ret 8`.
// Element: CBlurredAttitudesDef (0x48 bytes, vtable 0x0125B8AC), the element of the landed
// vector<CBlurredAttitudesDef>::_M_insert_overflow 0x0067CB20 (its two calls of 0x0067C9F0) and fill-insert
// 0x0067CC50. The copy-constructor chain is inlined as in the landed __uninitialized_fill_n 0x0067CA70 (same model):
// CDefClassBase's out-of-line copy (0x0046644C), the CParentDefClassBase vtable 0x01230CBC, InstantiationName +0x25,
// the SubDefInfoMap vector copy (0x0046EECF) with its sorted flag +0x36, the derived vtable, then the Attitudes vector
// copy (0x00471F97). The byte-packed layout (CDefClassBase 0x25 bytes) gives the odd offsets.
// Callees: 0x0046644C, 0x0046EECF, 0x00471F97.
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` from a /Gr TU (`__fastcall`: first
// in ecx, last in edx, result and the empty tag reference on the stack, `ret 8`): `for (; first != last; ++first,
// ++cur) _Construct(&*cur, *first);` -- a bottom-tested loop behind the `first == last` test, the placement new
// null-tests the destination, the advanced result is returned in eax.
// Label correction: the catalog's `_Uninit_copy<_Vector_const_iterator<_Vector_val<unsigned int,allocator<unsigned int> > >,
// unsigned int*,allocator<unsigned int> >` is a Dinkumware name propagated over all 40 rows of this chunk; retail's STL
// is STLport 4.x and the `ret 8`, the element stride and the placement null test name this body.
inline void* operator new(unsigned int, void* where) { return where; }
struct __false_type { __false_type() { } };
namespace FableCopy_0067c9f0 {
#pragma pack(push, 1)
struct CDefClassBase {
    virtual void v00();
    unsigned char _body[0x21];                          // +0x04 .. +0x25
    CDefClassBase(const CDefClassBase& other);          // 0x0046644C
};
struct CSubDefInfoVector {                              // retail 12-byte vector of {unsigned long, CSubDefInfo}
    void* m_begin; void* m_end; void* m_capacity;
    CSubDefInfoVector(const CSubDefInfoVector& other);  // 0x0046EECF
};
struct CKeyPairCompareLess { };                        // empty comparator (nothing copied)
struct CSubDefInfoMap {                                 // CVectorMap<unsigned long, CDefClassBase::CSubDefInfo>
    CSubDefInfoVector   m_entries;                      // +0x00
    CKeyPairCompareLess m_compare;                      // +0x0C
    unsigned char       m_sorted;                       // +0x0D
    CSubDefInfoMap(const CSubDefInfoMap& other) : m_entries(other.m_entries), m_compare(other.m_compare), m_sorted(other.m_sorted) { }
};
struct CParentDefClassBase : CDefClassBase {            // vtable 0x01230CBC
    virtual void v00();
    long           InstantiationName;                   // +0x25 CDefString
    CSubDefInfoMap SubDefInfoMap;                       // +0x29
    CParentDefClassBase(const CParentDefClassBase& other)
        : CDefClassBase(other), InstantiationName(other.InstantiationName), SubDefInfoMap(other.SubDefInfoMap) { }
};
struct CAttitudeVector {                                // retail 12-byte vector<EOpinionAttitudeType>
    void* m_begin; void* m_end; void* m_capacity;
    CAttitudeVector(const CAttitudeVector& other);      // 0x00471F97
};
struct CBlurredAttitudesDef : CParentDefClassBase {     // vtable 0x0125B8AC, 0x48 bytes
    virtual void v00();
    unsigned char   _pad[0x3c - 0x37];                  // +0x37 .. +0x3C
    CAttitudeVector Attitudes;                          // +0x3C
    CBlurredAttitudesDef(const CBlurredAttitudesDef& other)
        : CParentDefClassBase(other), Attitudes(other.Attitudes) { }
};
#pragma pack(pop)
}
namespace std {
FableCopy_0067c9f0::CBlurredAttitudesDef* __fastcall __uninitialized_copy(const FableCopy_0067c9f0::CBlurredAttitudesDef* first, const FableCopy_0067c9f0::CBlurredAttitudesDef* last,
    FableCopy_0067c9f0::CBlurredAttitudesDef* result, const __false_type&)
{
    FableCopy_0067c9f0::CBlurredAttitudesDef* cur = result;
    for (; first != last; ++first, ++cur)
        new ((void*)cur) FableCopy_0067c9f0::CBlurredAttitudesDef(*first);
    return cur;
}
}