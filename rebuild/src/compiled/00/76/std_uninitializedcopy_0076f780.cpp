// std::__uninitialized_copy(const SKeyedRecord18* first, const SKeyedRecord18* last, SKeyedRecord18* result, const __false_type&) -- 24-byte {key, two words + C3DVector} records: copy the key, then the record memberwise, return the end
// Leaf: ?__uninitialized_copy@std@@YIPAU?$pair@JUSRecord14@FableCopy_0076f780@@@FableCopy_0076f780@@PBU23@0PAU23@ABU__false_type@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076F780, length 75 bytes, `ret 8`.
// Element: 24 bytes = a key dword +0 and a 20-byte record +4 (two dwords, then a C3DVector at +0xC) whose memberwise
// user copy constructor holds the C3DVector (itself memberwise): VC7.1 keeps the second src/dst pointer pair at
// &vector + 8 = +0x14 (`lea edi,[eax+14h] ; lea esi,[ecx+14h]`) and copies +4..+0x14 relative to it. A flat
// five-word record puts the pair at +0xC instead (graded). The record's class is not established: descriptive names.
// Callers: 0x007700D0 (vector insert/reallocation).
// Byte twin: 0x0076F8E0 (separate COMDAT copy; distinct element namespace keeps one symbol per address).
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` from a /Gr TU (`__fastcall`: first
// in ecx, last in edx, result and the empty tag reference on the stack, `ret 8`): `for (; first != last; ++first,
// ++cur) _Construct(&*cur, *first);` -- a bottom-tested loop behind the `first == last` test, the placement new
// null-tests the destination, the advanced result is returned in eax.
// Label correction: the catalog's `_Uninit_copy<_Vector_const_iterator<_Vector_val<unsigned int,allocator<unsigned int> > >,
// unsigned int*,allocator<unsigned int> >` is a Dinkumware name propagated over all 40 rows of this chunk; retail's STL
// is STLport 4.x and the `ret 8`, the element stride and the placement null test name this body.
inline void* operator new(unsigned int, void* where) { return where; }
struct __false_type { __false_type() { } };
namespace FableCopy_0076f780 {
#pragma pack(push, 1)
struct C3DVector {                                      // byte-packed record in retail (the PDB places C3DVector members at
    float X, Y, Z;                                      // odd offsets, e.g. CPlayerPositionInfo::Pos +1); memberwise user copy
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
#pragma pack(pop)
struct SRecord14 {                                      // 20 bytes
    long      m_word0;                                  // +0x00
    long      m_word4;                                  // +0x04
    C3DVector m_vector;                                 // +0x08
    SRecord14(const SRecord14& other) : m_word0(other.m_word0), m_word4(other.m_word4), m_vector(other.m_vector) { }
};
template <class T1, class T2>
struct pair {                                           // STLport pair: explicit memberwise copy constructor
    T1 first;
    T2 second;
    pair(const pair& other) : first(other.first), second(other.second) { }
};
typedef pair<long, SRecord14> SKeyedRecord18;
}
namespace std {
FableCopy_0076f780::SKeyedRecord18* __fastcall __uninitialized_copy(const FableCopy_0076f780::SKeyedRecord18* first, const FableCopy_0076f780::SKeyedRecord18* last,
    FableCopy_0076f780::SKeyedRecord18* result, const __false_type&)
{
    FableCopy_0076f780::SKeyedRecord18* cur = result;
    for (; first != last; ++first, ++cur)
        new ((void*)cur) FableCopy_0076f780::SKeyedRecord18(*first);
    return cur;
}
}