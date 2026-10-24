// std::__uninitialized_copy(const STextureMorphEntry* first, const STextureMorphEntry* last, STextureMorphEntry* result, const __false_type&) -- pair<unsigned long, CRandomAppearanceMorph::CTextureMorph> elements: key + two POD words, return the end
// Leaf: ?__uninitialized_copy@std@@YIPAU?$pair@KUCTextureMorph@FableCopy_00673c60@@@FableCopy_00673c60@@PBU23@0PAU23@ABU__false_type@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00673C60, length 53 bytes, `ret 8`.
// Element: 12 bytes = pair<unsigned long, CRandomAppearanceMorph::CTextureMorph>, the entry of the CVectorMap
// CRandomAppearanceMorph::GeneralTextureMorphs / CBodyPartMesh::TextureMorphs (Ego_r.pdb: CTextureMorph = NewTexture
// +0, GroupID +4, a POD). STLport's pair copy constructor copies the key by register and the 8-byte POD as a block,
// which VC7.1 does through the strength-reduced &first->second (`lea esi,[ecx+4]`) and direct [eax+4]/[eax+8] stores.
// The vector copy constructor 0x00673E10 inlines the same loop; the neighbouring 0x00673D30 (landed) is this TU's
// pair<long, CEntry> copy.
// Callers: 0x006741E0 (vector assign).
// Byte twin: 0x00673CF0 (separate COMDAT copy; distinct element namespace keeps one symbol per address).
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` from a /Gr TU (`__fastcall`: first
// in ecx, last in edx, result and the empty tag reference on the stack, `ret 8`): `for (; first != last; ++first,
// ++cur) _Construct(&*cur, *first);` -- a bottom-tested loop behind the `first == last` test, the placement new
// null-tests the destination, the advanced result is returned in eax.
// Label correction: the catalog's `_Uninit_copy<_Vector_const_iterator<_Vector_val<unsigned int,allocator<unsigned int> > >,
// unsigned int*,allocator<unsigned int> >` is a Dinkumware name propagated over all 40 rows of this chunk; retail's STL
// is STLport 4.x and the `ret 8`, the element stride and the placement null test name this body.
inline void* operator new(unsigned int, void* where) { return where; }
struct __false_type { __false_type() { } };
namespace FableCopy_00673c60 {
struct CTextureMorph {                                  // Ego_r.pdb CRandomAppearanceMorph::CTextureMorph, 8 bytes, POD
    unsigned long NewTexture;                           // +0x00
    unsigned long GroupID;                              // +0x04
};
template <class T1, class T2>
struct pair {                                           // STLport pair: explicit memberwise copy constructor
    T1 first;
    T2 second;
    pair(const pair& other) : first(other.first), second(other.second) { }
};
typedef pair<unsigned long, CTextureMorph> STextureMorphEntry;
}
namespace std {
FableCopy_00673c60::STextureMorphEntry* __fastcall __uninitialized_copy(const FableCopy_00673c60::STextureMorphEntry* first, const FableCopy_00673c60::STextureMorphEntry* last,
    FableCopy_00673c60::STextureMorphEntry* result, const __false_type&)
{
    FableCopy_00673c60::STextureMorphEntry* cur = result;
    for (; first != last; ++first, ++cur)
        new ((void*)cur) FableCopy_00673c60::STextureMorphEntry(*first);
    return cur;
}
}