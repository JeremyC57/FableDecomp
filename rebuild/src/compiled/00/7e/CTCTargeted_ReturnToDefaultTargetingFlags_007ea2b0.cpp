#include "engine/CTCTargeted.h"  // retyped onto the PDB layout; byte parity re-verified
class CDefPointeeBase { public: char pad[0x28]; unsigned int defFlags; };
void __fastcall ReturnToDefaultTargetingFlags(CTCTargeted* targeted)
{
  CDefPointeeBase* s = targeted->PTargetingDef_Object;
  if (s)
    targeted->TargetingFlags = s->defFlags;
  else
    targeted->TargetingFlags = 0;
}