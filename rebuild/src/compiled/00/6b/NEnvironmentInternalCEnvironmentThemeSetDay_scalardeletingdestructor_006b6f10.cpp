// NEnvironmentInternal::CEnvironmentThemeSetDay::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the inline destructor destroys the CBlendedEnvironmentTheme range of its vector through the out-of-line __destroy_aux and frees the storage, then the object is freed when bit 0 of the flags is set
// Leaf: ??_GCEnvironmentThemeSetDay@NEnvironmentInternal@@QAEPAXI@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B6F10, 55 bytes (+ int3 padding to 64), thiscall `ret 4`, returns this.
// Identity (proven): NEnvironmentInternal::CEnvironmentThemeSetDay::`scalar deleting destructor'; evidence: its callee 0x006B67A0 is the landed __destroy_aux<CBlendedEnvironmentTheme*> (0x1F4-byte elements, Ego_r.pdb layout); 0x006B6FF0, the __destroy_aux over 12-byte elements that runs the same inline vector destructor per element, is __destroy_aux<CEnvironmentThemeSetDay*> (Ego_r.pdb: CEnvironmentThemeSetDay is one CArray<NEnvironmentInternal::CBlendedEnvironmentTheme> Themes; CEnvironmentThemeResourcePallette::DaySets is CArray<CEnvironmentThemeSetDay>; FableWin.pdb names CArray<NEnvironmentInternal::CEnvironmentThemeSetDay>), and its explicit `p->~CEnvironmentThemeSetDay()` is what emits this COMDAT
// Label: the manifest's "`scalar_deleting_destructor'" is propagated (family H064: six deleting destructors that differ only in the
//   __destroy_aux they call; retail has no identical-code folding, so each address is one class's COMDAT). No call or
//   data reference reaches this address: VC7.1 emits the ??_G COMDAT for every `delete p` and explicit `p->~T()` of a
//   class with a destructor (checked with a two-line probe), inlines it at the site, and the retail link kept it.
// Callees: std::__destroy_aux<NEnvironmentInternal::CBlendedEnvironmentTheme*> @ 0x006B67A0 (`__fastcall` first/last, the empty __false_type tag by reference,
//   homed on the flags argument's slot: `lea eax,[esp+8]`), free 0x00BFEA14 (the null-tested
//   ~_Vector_base deallocate), operator delete 0x00BFE9BC.
// Layout: local models (no engine header for these classes); STLport 4.6 vector = 12 bytes.
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
void operator delete(void* block);                      // 0x00BFE9BC

#include "engine/CArray.h"

namespace NEnvironmentInternal {
class CBlendedEnvironmentTheme {                      // 0x1F4 bytes in retail (TimeOfDay, Colours[19], Scalers[30], TextureSets[3])
public:
    unsigned char Data[0x1F4];
    ~CBlendedEnvironmentTheme();
};
}
namespace NEnvironmentInternal {
class CEnvironmentThemeSetDay {                        // 12 bytes in retail (Ego_r.pdb: one member)
public:
    CArray<CBlendedEnvironmentTheme> Themes;            // +0x0
};
}

// Same-TU stand-in for the out-of-line __destroy_aux 0x006B67A0: a plain function above the caller under auto_inline(off)
// that never touches the tag, so VC7.1 homes the empty tag temporary on the live flags slot as retail does (a declared-only
// callee opens a `push ecx` frame word; a template stand-in is inlined despite the pragma -- GOTCHAS 2026-10-20).
namespace std {
#pragma auto_inline(off)
void __fastcall __destroy_aux(NEnvironmentInternal::CBlendedEnvironmentTheme* __first, NEnvironmentInternal::CBlendedEnvironmentTheme* __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(__first);
}
#pragma auto_inline(on)
}

// __destroy_aux<CEnvironmentThemeSetDay*> (retail 0x006B6FF0, CArray<CEnvironmentThemeSetDay> DaySets): the explicit
// destructor call that emits the COMDAT.
void DestroyThemeSetDay_006b6f10(NEnvironmentInternal::CEnvironmentThemeSetDay* p) { p->~CEnvironmentThemeSetDay(); }