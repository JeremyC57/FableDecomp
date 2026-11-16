// CTCLook::IsOverriddenByParent() const -- whether the parent thing's creature mode overrides look turning
// (NCreatureMode::IsLookTurningOverridenByParent 0x0085F200 on ParentThing +0x04)
// Leaf: ?IsOverriddenByParent@CTCLook@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00751A70.
// NF39 (2026-11-16): moved off its free __fastcall stand-in (EditWorldUpdate over a local S / Sub) onto the shared engine/CTCLook.h:
//   Ego_r.pdb + FableWin.pdb ?IsOverriddenByParent@CTCLook@@QBE_NXZ (the header declares it); the callee is Ego_r.pdb / FableWin.pdb
//   ?IsLookTurningOverridenByParent@NCreatureMode@@YI_NABVCThing@@@Z (Ego_r 0x00833530; a free __fastcall taking the thing in ecx);
//   its two calls 0x00716E68 / 0x00716EE4 are on the TCI_LOOK component and the landed caller binds this symbol. Preamble from the
//   landed CTCLook::HasFrameUpdate 0x00755310.
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CLOOKDEF
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NOT_EQUAL
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_LOOK_HEAD_ANIM
#include "engine/CTCLook.h"

namespace NCreatureMode {
bool FABLE_FASTCALL IsLookTurningOverridenByParent(const CThing& thing);   // 0x0085F200
}

bool CTCLook::IsOverriddenByParent() const
{
    return NCreatureMode::IsLookTurningOverridenByParent(GetParentThing()) ? true : false;
}
