// CDefPointer<const CAppearanceModifierDef>::`scalar deleting destructor'(unsigned int flags) -- release the counted def (Destroy when its count reaches zero), clear the slot, then free the wrapper when bit 0 of the flags is set
// Leaf: ??_G?$CDefPointer@$$CBVCAppearanceModifierDef@@@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005baa41, 42 bytes, thiscall, `ret 4`, returns this (size-optimised TU: unaligned address).
// Types (proven, agentFAM11, 2026-11-12): CDefPointer<const CAppearanceModifierDef>. Sources: (1) retail: the landed real-typed
//   std::_List_base<CDefPointer<const CAppearanceModifierDef> >::clear 0x005BB190 destroys each element through this ??_G and its relocation
//   binds exactly this spelling (rebuild/oracles/reloc_bindings.tsv); (2) FableWin.pdb / Ego_r.pdb carry the CDefPointer<const
//   CAppearanceModifierDef> instantiation (??_G / ??1 and the list of it). Was the placeholder CDefPointer<const CUnprovenDef_005baa41>;
//   name-only change, the same body.
// Model (FAM12): ported off the landed local CDefPointer / CDefPointeeBase model onto engine/CDefPointer.h + engine/CIVCountedPointeeBase.h
//   (FABLE_RETAIL_DEF_CLASS_BASE, FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE, FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES:
//   the pointee's inline Release deletes through slot 1 Delete at zero).
// Callees: the def's Delete() (CIVCountedPointeeBase vtable slot 1), operator delete 0x00BFE9BC.
// Shape (family L012): `#pragma optimize("s", on)`; ~CDefPointer is force-inlined (the header FABLE_DEFPTR_INLINE) (an /Os TU otherwise calls it out of
//   line from the ??_G), the pointee's inline Release() gives `dec [ecx+4] ; jne`, `Object = 0` is `and [esi],0`.
#include "fable_compiler.h"
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#include "engine/CDefPointer.h"
class CAppearanceModifierDef;                              // the proven def type (engine/CAppearanceModifierDef.h; incomplete here)
// The delete expression that makes VC7.1 emit the scalar deleting destructor COMDAT.
void DeleteDefPointer_005baa41(CDefPointer<const CAppearanceModifierDef>* pointer) { delete pointer; }
