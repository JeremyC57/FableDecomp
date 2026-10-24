// std::__uninitialized_copy(const CGameMapCell* first, const CGameMapCell* last, CGameMapCell* result, const __false_type&) -- CGameMapCell elements (7-byte POD): block-copy dword + word + byte per element, return the end
// Leaf: ?__uninitialized_copy@std@@YIPAUCGameMapCell@FableCopy_006b7b70@@PBU23@0PAU23@ABU__false_type@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B7B70, length 58 bytes, `ret 8`.
// Element: CGameMapCell, the 7-byte element of CGameMap::Cells (Ego_r.pdb: AtmosThemes uchar[3] +0, AtmosBlends
// uchar[2] +3, SoundTheme schar +5, Music uchar +6; the only 7-byte record in the PDB). The implicit copy of the
// byte-aligned POD is an inline 7-byte block copy (dword, word, byte through copies of the element pointers).
// Callers: 0x006B7CC0 / 0x006B7E20 (this vector's fill-insert and assignment) and 0x006B7F70 / 0x006B8050.
// Byte twin: 0x006B7BB0 (separate COMDAT copy; distinct element namespace keeps one symbol per address).
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` from a /Gr TU (`__fastcall`: first
// in ecx, last in edx, result and the empty tag reference on the stack, `ret 8`): `for (; first != last; ++first,
// ++cur) _Construct(&*cur, *first);` -- a bottom-tested loop behind the `first == last` test, the placement new
// null-tests the destination, the advanced result is returned in eax.
// Label correction: the catalog's `_Uninit_copy<_Vector_const_iterator<_Vector_val<unsigned int,allocator<unsigned int> > >,
// unsigned int*,allocator<unsigned int> >` is a Dinkumware name propagated over all 40 rows of this chunk; retail's STL
// is STLport 4.x and the `ret 8`, the element stride and the placement null test name this body.
inline void* operator new(unsigned int, void* where) { return where; }
struct __false_type { __false_type() { } };
namespace FableCopy_006b7b70 {
struct CGameMapCell {                                   // Ego_r.pdb, 7 bytes, POD
    unsigned char AtmosThemes[3];                       // +0x00
    unsigned char AtmosBlends[2];                       // +0x03
    signed char   SoundTheme;                           // +0x05
    unsigned char Music;                                // +0x06
};
}
namespace std {
FableCopy_006b7b70::CGameMapCell* __fastcall __uninitialized_copy(const FableCopy_006b7b70::CGameMapCell* first, const FableCopy_006b7b70::CGameMapCell* last,
    FableCopy_006b7b70::CGameMapCell* result, const __false_type&)
{
    FableCopy_006b7b70::CGameMapCell* cur = result;
    for (; first != last; ++first, ++cur)
        new ((void*)cur) FableCopy_006b7b70::CGameMapCell(*first);
    return cur;
}
}