// std::__uninitialized_copy(const SVectorFlag* first, const SVectorFlag* last, SVectorFlag* result, const __false_type&) -- pair<C3DVector, bool> elements (13 bytes): copy the vector memberwise and the flag, return the end
// Leaf: ?__uninitialized_copy@std@@YIPAU?$pair@UC3DVector@FableCopy_006efec0@@_N@FableCopy_006efec0@@PBU23@0PAU23@ABU__false_type@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006EFEC0, length 56 bytes, `ret 8`.
// Element: 13 bytes = pair<C3DVector, bool> (Ego_r.pdb `_Pair_base<C3DVector,bool>` is 13 bytes: C3DVector is a
// byte-packed record, so the flag sits at +0xC with no padding; the PDB also has a vector of this pair). The vector's
// memberwise copy goes word by word through edx, then the flag byte; the 13-byte stride is `add ecx,0Dh`.
// The same-shaped CThingRespawnInfo {DiedPosition, DeadInfoSet} would compile identically; the pair is preferred
// for the vector evidence. Caller: 0x006F3260 (vector fill-insert).
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` from a /Gr TU (`__fastcall`: first
// in ecx, last in edx, result and the empty tag reference on the stack, `ret 8`): `for (; first != last; ++first,
// ++cur) _Construct(&*cur, *first);` -- a bottom-tested loop behind the `first == last` test, the placement new
// null-tests the destination, the advanced result is returned in eax.
// Label correction: the catalog's `_Uninit_copy<_Vector_const_iterator<_Vector_val<unsigned int,allocator<unsigned int> > >,
// unsigned int*,allocator<unsigned int> >` is a Dinkumware name propagated over all 40 rows of this chunk; retail's STL
// is STLport 4.x and the `ret 8`, the element stride and the placement null test name this body.
inline void* operator new(unsigned int, void* where) { return where; }
struct __false_type { __false_type() { } };
namespace FableCopy_006efec0 {
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
typedef pair<C3DVector, bool> SVectorFlag;
}
namespace std {
FableCopy_006efec0::SVectorFlag* __fastcall __uninitialized_copy(const FableCopy_006efec0::SVectorFlag* first, const FableCopy_006efec0::SVectorFlag* last,
    FableCopy_006efec0::SVectorFlag* result, const __false_type&)
{
    FableCopy_006efec0::SVectorFlag* cur = result;
    for (; first != last; ++first, ++cur)
        new ((void*)cur) FableCopy_006efec0::SVectorFlag(*first);
    return cur;
}
}