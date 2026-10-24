// std::__uninitialized_copy(const SSkeletalMorphEntry* first, const SSkeletalMorphEntry* last, SSkeletalMorphEntry* result, const __false_type&) -- pair<long, CRandomAppearanceMorph::CSkeletalMorph> elements: key + a 12-byte POD block, return the end
// Leaf: ?__uninitialized_copy@std@@YIPAU?$pair@JUCSkeletalMorph@FableCopy_00673ca0@@@FableCopy_00673ca0@@PBU23@0PAU23@ABU__false_type@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00673CA0, length 65 bytes, `ret 8`.
// Element: 16 bytes = pair<long, CRandomAppearanceMorph::CSkeletalMorph>, the entry of the CVectorMap
// CRandomAppearanceMorph::SkeletalMorphs (Ego_r.pdb: CSkeletalMorph = GroupID +0, Name (CDefString, a 4-byte index)
// +4, AllowVariation (bool) +8 -- a 12-byte POD). STLport's pair copy constructor copies the key by register and
// the 12-byte POD as a block, here through re-based registers (`mov ebx,esi ; lea edi,[eax+4]`, data in ebp).
// Callers: 0x00673F90 (vector allocate-and-copy) and 0x00674300 (vector assign); same TU as 0x00673C60.
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` from a /Gr TU (`__fastcall`: first
// in ecx, last in edx, result and the empty tag reference on the stack, `ret 8`): `for (; first != last; ++first,
// ++cur) _Construct(&*cur, *first);` -- a bottom-tested loop behind the `first == last` test, the placement new
// null-tests the destination, the advanced result is returned in eax.
// Label correction: the catalog's `_Uninit_copy<_Vector_const_iterator<_Vector_val<unsigned int,allocator<unsigned int> > >,
// unsigned int*,allocator<unsigned int> >` is a Dinkumware name propagated over all 40 rows of this chunk; retail's STL
// is STLport 4.x and the `ret 8`, the element stride and the placement null test name this body.
inline void* operator new(unsigned int, void* where) { return where; }
struct __false_type { __false_type() { } };
namespace FableCopy_00673ca0 {
struct CDefString {                                     // 4-byte def-name index (POD)
    long m_index;
};
struct CSkeletalMorph {                                 // Ego_r.pdb CRandomAppearanceMorph::CSkeletalMorph, 12 bytes, POD
    long       GroupID;                                 // +0x00
    CDefString Name;                                    // +0x04
    bool       AllowVariation;                          // +0x08
};
template <class T1, class T2>
struct pair {                                           // STLport pair: explicit memberwise copy constructor
    T1 first;
    T2 second;
    pair(const pair& other) : first(other.first), second(other.second) { }
};
typedef pair<long, CSkeletalMorph> SSkeletalMorphEntry;
}
namespace std {
FableCopy_00673ca0::SSkeletalMorphEntry* __fastcall __uninitialized_copy(const FableCopy_00673ca0::SSkeletalMorphEntry* first, const FableCopy_00673ca0::SSkeletalMorphEntry* last,
    FableCopy_00673ca0::SSkeletalMorphEntry* result, const __false_type&)
{
    FableCopy_00673ca0::SSkeletalMorphEntry* cur = result;
    for (; first != last; ++first, ++cur)
        new ((void*)cur) FableCopy_00673ca0::SSkeletalMorphEntry(*first);
    return cur;
}
}