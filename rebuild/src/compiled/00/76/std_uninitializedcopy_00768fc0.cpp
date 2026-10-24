// std::__uninitialized_copy(const SKeyedVector* first, const SKeyedVector* last, SKeyedVector* result, const __false_type&) -- pair<unsigned long, C3DVector> elements: copy the key, then the vector memberwise, return the end
// Leaf: ?__uninitialized_copy@std@@YIPAU?$pair@KUC3DVector@FableCopy_00768fc0@@@FableCopy_00768fc0@@PBU23@0PAU23@ABU__false_type@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00768FC0, length 63 bytes, `ret 8`.
// Element: a 16-byte {key dword, C3DVector} pair; the key's type is not established (the PDB holds both
// pair<unsigned long, C3DVector> and pair<const CRegion*, C3DVector> vectors), modelled as pair<unsigned long, C3DVector>.
// The key is copied through the element pointers (`mov ebx,[ecx] ; mov [eax],ebx`); C3DVector's memberwise user copy
// constructor makes VC7.1 keep a second strength-reduced src/dst pointer pair at &second + 8 (`lea edi,[eax+0Ch] ;
// lea esi,[ecx+0Ch]`) and address the vector's words relative to it (GOTCHAS 00776b60).
// Callers: the vector range-insert at 0x0076ADA0.
// Byte twin: 0x006E1D80 (separate COMDAT copy; distinct element namespace keeps one symbol per address).
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` from a /Gr TU (`__fastcall`: first
// in ecx, last in edx, result and the empty tag reference on the stack, `ret 8`): `for (; first != last; ++first,
// ++cur) _Construct(&*cur, *first);` -- a bottom-tested loop behind the `first == last` test, the placement new
// null-tests the destination, the advanced result is returned in eax.
// Label correction: the catalog's `_Uninit_copy<_Vector_const_iterator<_Vector_val<unsigned int,allocator<unsigned int> > >,
// unsigned int*,allocator<unsigned int> >` is a Dinkumware name propagated over all 40 rows of this chunk; retail's STL
// is STLport 4.x and the `ret 8`, the element stride and the placement null test name this body.
inline void* operator new(unsigned int, void* where) { return where; }
struct __false_type { __false_type() { } };
namespace FableCopy_00768fc0 {
#pragma pack(push, 1)
struct C3DVector {                                      // byte-packed record in retail (the PDB places C3DVector members at
    float X, Y, Z;                                      // odd offsets, e.g. CPlayerPositionInfo::Pos +1); memberwise user copy
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
#pragma pack(pop)
template <class T1, class T2>
struct pair {                                           // STLport pair: explicit memberwise copy constructor
    T1 first;
    T2 second;
    pair(const pair& other) : first(other.first), second(other.second) { }
};
typedef pair<unsigned long, C3DVector> SKeyedVector;
}
namespace std {
FableCopy_00768fc0::SKeyedVector* __fastcall __uninitialized_copy(const FableCopy_00768fc0::SKeyedVector* first, const FableCopy_00768fc0::SKeyedVector* last,
    FableCopy_00768fc0::SKeyedVector* result, const __false_type&)
{
    FableCopy_00768fc0::SKeyedVector* cur = result;
    for (; first != last; ++first, ++cur)
        new ((void*)cur) FableCopy_00768fc0::SKeyedVector(*first);
    return cur;
}
}