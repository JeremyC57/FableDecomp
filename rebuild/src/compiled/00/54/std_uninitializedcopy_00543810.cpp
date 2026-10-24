// std::__uninitialized_copy(const SKeyedRecord1C* first, const SKeyedRecord1C* last, SKeyedRecord1C* result, const __false_type&) -- 28-byte {key, three words + C3DVector} records: copy the key, then the record memberwise, return the end
// Leaf: ?__uninitialized_copy@std@@YIPAU?$pair@JUSRecord18@FableCopy_00543810@@@FableCopy_00543810@@PBU23@0PAU23@ABU__false_type@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00543810, length 81 bytes, `ret 8`.
// Element: 28 bytes = a key dword +0 and a 24-byte record +4 (three dwords, then a C3DVector at +0x10) with a
// memberwise user copy constructor: VC7.1 keeps the second src/dst pointer pair at &vector + 8 = +0x18
// (`lea edi,[eax+18h] ; lea esi,[ecx+18h]`) and copies +4..+0x18 relative to it; a flat six-word record puts the
// pair at +0xC (graded). The record's class is not established (caller 0x00543B70, manifest-labelled
// CAnimatedBlockArray::Allocate; 28-byte PDB candidates such as CCylinder are unproven): descriptive names.
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` from a /Gr TU (`__fastcall`: first
// in ecx, last in edx, result and the empty tag reference on the stack, `ret 8`): `for (; first != last; ++first,
// ++cur) _Construct(&*cur, *first);` -- a bottom-tested loop behind the `first == last` test, the placement new
// null-tests the destination, the advanced result is returned in eax.
// Label correction: the catalog's `_Uninit_copy<_Vector_const_iterator<_Vector_val<unsigned int,allocator<unsigned int> > >,
// unsigned int*,allocator<unsigned int> >` is a Dinkumware name propagated over all 40 rows of this chunk; retail's STL
// is STLport 4.x and the `ret 8`, the element stride and the placement null test name this body.
inline void* operator new(unsigned int, void* where) { return where; }
struct __false_type { __false_type() { } };
namespace FableCopy_00543810 {
#pragma pack(push, 1)
struct C3DVector {                                      // byte-packed record in retail (the PDB places C3DVector members at
    float X, Y, Z;                                      // odd offsets, e.g. CPlayerPositionInfo::Pos +1); memberwise user copy
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
#pragma pack(pop)
struct SRecord18 {                                      // 24 bytes
    long      m_word0;                                  // +0x00
    long      m_word4;                                  // +0x04
    long      m_word8;                                  // +0x08
    C3DVector m_vector;                                 // +0x0C
    SRecord18(const SRecord18& other)
        : m_word0(other.m_word0), m_word4(other.m_word4), m_word8(other.m_word8), m_vector(other.m_vector) { }
};
template <class T1, class T2>
struct pair {                                           // STLport pair: explicit memberwise copy constructor
    T1 first;
    T2 second;
    pair(const pair& other) : first(other.first), second(other.second) { }
};
typedef pair<long, SRecord18> SKeyedRecord1C;
}
namespace std {
FableCopy_00543810::SKeyedRecord1C* __fastcall __uninitialized_copy(const FableCopy_00543810::SKeyedRecord1C* first, const FableCopy_00543810::SKeyedRecord1C* last,
    FableCopy_00543810::SKeyedRecord1C* result, const __false_type&)
{
    FableCopy_00543810::SKeyedRecord1C* cur = result;
    for (; first != last; ++first, ++cur)
        new ((void*)cur) FableCopy_00543810::SKeyedRecord1C(*first);
    return cur;
}
}