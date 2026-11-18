// CTCTargetingPlayer::UpdateTargetThreatening() -- when the player (Z-targeting or first-person targeting) targets an AI creature that is not an enemy willing to attack and is aware of the player, post a threaten deed (large with a loaded projectile weapon in the primary slot, small otherwise) and remember the threatened thing; with no target clear it
// Leaf: ?UpdateTargetThreatening@CTCTargetingPlayer@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00803280, 551 bytes, plain `ret`.
// Ego_d.pdb: { ptargeted_creature (CThingAICreature*) { ptc_deedlog (CTCHeroOpinionDeedLog*), ptc_carrying (const CTCCarrying*)
//   { pcarried_thing (CThing*), ptc_weapon (const CTCWeapon*) { threaten_deed_type (EOpinionDeedType) } } } }.
// Include / opt-in set: the landed same-TU sibling CTCTargetingPlayer::UpdateProjectileTargeting 0x00803D60.
#define FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
#define FABLE_RETAIL_CBASEOBJECT_ORIGINAL_ACCESS
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_SUBTRACT_ASSIGN
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTCENEMY_QUERIES
#define FABLE_RETAIL_EXPRESSION_FRAME
#define FABLE_RETAIL_CTHING_PEEKPLAYER
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCSTOCKITEM_TARGETING
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_RAW_NOT_EQUAL
#define FABLE_RETAIL_CINTELLIGENTPOINTER_EQUAL_RAW
#define FABLE_RETAIL_UPDATE_NEAREST_INTERESTING
#define FABLE_RETAIL_CTCTARGETINGPLAYER
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_LASTPOSITION
#define FABLE_RETAIL_CTCTARGETED
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CGAMECAMERAMANAGER_LOOKAROUND
#define FABLE_RETAIL_CGAMECAMERAMANAGER_GETCAMERA
#define FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_PROJECTILE
#define FABLE_RETAIL_CCAMERA_FOV
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#define FABLE_RETAIL_C2DVECTOR_SQUAREDMAGNITUDE_YX
#define FABLE_RETAIL_NCAMERAMODES_CBASE_CYLINDER
#define FABLE_RETAIL_NCAMERAMODES_CBASE_LOOKATPHASE_INLINE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_MAP_VALIDATE
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_FILTER_BIND3_FUNCTION
#define FABLE_RETAIL_STLP_FOR_EACH_BODY
#define FABLE_RETAIL_STLP_FIND_RANDOM_DISPATCH
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCAL
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_STATIC_FOCAL
#define FABLE_RETAIL_CTCTARGETINGPLAYER_FRAMEUPDATE
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#define FABLE_RETAIL_NCREATUREAI_WILLING_TO_ATTACK
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCWEAPON_READYPROJECTILE
#define FABLE_RETAIL_CTCWEAPON_LOADING
#include "engine/EPlayerMode.h"
#include "engine/CThing.h"
#include "engine/ThingAlive.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCTargetingPlayer.h"
#include "engine/CTargetingDef.h"
#include "engine/CPlayer.h"
#include "engine/CGamePlayerInterface.h"
#include "engine/CWorld.h"
#include "engine/CCamera.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CGameCameraManager.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCTargeted.h"
#include "engine/CTCCarrying.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CTCMapwho.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CThingFilter_EnemyCreature.h"
#include "engine/CTCEnemy.h"
#include "engine/C2DBoxI.h"
#include "engine/CTCAIScratchpad.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CTCStockItem.h"
#include "engine/TargetingPlayerSupport.h"
#include "engine/NCameraModes_CBase.h"
#include "engine/CThingFilter_IsInFOV.h"
#include "engine/CThingFilter_In2DRange.h"
#include "engine/C2DVector.h"
#include "stlport/targeting_algorithms.h"
extern const float DELTA_NOTIONAL_ZERO;
#include "engine/CMainGameComponent.h"
#include "engine/CTCWeapon.h"
#include "engine/CTCHeroOpinionDeedLog.h"
#include "engine/EOpinionPostDeedType.h"
#include "engine/NCreatureAI.h"

void CTCTargetingPlayer::UpdateTargetThreatening()
{
    if (GetPTargetedThing()) {
        if (GetPTargetedThing()->GetType() == 1 && (GetParentThing().GetPlayer().ZTargeting || GetParentThing().GetPlayer().IsFirstPersonTargeting())) {
            CThingAICreature* ptargeted_creature = (CThingAICreature*)GetPTargetedThing();
            if ((!CTCEnemy::AreThingsEnemies(*GetPTargetedThing(), *ParentThing) ||
                 !NCreatureAI::IsWillingToAttackTarget(ptargeted_creature, ParentThing)) &&
                GetPTargetedThing()->HasTC(TCI_CREATURE_OPINION_OF_HERO) &&
                NCreatureAI::IsAwareOfThing(ptargeted_creature, ParentThing, true)) {
                CTCHeroOpinionDeedLog* ptc_deedlog;
                const CTCCarrying* ptc_carrying;
                if (GetParentThing().GetTC(&ptc_deedlog) && GetParentThing().PeekTC(&ptc_carrying)) {
                    CThing* pcarried_thing = ptc_carrying->GetThingInPrimarySlot();
                    const CTCWeapon* ptc_weapon;
                    if (pcarried_thing && pcarried_thing->PeekTC(&ptc_weapon)) {
                        EOpinionDeedType threaten_deed_type = (ptc_weapon->IsProjectileWeapon() && ptc_weapon->IsProjectileWeaponLoaded()) ?
                            OPINION_DEED_TYPE_EXPRESSION_THREATEN_LARGE : OPINION_DEED_TYPE_EXPRESSION_THREATEN_SMALL;
                        ptc_deedlog->PostDeed(threaten_deed_type, GetParentThing().GetPos(), GetPTargetedThing(), ptargeted_creature,
                                              OPINION_POST_DEED_TYPE_RECIPIENT, false, false, 0, false, 0);
                        PLastThreatenedThing = GetPTargetedThing();
                    }
                }
            }
        }
    } else {
        PLastThreatenedThing = 0;
    }
}
