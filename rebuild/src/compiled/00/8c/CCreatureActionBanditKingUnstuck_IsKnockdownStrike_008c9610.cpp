// Layout: retail STLport 4.6 container sizes put the PDB member on these bytes; renamed `f` -> `Knockdown`.
#include "engine/CCreatureAction_BanditKingUnstuck.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCreatureAction_BanditKingUnstuck_Methods : CCreatureAction_BanditKingUnstuck {
    unsigned char M();
};
unsigned char CCreatureAction_BanditKingUnstuck_Methods::M(){ return this->Knockdown; }