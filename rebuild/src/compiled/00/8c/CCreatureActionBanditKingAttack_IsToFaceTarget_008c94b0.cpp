#include "engine/CCreatureAction_BanditKingAttack.h"  // retyped onto the PDB layout; byte parity re-verified
#include "rebuild_abi.h"
struct CCreatureAction_BanditKingAttack_Methods : CCreatureAction_BanditKingAttack {
    char IsToFaceTarget();
};
char CCreatureAction_BanditKingAttack_Methods::IsToFaceTarget(){ return (char)FaceTarget; }  // Layout: retail STLport 4.6 container sizes put the PDB member on these bytes; renamed `m_v` -> `FaceTarget` (+0x114).