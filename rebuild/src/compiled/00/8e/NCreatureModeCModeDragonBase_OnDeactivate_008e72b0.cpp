// NCreatureMode::CModeDragonBase::OnDeactivate() -- the dragon is no longer flying: gravity back on (the physics component's slot 38,
//   EnableGravity(true)), then the base deactivation (CModeBase::OnDeactivate, Active = false)
// Leaf: ?OnDeactivate@CModeDragonBase@NCreatureMode@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E72B0, 25 bytes, `ret`; CModeDragonBase vtable slot 6 (.rdata 0x012684F8, the only reference: no
//   direct call anywhere in .text).
// Name: Ego_r.pdb 0x0090EE50 + FableWin.pdb ?OnDeactivate@CModeDragonBase@NCreatureMode@@UAEXXZ (public virtual, slot 6; engine/CModeDragonBase.h
//   FABLE_RETAIL_CMODEDRAGONBASE_MODEBASE). Was a local July model (`struct CModeDragonBase { void* pad0; Holder* holder; ... }`, Inner with
//   s00..s37 placeholder virtuals) as the free __fastcall CModeDragonBase_OnDeactivate (no Leaf).
// One definition (NF92, q11224): NCreatureMode::CModeIntangible::OnDeactivate 0x008E74F0 expands its qualified CModeDragonBase::OnDeactivate()
//   call, so the body lives once in the header (NF92 opt-in FABLE_RETAIL_CMODEDRAGONBASE_ONDEACTIVATE_INLINE, on NF90's inline
//   CModeBase::OnDeactivate) and this row, like 0x008E74F0, only includes it; the scaffolding below makes this object carry the vtable's
//   out-of-line copy as a pick-any COMDAT (the nf86 / nf88 pattern, NF90's 0x00739E60). Link proof: link/ (NOTES).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODEBASE_ONDEACTIVATE_INLINE
#define FABLE_RETAIL_CMODEDRAGONBASE_MODEBASE
#define FABLE_RETAIL_CMODEDRAGONBASE_ONDEACTIVATE_INLINE
#define FABLE_RETAIL_CTCPHYSICSBASE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CModeDragonBase.h"

namespace {
// Fidelity: scaffolding, not retail: a non-virtual (qualified) call of the header's inline OnDeactivate with inline expansion switched off for this
// one function, so this object emits the body out of line as a pick-any COMDAT (retail's 0x008E72B0, the vtable's copy). The address of a virtual
// member is a vcall thunk and would not reference the body. Internal to this TU; never called.
#pragma inline_depth(0)
void ReferenceDragonBaseOnDeactivate(NCreatureMode::CModeDragonBase& mode)
{
    mode.NCreatureMode::CModeDragonBase::OnDeactivate();
}
#pragma inline_depth()
}
