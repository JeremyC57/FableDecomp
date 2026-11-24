// 006ebf70 CDefendingCombatantInfo::CCombatRing::AssignAttackersToRing
// Leaf: ?AssignAttackersToRing@CCombatRing@CDefendingCombatantInfo@@QAEXABVCWorldMap@@ABVCCombatWheel@2@AAV?$map@PAVCThingCreatureBase@@PAVCAttackingCombatantInfo@@U?$less@PAVCThingCreatureBase@@@std@@V?$allocator@U?$pair@QAVCThingCreatureBase@@PAVCAttackingCombatantInfo@@@std@@@4@@std@@ABVCThingCreatureBase@@@Z
// Flags:
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_NAMED_SQUARED
#include <math.h>
#define FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE
#define FABLE_RETAIL_COMBATRING_ASSIGN_PASS27
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_STLP_TREE_POSTINC
#define FABLE_RETAIL_STLP_MAP_EMPTY
#define FABLE_RETAIL_STLP_TREE_ERASE_KEY_DECL_ONLY
#define FABLE_RETAIL_STLP_FOR_EACH_BODY
#define FABLE_RETAIL_GFANGLES_SIGNED_ROUNDING
#define FABLE_RETAIL_FREE_PRONG_PASS26
#define FABLE_RETAIL_CCOMBATRING_ISOUTERRING
#define FABLE_RETAIL_STLP_TREE_ERASE
#define FABLE_RETAIL_STLP_TREE_ERASE_BODY
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CDEFENDINGCOMBATANTINFO_PRONG
#define FABLE_RETAIL_CDEFENDINGCOMBATANTINFO_PASS22_INIT
#define FABLE_RETAIL_CCOMBATTYPEDEF
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCCOMBAT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_GETCOMBATTYPEDEF
#define FABLE_RETAIL_CTCCOMBAT_PASS22_UNSHEATHING
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCombatTypeDef.h"
#include "engine/CAttackingCombatantInfo.h"
#include "engine/ECombatAbilityDataType.h"
#include "engine/CTCCombat.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCWeapon.h"
#include "engine/CDefendingCombatantInfo.h"
#include "engine/GFAngles.h"
#include "engine/CGetBestFreeProngForAttacker.h"
#include "engine/CGetBestAttackerForProng.h"
#include "stlport/fable_algorithm.h"
inline bool CDefendingCombatantInfo::CCombatRing::IsOuterRing(const CCombatTypeDef& defender_combat_type_def) const
{
    return Index >= defender_combat_type_def.DefenderWheelOuterRingStartIndex;
}

void CDefendingCombatantInfo::CCombatRing::AssignAttackersToRing(const CWorldMap& wmap,const CCombatWheel& wheel,
    std::map<CThingCreatureBase*,CAttackingCombatantInfo*>& attackers,const CThingCreatureBase& defender)
{
    typedef std::map<CThingCreatureBase*,CAttackingCombatantInfo*> AttackerMap;
    const CCombatTypeDef& def_cmbt_type=defender.GetCombatTypeDef();
    bool outer_ring=IsOuterRing(def_cmbt_type);
    std::vector<CProng>::iterator it_usable_end=std::partition(Prongs.begin(),Prongs.end(),CIsProngUsable());
    long num_assign_attackers=attackers.size();
    long num_free_prongs=it_usable_end-Prongs.begin();
    if(num_free_prongs==0)return;
    if(Index<def_cmbt_type.DefenderWheelOuterRingStartIndex && attackers.size()==1 && !IsRotating()) {
        const CThingCreatureBase& one_true_attacker=*attackers.begin()->first;
        const CProng& one_true_prong=*Prongs.begin();
        C3DVector preferred_prong_vector=one_true_attacker.GetPos()-defender.GetPos();
        preferred_prong_vector.Z=0.0f;
        if(preferred_prong_vector.IsNonZero())preferred_prong_vector.AccurateNormalise();
        C3DVector prong_vector=one_true_prong.GetProngVector(0.0f);
        float cos_thet=prong_vector.Dot(preferred_prong_vector);
        if(cos_thet < -1.0f)cos_thet=-1.0f;
        else if(cos_thet > 1.0f)cos_thet=1.0f;
        float new_rotation_angle=GFArcCos(cos_thet);
        C3DVector perp=prong_vector.Cross(preferred_prong_vector);
        if(perp.Z>0.0f)new_rotation_angle*=-1.0f;
        RotationAngle=new_rotation_angle;
    }
    if(num_assign_attackers>=num_free_prongs){
        for(std::vector<CProng>::iterator it=Prongs.begin();it!=it_usable_end && num_assign_attackers!=0;++it){
            CProng& prong=*it;
            CThingCreatureBase* pbest_attacker=std::for_each(attackers.begin(),attackers.end(),
                CGetBestAttackerForProng(wmap,wheel,*this,prong,defender)).GetPBestAttacker();
            if(pbest_attacker){
                AttackerMap::const_iterator it_look=attackers.find(pbest_attacker);
                CAttackingCombatantInfo* pattacker_inf=it_look->second;
                AssignAttackerToProng(&prong,pbest_attacker,pattacker_inf,outer_ring);
                attackers.erase(pbest_attacker);
                --num_free_prongs;
                --num_assign_attackers;
            }
        }
    } else {
        for(AttackerMap::iterator it=attackers.begin();it!=attackers.end();){
            CThingCreatureBase* pattacker=it->first;
            CAttackingCombatantInfo* pattacker_inf=it->second;
            CProng* pbest_prong=std::for_each(Prongs.begin(),it_usable_end,
                CGetBestFreeProngForAttacker(wmap,wheel,*this,*pattacker,defender,*pattacker_inf)).GetPBestProng();
            if(pbest_prong){
                AssignAttackerToProng(pbest_prong,pattacker,pattacker_inf,outer_ring);
                attackers.erase(it++);
                --num_assign_attackers;
                --num_free_prongs;
            }else ++it;
        }
    }
}