// Leaf: ?CTCTargeted_ReturnToDefaultTargetingFlags@@YIXPAVCTCTargeted@@@Z
#include "engine/CTCTargeted.h"  // retyped onto the PDB layout; byte parity re-verified
class CDefPointeeBase { public: char pad[0x28]; unsigned int defFlags; };
void __fastcall CTCTargeted_ReturnToDefaultTargetingFlags(CTCTargeted* targeted)
{
  CDefPointeeBase* s = targeted->PTargetingDef_Object;
  if (s)
    targeted->TargetingFlags = s->defFlags;
  else
    targeted->TargetingFlags = 0;
}