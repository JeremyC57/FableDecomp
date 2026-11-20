// NUISystem::CComponent::SetDeletion(CDeletion deletion) -- replace the component's deletion record (method and associated parents) with a copy of the argument
// Leaf: ?SetDeletion@CComponent@NUISystem@@UAEXVCDeletion@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00531E90, 38 bytes (48 with padding), virtual, `ret 8`.
// Vtable 0x0122F5D4 (NUISystem::CSprite, inherited from CComponent) slot 62; Ego_r.pdb declares
// `virtual void CComponent::SetDeletion(CDeletion)` -- the record is passed BY VALUE (8 bytes in retail: the
// EDeletionMethod plus a 4-byte STLport list<CComponent*>), hence `ret 8`.
// Callees: list<CComponent*>::operator= 0x00535800 (manifest "std::list::splice"; the body is the element-wise
//   assign of an STLport list), ~list<CComponent*> 0x0042ABCA (the by-value parameter is destroyed by the callee).
// Shape: the implicit CDeletion::operator= copies Method as a dword then calls the list's operator=; the by-value
//   parameter's implicit destructor then runs on its own argument slot (`lea ecx,[esp+8]`).
// Layout: retail CComponent Deletion at +0xd4 (Ego_r.pdb +0xe4; the retail list is 4 bytes where the PDB has 12).
// NF156 (2026-11-20, q4776): the local CComponentList / CDeletion / CComponent models replaced by the shared headers: CComponent with
//   its typed Deletion member (+0xd4, FABLE_RETAIL_NUISYSTEM_CCOMPONENT_MEMBERS, engine/NUISystem_CComponent.h), CDeletion
//   (engine/NUISystem_CDeletion.h: Method + STLport list<CComponent*> AssociatedParents) and the list's operator= declared out of line
//   (FABLE_RETAIL_STLP_LIST_ASSIGN_POPBACK_DECL, stlport/fable_containers.h) and ~list kept out of line with its body
//   visible (FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE, as the landed list<CComponent*> callers 0x00583815 / 0x005A887D). 0x0042ABCA is now bound as ~list<CComponent*> (was
//   ??1CComponentList@NUISystem@@QAE@XZ) and 0x00535800 as the self row's list<CComponent*>::operator= (was ??4CComponentList@NUISystem@@..).
#define FABLE_RETAIL_NUISYSTEM_CCOMPONENT_MEMBERS
#define FABLE_RETAIL_STLP_LIST_ASSIGN_POPBACK_DECL
#define FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CDeletion.h"

void NUISystem::CComponent::SetDeletion(CDeletion deletion)
{
    Deletion = deletion;
}
