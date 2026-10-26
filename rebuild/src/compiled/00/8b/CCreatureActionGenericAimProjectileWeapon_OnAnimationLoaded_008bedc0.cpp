#include "engine/CCreatureAction_GenericAimProjectileWeapon.h"  // retyped onto the PDB layout; byte parity re-verified
class CCreatureAction_GenericAimProjectileWeapon; void __fastcall oal_sub(CCreatureAction_GenericAimProjectileWeapon* action);
struct CCreatureAction_GenericAimProjectileWeapon_Methods : CCreatureAction_GenericAimProjectileWeapon {
    void OnAnimationLoaded();
};
void CCreatureAction_GenericAimProjectileWeapon_Methods::OnAnimationLoaded(){ oal_sub(this); this->f10 = 7; }