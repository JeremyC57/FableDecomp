// CTCCreatureModeManager::CModeBase::OnDeactivate() -- the mode is no longer active (Active = false)
// Leaf: ?OnDeactivate@CModeBase@CTCCreatureModeManager@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00739E60, 5 bytes (`mov byte ptr [ecx+18h],0 ; ret`); CModeBase vtable 0x012673CC slot 6 and the mode vtables
//   that do not override it (14 .rdata references, 0x012673E4 .. 0x01293010).
// Name: Ego_r.pdb 0x0051C5E0 + FableWin.pdb ?OnDeactivate@CModeBase@CTCCreatureModeManager@@UAEXXZ (public virtual, slot 6; engine/CTCCreatureModeManager.h
//   FABLE_RETAIL_CMODEBASE_VIRTUALS). Was a local `struct T { char pad[0x18]; char field; void Clear(); }` model (leaf ?Clear@T@@QAEXXZ).
// One definition (NF90, q11189, coordinator ruling): the body is inline in retail -- NCreatureMode::CModeIntangible::OnDeactivate 0x008E74F0 expands
//   its qualified CModeBase::OnDeactivate() call -- so it lives once in the header (NF90 opt-in FABLE_RETAIL_CMODEBASE_ONDEACTIVATE_INLINE) and this
//   row, like 0x008E74F0, only includes it. VC7.1 emits an inline function out of line only where it is referenced and not expanded (retail: the
//   vtables); the scaffolding below makes this object carry it as a pick-any COMDAT (selection 2), so it links with every includer.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODEBASE_ONDEACTIVATE_INLINE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCCreatureModeManager.h"

namespace {
// Fidelity: scaffolding, not retail: a non-virtual (qualified) call of the header's inline OnDeactivate with inline expansion switched off for this
// one function, so this object emits the body out of line as a pick-any COMDAT (retail's 0x00739E60, the vtables' copy). The address of a virtual
// member is a vcall thunk and would not reference the body. Internal to this TU; never called.
#pragma inline_depth(0)
void ReferenceModeBaseOnDeactivate(CTCCreatureModeManager::CModeBase& mode)
{
    mode.CTCCreatureModeManager::CModeBase::OnDeactivate();
}
#pragma inline_depth()
}
