// 008591e0 CCombatActionBase::PerformPossibleDamageAction
// Flags: none
#define FABLE_RETAIL_CFILTER_BIND4_FORCEINLINE
#define FABLE_RETAIL_CTHING_SCANINCLUSION_ISALIVE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY_EGOD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_C2DBOXI_COPY
#define FABLE_RETAIL_CTCMAPWHO_STATTYPE
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHINGGAMEOBJECT_STATTYPEFLAGS
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_C2DBOXI_GETWIDTH
#define FABLE_RETAIL_C2DBOXI_GETHEIGHT
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY_TYPEMASK
// Leaf: ?PerformPossibleDamageAction@CCombatActionBase@@IAEXM@Z
#define FABLE_RETAIL_DAMAGE_ACTION_PASS29
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTIONBASE_PRECEEDING
#define FABLE_RETAIL_CCREATUREACTIONBASE_PEEKTHINGSEARCHTOOLS
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE_OWN_VIRTUALS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ISDEAD_DIRECT
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_GETTC_DONOR_NAMES_PASS23
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_TO2D
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DCOORDI
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_CTORS
#define FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS
#define FABLE_RETAIL_CREGION
#define FABLE_RETAIL_CREGION_PREGIONDEF
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCCARRYING
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C2DCoordI.h"
#include "engine/C2DVector.h"
#include "engine/C3DVector.h"
#include "engine/C2DBoxI.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"
#include "engine/CThing.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CDefPointer.h"
#include "engine/CRegionDef.h"
#include "engine/CRegion.h"
#include "engine/CWorldMap.h"
#include "engine/CTCWeapon.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCWeaponTrail.h"
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#define FABLE_RETAIL_THING_TYPE_IDS
#define FABLE_RETAIL_CCLASSFACTORY
#include "engine/CThingManager.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CFilter_Bind4.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CTCHitLocations.h"
#include "engine/GFSqr.h"
#include "engine/CThingFilter_CombatOverlappingSphere.h"
namespace {
class CThingFilter_CombatIsStrikeToStrike : public std::unary_function<CThing,bool> {
public:
    CThingFilter_CombatIsStrikeToStrike(const CThing& thing_striking) : ThingStriking(thing_striking) {}
    bool operator()(const CThing& test_thing) const;                    // 0x008565E0
private:
    const CThing& ThingStriking;                                        // +0x00
};
class CThingFilter_CombatCanCollideOrInteractWithThing : public std::unary_function<CThing,bool> {
public:
    CThingFilter_CombatCanCollideOrInteractWithThing(const CTCPhysicsBase& tc_physics, bool exclude_dead_creatures, bool exclude_creatures)
        : TCPhysics(tc_physics), ExcludeCreatures(exclude_creatures), ExcludeDeadCreatures(exclude_dead_creatures) {}
    bool operator()(const CThing& test_thing) const;                    // 0x00857250
private:
    const CTCPhysicsBase& TCPhysics;                                    // +0x00
    bool ExcludeCreatures;                                              // +0x04
    bool ExcludeDeadCreatures;                                          // +0x05
};
}
namespace {
void FABLE_FASTCALL ProcessWeaponTrailPointsToIncludeCylinderCloseToCreatureAxis(std::vector<std::pair<C3DVector,C3DVector> >&,const CThing&);
}
void CCombatActionBase::PerformPossibleDamageAction(float attack_point_radius)
{
 if(IS_THING_ALIVE(PCreature)) {
  CThingCreatureBase& attacker=*PCreature;
  bool perform_extended_check=(attacker.GetType()==THING_TYPE_PLAYER_CREATURE);
  std::vector<std::pair<C3DVector,C3DVector> > trail_list;
  std::list<CThingGameObject*> thing_list;
  float target_radius; if(IS_THING_ALIVE(PTarget)) target_radius=PTarget->GetPhysicsTC()->GetRadius(); else target_radius=0.0f;
  float creature_radius=PCreature->GetPhysicsTC()->GetRadius();
  float actual_radius=((creature_radius+1.0f)*2.0f+target_radius)*1.2f;
  long region_index=GetWorldMap().GetLoadedRegion();
  const CRegion& region=GetWorldMap().GetRegion(region_index);
  bool hit_dead_creatures=false;
  if(IsToHitProneThings())hit_dead_creatures=true;
  CThingFilter_IsNot filter1(*PCreature);
  CThingFilter_CombatIsStrikeToStrike filter2(*PCreature);
  CThingFilter_CombatOverlappingSphere filter3(attacker.GetPos(),(creature_radius+5.0f+target_radius)*1.25f);
  CThingFilter_CombatCanCollideOrInteractWithThing filter4(*PCreature->GetPhysicsTC(),!hit_dead_creatures,false);
  CFilter_Bind4<CThingFilter_IsNot,CThingFilter_CombatIsStrikeToStrike,CThingFilter_CombatOverlappingSphere,CThingFilter_CombatCanCollideOrInteractWithThing> filter_main(filter1,filter2,filter3,filter4);
  CDefPointer<const CRegionDef> pregion_def=region.GetPRegionDef();
  if(pregion_def.IsNull()||!pregion_def->CheckAllThingsOnMapForCombatCollision){
   C2DBoxI area(PCreature->PeekPos().ToC2DCoordI(),(long)actual_radius);
   PeekThingSearchTools().GetAllThingsInRoughArea<CThingGameObject>(area,filter_main,thing_list);
  }else PeekThingSearchTools().GetAllThings<CThingGameObject>(filter_main,thing_list);
  if(thing_list.empty())return;
  long strike_event_index;
  CTCWeaponTrail* tc_weapon_trail=GetWeaponTrailUsedInStrikeAction(strike_event_index);
  if(tc_weapon_trail){
   tc_weapon_trail->GetLastWeaponTrailPoints(trail_list,strike_event_index);
   const CTCCarrying* ptc_carrying;
   if(PCreature->PeekTC(&ptc_carrying)){
    CThing* pthing=ptc_carrying->GetThingInPrimarySlot();
    const CTCWeapon* ptc_weapon;
    if(IS_THING_ALIVE(pthing)&&pthing->PeekTC(&ptc_weapon)&&ptc_weapon->IsMeleeWeapon())
     ProcessWeaponTrailPointsToIncludeCylinderCloseToCreatureAxis(trail_list,*PCreature);
   }
   if(false){}else for(std::list<CThingGameObject*>::iterator it=thing_list.begin();it!=thing_list.end();++it){
    CThingGameObject& possible_hit_thing=**it;
    if(false){}else for(long c1=1;c1<trail_list.size();++c1){
     long type_flags=possible_hit_thing.GetTypeFlags();
     if(type_flags&8)ProcessPossibleCreatureStrikeEvent(attack_point_radius,trail_list[c1-1],trail_list[c1],possible_hit_thing);
     else if(IsToHitProneThings())ProcessPossibleProneThingsStrikeEvent(attack_point_radius,trail_list[c1-1],trail_list[c1],possible_hit_thing);
     else if(type_flags&0x80)continue;
     else if(possible_hit_thing.HasTC(TCI_SHOT_DEFINITION))ProcessPossibleShotStrikeEvent(attack_point_radius,trail_list[c1-1],trail_list[c1],possible_hit_thing);
     else ProcessPossibleObjectStrikeEvent(attack_point_radius,trail_list[c1-1],trail_list[c1],possible_hit_thing);
    }
   }
  }
 }
}