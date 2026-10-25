// _STL::__destroy_aux(CMultiStaticMeshEntryDef* first, CMultiStaticMeshEntryDef* last, const __false_type&) -- destroy each multi-static-mesh entry def in [first, last) through its virtual destructor
// Leaf: ??$__destroy_aux@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DA655, 28 bytes (a /Os module: unaligned entry, the loop entered at its test),
//   /Gr `__fastcall` (first in ecx -> esi, last in edx -> edi), `ret 4` (the tag reference).
// Identity: STLport `__destroy_aux(first, last, __false_type)` at a 0x38 stride with a virtual destructor (slot 0, the
//   scalar deleting destructor called with flag 0). The 56-byte def is CMultiStaticMeshEntryDef (Ego_r.pdb 0x38,
//   vtable 0x012438A4): the caller 0x004EDFCA is vector<CMultiStaticMeshEntryDef>::erase(first, last) next to the landed
//   resize 0x004EDF0A; the other callers (0x004DBDD8, 0x004DD6CD, 0x004DFA74, 0x004EB790, 0x004EB81C) are vectors of other
//   0x38-byte defs whose identical instantiations the linker folded here. Byte twin of the landed CCardDef row
//   0x004DA636 (0x44 stride). The catalog's "vector<CIntelligentPointer<CThing const>>::_Destroy" is propagated.
// Model: engine/CMultiStaticMeshEntryDef.h models the vptr as a data member, which cannot express the virtual
//   destructor call, so the def is modelled locally (polymorphic, 0x38 bytes).
// Shape: `for (; first != last; ++first) _Destroy(&*first);` in a size TU (`jmp` to the bottom test).
#pragma optimize("s", on)
class CMultiStaticMeshEntryDef {                        // Ego_r.pdb, 0x38 bytes (CSubDefClassBase-derived)
public:
    virtual ~CMultiStaticMeshEntryDef();
    unsigned char _body[0x34];                          // +0x04 CResource/CSubDefClassBase state, Mesh +0x28, flags, fade, channel
};

namespace _STL {
struct __false_type { __false_type() { } };

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template void __fastcall __destroy_aux<CMultiStaticMeshEntryDef*>(CMultiStaticMeshEntryDef*, CMultiStaticMeshEntryDef*, const __false_type&);
}