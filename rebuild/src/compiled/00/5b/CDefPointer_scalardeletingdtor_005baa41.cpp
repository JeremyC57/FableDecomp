// CDefPointer<const CAppearanceModifierDef>::`scalar deleting destructor'(unsigned int flags) -- release the counted def (Destroy when its count reaches zero), clear the slot, then free the wrapper when bit 0 of the flags is set
// Leaf: ??_G?$CDefPointer@$$CBVCAppearanceModifierDef@@@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005baa41, 42 bytes, thiscall, `ret 4`, returns this (size-optimised TU: unaligned address).
// Types (proven, agentFAM11, 2026-11-12): CDefPointer<const CAppearanceModifierDef>. Sources: (1) retail: the landed real-typed
//   std::_List_base<CDefPointer<const CAppearanceModifierDef> >::clear 0x005BB190 destroys each element through this ??_G and its relocation
//   binds exactly this spelling (rebuild/oracles/reloc_bindings.tsv); (2) FableWin.pdb / Ego_r.pdb carry the CDefPointer<const
//   CAppearanceModifierDef> instantiation (??_G / ??1 and the list of it). Was the placeholder CDefPointer<const CUnprovenDef_005baa41>;
//   name-only change, the same body (the existing local CDefPointer model is kept as landed; no new model is added).
// Callees: the def's Destroy() (vtable slot 1), operator delete 0x00BFE9BC.
// Shape (family L012): `#pragma optimize("s", on)`; ~CDefPointer must be __forceinline (an /Os TU otherwise calls it out of
//   line from the ??_G), the pointee's inline Release() gives `dec [ecx+4] ; jne`, `Object = 0` is `and [esi],0`.
#include "fable_compiler.h"
#pragma optimize("s", on)
class CDefPointeeBase {                                    // vptr; reference count at +4
public:
    virtual ~CDefPointeeBase();
    virtual void Destroy() const;                         // slot 1
    mutable long RefCount;                                // +0x04
    FABLE_FORCEINLINE void Release() const
    {
        if (--RefCount == 0)
            Destroy();
    }
};
class CAppearanceModifierDef;                              // the proven def type (engine/CAppearanceModifierDef.h; incomplete here)
template <class T> class CDefPointer {                     // Ego_r.pdb, 4 bytes
public:
    const CDefPointeeBase* Object;                        // +0x00
    FABLE_FORCEINLINE ~CDefPointer()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
};
// The delete expression that makes VC7.1 emit the scalar deleting destructor COMDAT.
void DeleteDefPointer_005baa41(CDefPointer<const CAppearanceModifierDef>* pointer) { delete pointer; }