// CFactionManager::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the inline destructor destroys the pair<unsigned long, CCountedPointer<CFaction> > range of its vector through the out-of-line __destroy_aux and frees the storage, then the object is freed when bit 0 of the flags is set
// Leaf: ??_GCFactionManager@@QAEPAXI@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AE4D0, 55 bytes (+ int3 padding to 64), thiscall `ret 4`, returns this.
// Identity (proven): CFactionManager::`scalar deleting destructor'; evidence: its callee 0x004AD9E0 is __destroy_aux<pair<unsigned long, CCountedPointer<CFaction> >*> (the landed vector<pair<unsigned long, CCountedPointer<CFaction> > >::_M_insert_overflow 0x006BAA90 calls it), the element type of Ego_r.pdb CFactionManager::Factions (CVectorMap<unsigned long, CCountedPointer<CFaction> >, the class's only member); the landed CWorld::~CWorld 0x004A6AB0 deletes the CWorld::PFactionManager scoped_ptr<CFactionManager> through the same inline body (0x004A6D13 calls 0x004AD9E0 then operator delete)
// Label: the manifest's "`scalar_deleting_destructor'" is propagated (family H064: six deleting destructors that differ only in the
//   __destroy_aux they call; retail has no identical-code folding, so each address is one class's COMDAT). No call or
//   data reference reaches this address: VC7.1 emits the ??_G COMDAT for every `delete p` and explicit `p->~T()` of a
//   class with a destructor (checked with a two-line probe), inlines it at the site, and the retail link kept it.
// Callees: std::__destroy_aux<std::pair<unsigned long, CCountedPointer<CFaction> >*> @ 0x004AD9E0 (`__fastcall` first/last, the empty __false_type tag by reference,
//   homed on the flags argument's slot: `lea eax,[esp+8]`), free 0x00BFEA14 (the null-tested
//   ~_Vector_base deallocate), operator delete 0x00BFE9BC.
// Layout: local models (no engine header for these classes); STLport 4.6 vector = 12 bytes.
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
void operator delete(void* block);                      // 0x00BFE9BC

#include "engine/CVectorMap.h"

class CFaction;
#include "engine/CCountedPointer.h"
class CFactionManager {                               // 16 bytes in retail (Ego_r.pdb: one member)
public:
    CVectorMap<unsigned long, CCountedPointer<CFaction>, CKeyPairCompareLess<unsigned long, CCountedPointer<CFaction> > > Factions;   // +0x0
};

// Same-TU stand-in for the out-of-line __destroy_aux 0x004AD9E0: a plain function above the caller under auto_inline(off)
// that never touches the tag, so VC7.1 homes the empty tag temporary on the live flags slot as retail does (a declared-only
// callee opens a `push ecx` frame word; a template stand-in is inlined despite the pragma -- GOTCHAS 2026-10-20).
namespace std {
#pragma auto_inline(off)
void __fastcall __destroy_aux(std::pair<unsigned long, CCountedPointer<CFaction> >* __first, std::pair<unsigned long, CCountedPointer<CFaction> >* __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(__first);
}
#pragma auto_inline(on)
}

// scoped_ptr<CFactionManager>::~scoped_ptr / reset (CWorld::PFactionManager): the delete that emits the COMDAT.
void DeleteFactionManager_004ae4d0(CFactionManager* p) { delete p; }