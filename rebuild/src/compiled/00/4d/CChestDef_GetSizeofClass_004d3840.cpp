// Leaf: ?GetSizeofClass@CChestDef@@UAEKXZ
// Class-model move (NF322, 2026-11-23): defined on the real class of engine/CChestDef.h under FABLE_RETAIL_CCHESTDEF_VIRTUAL_MODEL (the opt-in polymorphic model;
//   was CChestDef_Methods::GetSizeofClass); the retail RTTI vftable placement names this PDB member here.
#pragma optimize("s",on)
// Constant-return member: returns 60. __fastcall this=ecx.
#define FABLE_RETAIL_CCHESTDEF_VIRTUAL_MODEL
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CChestDef.h"  // retyped onto the PDB layout; byte parity re-verified
unsigned long CChestDef::GetSizeofClass() { return 60; }
