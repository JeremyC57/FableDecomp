// CCachedVolumes::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the inline destructor destroys the pair<long, CCountedPointer<CContainmentVolume> > range of its vector through the out-of-line __destroy_aux and frees the storage, then the object is freed when bit 0 of the flags is set
// Leaf: ??_GCCachedVolumes@@QAEPAXI@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00827470, 55 bytes (+ int3 padding to 64), thiscall `ret 4`, returns this.
// Identity (proven): CCachedVolumes::`scalar deleting destructor'; evidence: its callee 0x00825AE0 is the __destroy_aux CMap::InitCachedVolumes 0x00821850 runs on the old object when it resets the member at +0x08 with a `new` 0x10-byte object (three zeroed words, Dirty +0xD cleared) and deletes the old one through the same inline body; Ego_r.pdb CMap::CachedVolumes (+8) is scoped_ptr<CCachedVolumes> and CCachedVolumes is one CVectorMap<long, CCountedPointer<CContainmentVolume> > NavigationContainmentVolumes
// Label: the manifest's "`scalar_deleting_destructor'" is propagated (family H064: six deleting destructors that differ only in the
//   __destroy_aux they call; retail has no identical-code folding, so each address is one class's COMDAT). No call or
//   data reference reaches this address: VC7.1 emits the ??_G COMDAT for every `delete p` and explicit `p->~T()` of a
//   class with a destructor (checked with a two-line probe), inlines it at the site, and the retail link kept it.
// Callees: std::__destroy_aux<std::pair<long, CCountedPointer<CContainmentVolume> >*> @ 0x00825AE0 (`__fastcall` first/last, the empty __false_type tag by reference,
//   homed on the flags argument's slot: `lea eax,[esp+8]`), free 0x00BFEA14 (the null-tested
//   ~_Vector_base deallocate), operator delete 0x00BFE9BC.
// Layout: local models (no engine header for these classes); STLport 4.6 vector = 12 bytes.
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
void operator delete(void* block);                      // 0x00BFE9BC

#include "engine/CVectorMap.h"

class CContainmentVolume;
#include "engine/CCountedPointer.h"
class CCachedVolumes {                                // 16 bytes in retail (Ego_r.pdb: one member)
public:
    CVectorMap<long, CCountedPointer<CContainmentVolume>, CKeyPairCompareLess<long, CCountedPointer<CContainmentVolume> > > NavigationContainmentVolumes;   // +0x0
};

// Same-TU stand-in for the out-of-line __destroy_aux 0x00825AE0: a plain function above the caller under auto_inline(off)
// that never touches the tag, so VC7.1 homes the empty tag temporary on the live flags slot as retail does (a declared-only
// callee opens a `push ecx` frame word; a template stand-in is inlined despite the pragma -- GOTCHAS 2026-10-20).
namespace std {
#pragma auto_inline(off)
void __fastcall __destroy_aux(std::pair<long, CCountedPointer<CContainmentVolume> >* __first, std::pair<long, CCountedPointer<CContainmentVolume> >* __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(__first);
}
#pragma auto_inline(on)
}

// scoped_ptr<CCachedVolumes>::reset (CMap::CachedVolumes): the delete that emits the COMDAT.
void DeleteCachedVolumes_00827470(CCachedVolumes* p) { delete p; }