// Layout: retail STLport 4.6 container sizes put the PDB member on these bytes; renamed `f` -> `StillUnderground`.
#include "engine/CCreatureAction_UndeadRiseUp.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCreatureAction_UndeadRiseUp_Methods : CCreatureAction_UndeadRiseUp {
    unsigned char M();
};
unsigned char CCreatureAction_UndeadRiseUp_Methods::M(){ return this->StillUnderground; }