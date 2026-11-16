// 006e99b0 CCombatManager::GetDefenderZone
// Flags: none
// Leaf: ?GetDefenderZone@CCombatManager@@QBE?AW4ECombatZone@@ABVCThingCreatureBase@@0J@Z
#define FABLE_RETAIL_COMBAT_DEFENDER_ZONE_PASS23
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_GETCOMBATTYPEDEF
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_LASTPOSITION
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCOMBATMANAGER
#define FABLE_RETAIL_CCOMBATMANAGER_OUTERRING
#define FABLE_RETAIL_CCOMBATTYPEDEF
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ACCURATE_MAGNITUDE
#define FABLE_RETAIL_C3DVECTOR_CROSSWITHUP
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_SQUARE_ORIGINAL_FORMAL
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
#define FABLE_RETAIL_C3DVECTOR_NONZERO_SQUARED_XYZ
#define FABLE_RETAIL_C3DVECTOR_DOT_XY_THEN_Z
#define FABLE_RETAIL_STLP_TREE_MFIND_DECL
#define FABLE_RETAIL_STLP_TREE_CONSTFIND
#include "fable_compiler.h"
#include "engine/ECombatZone.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CCombatTypeDef.h"
#include "engine/CCombatManager.h"
#include "engine/GFCos.h"
// Original template signature in both PDBs; donor0241c049 uses<float,float>.
template<class T,class L>inline void FABLE_FASTCALL GFLimitLower(T& value,const L& lower){if(value<lower)value=lower;}

ECombatZone CCombatManager::GetDefenderZone(const CThingCreatureBase& attackee,const CThingCreatureBase& attacker,long prediction_frames)const
{
    if(IsAttacker(attacker) && IsCreatureOnOuterRing(attacker))
        return COMBAT_ZONE_OUTER_RING;
    // Fidelity: group all five floating call results; measured81-tuple boundary then
    // falls before the Y load, reproducing native6e9af4 Z-zero store without new operations.
    const float FRONT_FAR_LIMIT_SQR=(GFSqr(attacker.GetCombatTypeDef().FrontFarLimit+0.6f));
    const float FRONT_MIDDLE_LIMIT_SQR=(GFSqr(attacker.GetCombatTypeDef().FrontMiddleLimit));
    const float FRONT_NEAR_LIMIT_SQR=(GFSqr(attacker.GetCombatTypeDef().FrontNearLimit));
    const float REAR_LIMIT_SQR=(GFSqr(attacker.GetCombatTypeDef().RearLimit+0.6f));
    const float SIDE_LIMIT_SQR=(GFSqr(attacker.GetCombatTypeDef().SideLimit+0.6f));
    const CTCPhysicsBase* ptc_phys=attackee.GetPhysicsTC();
    C3DVector velocity=ptc_phys->GetPosition()-ptc_phys->GetLastPosition();
    C3DVector target_pos=ptc_phys->GetPosition();
    C3DVector predicted_position=target_pos+velocity*(float)prediction_frames;
    C3DVector facing_direction=attacker.GetPhysicsTC()->GetFacingVector();
    C3DVector perp=facing_direction.CrossWithUp();
    C3DVector between=predicted_position-attacker.PeekPos();
    between.Z=0.0f;
    float distance=between.GetAccurateMagnitude();
    if(distance>DELTA_NOTIONAL_ZERO)between/=distance;
    C3DVector static_between=target_pos-attacker.PeekPos();
    static_between.Z=0.0f;
    if(static_between.IsNonZero())static_between.AccurateNormalise();
    distance-=attackee.GetPhysicsTC()->GetRadius();
    GFLimitLower(distance,0.0f);
    float sqr_distance=GFSqr(distance);
    if(between.Dot(static_between)<0.0f && sqr_distance<SIDE_LIMIT_SQR)
        return between.Dot(perp)<0.0f?COMBAT_ZONE_LEFT:COMBAT_ZONE_RIGHT;
    float cos_theta=between.Dot(facing_direction);
    if(cos_theta>GFCos(0.125f)) {
        if(sqr_distance<FRONT_NEAR_LIMIT_SQR)return COMBAT_ZONE_FRONT_NEAR;
        if(sqr_distance<FRONT_MIDDLE_LIMIT_SQR)return COMBAT_ZONE_FRONT_MIDDLE;
        if(sqr_distance<FRONT_FAR_LIMIT_SQR)return COMBAT_ZONE_FRONT_FAR;
        return COMBAT_ZONE_NO_ZONE;
    }
    if(cos_theta<GFCos(0.375f)) {
        if(sqr_distance<REAR_LIMIT_SQR)return COMBAT_ZONE_REAR;
        return COMBAT_ZONE_NO_ZONE;
    }
    if(sqr_distance<SIDE_LIMIT_SQR)
        return between.Dot(perp)<0.0f?COMBAT_ZONE_LEFT:COMBAT_ZONE_RIGHT;
    return COMBAT_ZONE_NO_ZONE;
}