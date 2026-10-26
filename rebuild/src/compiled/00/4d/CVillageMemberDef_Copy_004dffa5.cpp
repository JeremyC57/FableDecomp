#include "engine/CVillageMemberDef.h"  // retyped onto the PDB layout; byte parity re-verified

class CDefClassBase { public:
  CDefClassBase& operator=(const CDefClassBase& other);  // 0x431f10, __fastcall
};


void __fastcall CVillageMemberDef_Copy(CVillageMemberDef* self, void* edx, const CDefClassBase* src)
{
  reinterpret_cast<CDefClassBase*>(self)->operator=(*(const CDefClassBase*)(src));
  self->NeedsVillageToFunctionCorrectly = reinterpret_cast<const CVillageMemberDef*>(src)->NeedsVillageToFunctionCorrectly;
}