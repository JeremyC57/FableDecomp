#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CVillageMemberDef.h"  // retyped onto the PDB layout; byte parity re-verified

#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)


void __fastcall CVillageMemberDef_Copy(CVillageMemberDef* self, void* edx, const CDefClassBase* src)
{
  reinterpret_cast<CDefClassBase*>(self)->operator=(*(const CDefClassBase*)(src));
  self->NeedsVillageToFunctionCorrectly = reinterpret_cast<const CVillageMemberDef*>(src)->NeedsVillageToFunctionCorrectly;
}