// Leaf: ?Copy@CVillageMemberDef@@UAEXPBVCDefClassBase@@@Z
#define FABLE_RETAIL_CVILLAGEMEMBERDEF_VIRTUAL_MODEL
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CVillageMemberDef.h"  // retyped onto the PDB layout; byte parity re-verified

#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)


void CVillageMemberDef::Copy(const class CDefClassBase* src)
{
    CVillageMemberDef* self = this;
  reinterpret_cast<CDefClassBase*>(self)->operator=(*(const CDefClassBase*)(src));
  self->NeedsVillageToFunctionCorrectly = reinterpret_cast<const CVillageMemberDef*>(src)->NeedsVillageToFunctionCorrectly;
}
// Flatten route (flatten_pilot v6, NF335): CVillageMemberDef_Copy -> CVillageMemberDef::Copy, the RTTI-placed PDB member; class model: engine/CVillageMemberDef.h under FABLE_RETAIL_CVILLAGEMEMBERDEF_VIRTUAL_MODEL.
