// Layout: retail STLport 4.6 container sizes put the PDB member on these bytes; renamed `b114` -> `Landed`.
#include "engine/CCreatureAction_LeapLong.h"  // retyped onto the PDB layout; byte parity re-verified
bool __fastcall IsToEvadeAllHits(CCreatureAction_LeapLong* action){ return action->Landed == 0; }