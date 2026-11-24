// CGroundNavigatorExternalInterface::GetTerrainDesirabilityAt @ 0x006CD9B0
// Retail:
//   fld dword ptr ds:[0x122ded8]   ; return global float
//   ret 4                          ; __fastcall: this in ecx, one 4-byte stack arg
//
// The function ignores 'this' and its single argument and just returns a
// global float value. Model it as a __fastcall member with one 4-byte
// parameter so the compiler emits 'ret 4'.

// Leaf: ?GetTerrainDesirabilityAt@CGroundNavigatorExternalInterface@@UBEMABVC3DVector@@@Z
// Class-model move (NF322, 2026-11-23): defined on the real class of engine/CGroundNavigatorExternalInterface.h under FABLE_RETAIL_CGROUNDNAVIGATOREXTERNALINTERFACE_VIRTUAL_MODEL (the opt-in polymorphic model;
//   was CGroundNavigatorExternalInterface_Methods::GetTerrainDesirabilityAt); the retail RTTI vftable placement names this PDB member here.
#define FABLE_RETAIL_CGROUNDNAVIGATOREXTERNALINTERFACE_VIRTUAL_MODEL
#include "engine/CGroundNavigatorExternalInterface.h"  // retyped onto the PDB layout; byte parity re-verified

float CGroundNavigatorExternalInterface::GetTerrainDesirabilityAt(const C3DVector& pos) const {
    (void)pos;
    return 1.0f;
}
