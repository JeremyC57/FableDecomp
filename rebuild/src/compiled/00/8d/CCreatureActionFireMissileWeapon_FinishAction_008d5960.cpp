// Leaf: ?FinishAction@CCreatureAction_FireMissileWeapon@@UAEXXZ
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_FIREMISSILEWEAPON
#include "engine/CIntelligentPointer.h"
#include "engine/CCreatureAction_FireMissileWeapon.h"
extern void __fastcall dv_h1_008d58d0(void*);
extern void __fastcall dv_h2_00694430(void*);
void CCreatureAction_FireMissileWeapon::FinishAction()
{
    void* action = (void*)this; dv_h1_008d58d0(action); dv_h2_00694430(action); }
// Flatten route (flatten_pilot v4, NF324): CCreatureAction_FireMissileWeapon_FinishAction -> CCreatureAction_FireMissileWeapon::FinishAction, the RTTI-placed PDB member; class model: engine/CCreatureAction_FireMissileWeapon.h under FABLE_RETAIL_CCREATUREACTION_FIREMISSILEWEAPON (preamble of CCreatureActionFireMissileWeapon_FrameUpdate_008d5000.cpp).
