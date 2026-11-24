// 006eae20 CCombatManager::GetBestSequenceIndexAttackerOnDefender
// Leaf: ?GetBestSequenceIndexAttackerOnDefender@CCombatManager@@ABEJABUCCombatantInfo@@0@Z
// Flags: /d2inlT60
#define FABLE_RETAIL_COMBAT_SEQUENCE_MAGNITUDE_GROUP_PASS27
#define FABLE_RETAIL_COMBAT_SEQUENCE_SELECTION_PASS27
#define FABLE_RETAIL_STLP_VECTOR_LONG_PUSHBACK_OWNER_PASS26
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_ITEM_NE
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_ACCURATE_MAGNITUDE
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS_FIRST
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_NCREATUREMODE_QUERIES
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CGAMECAMERAMANAGER_CAMTOOLS_TYPED
#define FABLE_RETAIL_NCAMERAMODES_CTOOLS_PASS22_LOS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CCOMBATMANAGER
#define FABLE_RETAIL_CCOMBATMANAGER_OUTERRING
#define FABLE_RETAIL_CCOMBATMANAGER_ISDEFENDERDUEFORATTACK
#define FABLE_RETAIL_CTHINGMAP_LAYERINFO
#define FABLE_RETAIL_CTCMAPWHO_PASS22_SHARED_NODE
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_C2DBOXI_COPY
#define FABLE_RETAIL_C2DBOXI_GETWIDTH
#define FABLE_RETAIL_C2DBOXI_GETHEIGHT
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_STATTYPE
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_MAP_VALIDATE
#define FABLE_RETAIL_CTHINGOBJECT_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGBUILDING
#define FABLE_RETAIL_CTHINGBUILDING_TYPEFLAGS
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingObject.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "engine/C2DBoxI.h"
#include "engine/CThingFilter_None.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CTCMapwho.h"
#include "engine/CThingBuilding.h"
#include "engine/CThingSearchTools.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CGameCameraManager.h"
#include "engine/CWorld.h"
#include "engine/CCombatSequenceBase.h"
#include "engine/CCombatManager.h"
#include "engine/NCreatureMode.h"
#include "engine/GFRandom.h"
#include "engine/GFCos.h"

// Inline-budget: native 006EB25F expands the first preference-list selection.
FABLE_FORCEINLINE long CCombatManager::PickBestSequenceFromIndexList(const std::vector<long>& index_list) const
{
    long size=index_list.size();
    long random=GFRandom(size*100,GetWorld().GetWorldSeed());
    random/=100;
    long index=index_list[random];
    return index;
}
long CCombatManager::GetBestSequenceIndexAttackerOnDefender(const CCombatantInfo& attacker_info,const CCombatantInfo& defender_info) const
{
    const CThingCreatureBase& attacker_creature = *attacker_info.Parent;
    const CThingCreatureBase& defender_creature = *defender_info.Parent;
    const std::vector<CCountedPointer<CCombatSequenceBase> >& all_possible_sequences = attacker_info.AttackerInfo.CombatSequences;
    const CCombatSequenceBase* pold_sequence = 0;
    if (attacker_info.AttackerInfo.CurrentSequence >= 0)
        pold_sequence = all_possible_sequences[attacker_info.AttackerInfo.CurrentSequence].Get();
    std::vector<long> correct_zone_is_valid, correct_zone_melee, correct_zone_break_into_melee;
    std::vector<long> correct_zone_boast, correct_zone_outerring, correct_zone_non_melee;
    bool attacker_on_outer_ring = IsCreatureOnOuterRing(attacker_creature);
    bool can_attack = IsDefenderDueForAttack(defender_info);
    if (EnableAINonAttackMode) can_attack = false;
    long c1 = 0;
    for (std::vector<CCountedPointer<CCombatSequenceBase> >::const_iterator it=all_possible_sequences.begin(); it!=all_possible_sequences.end(); ++it, ++c1) {
        CCombatSequenceBase& possible_sequence=**it;
        bool melee_seq=possible_sequence.IsMeleeSequence();
        bool break_into_seq=possible_sequence.IsBreakIntoMeleeSequence();
        if ((melee_seq || break_into_seq) && !can_attack) continue;
        if (!possible_sequence.IsValid() || !possible_sequence.IsZoneValid()) continue;
        if(possible_sequence.IsToPreferSequenceWhenValid()) correct_zone_is_valid.push_back(c1);
        else if(melee_seq) correct_zone_melee.push_back(c1);
        else if(break_into_seq) correct_zone_break_into_melee.push_back(c1);
        else if(possible_sequence.IsBoastSequence()) correct_zone_boast.push_back(c1);
        else correct_zone_non_melee.push_back(c1);
    }
    if(can_attack && defender_creature.IsPlayerCharacter() && (!correct_zone_melee.empty() || !correct_zone_break_into_melee.empty())) {
        C2DBoxI area(((attacker_creature.GetPos()+defender_creature.GetPos())/2.0f).ToC2DCoordI(),
                     (long)((attacker_creature.GetPos()-defender_creature.GetPos()).GetAccurateMagnitude()/2.0f));
        std::vector<const CThing*> things;
        PeekThingSearchTools().GetAllThingsInRoughArea<CThingObject>(area,CThingFilter_None(),things);
        PeekThingSearchTools().GetAllThingsInRoughArea<CThingBuilding>(area,CThingFilter_None(),things);
        const CGameCameraManager* pcam_man=0;
        GetWorld().GetGameCamera()->GetCameraManager(&pcam_man);
        C3DVector from_pos=attacker_creature.GetPos()+C3DVector(0,0,1);
        C3DVector to_pos=defender_creature.GetPos();
        to_pos.Z=from_pos.Z;
        C3DVector coll_pos,coll_normal;
        float cam_rad;
        const CThing* phit_thing;
        if(!pcam_man->GetCameraTools().IsLineOfSight(things,from_pos,to_pos,coll_pos,coll_normal,cam_rad,&phit_thing,NCameraModes::CTools::TEST_THINGS,false)) {
            float cos_theta=coll_normal.Z;
            if((float)fabs(cos_theta)<GFCos(1.0f/6.0f)) can_attack=false;
        }
    }
    bool break_into_melee=false;
    if(pold_sequence && pold_sequence->IsBreakIntoMeleeSequence()) break_into_melee=true;
    bool target_can_be_mocked=NCreatureMode::IsKnockedDown(defender_creature);
    if(pold_sequence && pold_sequence->IsMeleeSequence() && defender_info.DefenderInfo.Attackers.size()>1) can_attack=false;
    long chosen_index=-1;
    if(!correct_zone_is_valid.empty()) chosen_index=PickBestSequenceFromIndexList(correct_zone_is_valid);
    else if(!correct_zone_boast.empty() && target_can_be_mocked) {
// Inline-budget: the four secondary list selections share the retail call at 006EB324.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
        chosen_index=PickBestSequenceFromIndexList(correct_zone_boast);
#ifdef _MSC_VER
#pragma inline_depth()
#endif
    }
    else if(!correct_zone_break_into_melee.empty() && defender_info.DefenderInfo.PLastAttacker!=&attacker_creature && can_attack) {
// Inline-budget: the four secondary list selections share the retail call at 006EB324.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
        chosen_index=PickBestSequenceFromIndexList(correct_zone_break_into_melee);
#ifdef _MSC_VER
#pragma inline_depth()
#endif
    }
    else if(!correct_zone_melee.empty() && (can_attack || break_into_melee)) {
// Inline-budget: the four secondary list selections share the retail call at 006EB324.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
        chosen_index=PickBestSequenceFromIndexList(correct_zone_melee);
#ifdef _MSC_VER
#pragma inline_depth()
#endif
    }
    else if(!correct_zone_non_melee.empty()) {
// Inline-budget: the four secondary list selections share the retail call at 006EB324.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
        chosen_index=PickBestSequenceFromIndexList(correct_zone_non_melee);
#ifdef _MSC_VER
#pragma inline_depth()
#endif
    }
    return chosen_index;
}