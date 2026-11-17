// CAIStateGroup_JackOfBladesStage2::OnActivate() -- enlist Jack against his target with the combat manager and switch the three hit-location enabled states (body and force field off, the big one on); reset the speech counter
// Leaf: ?OnActivate@CAIStateGroup_JackOfBladesStage2@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00972490, 254 bytes. Callees: CAIStateGroupBase::GetWorld 0x008FCFC0 (CWorld +0x4c combat manager),
//   CBaseIntelligentPointer::GetPItem 0x00A01B50 (PTargetThing +0x24), CCombatManager::RegisterCombatant 0x006EDC50, CVectorMap::LowerBound
//   0x0040F020 (inline GetTC TCI_HIT_LOCATIONS 127), CCharString 0x0099EBF0 / 0x0099EAE0, CTCHitLocations::GetPHitLocationWithDefName 0x00815820,
//   NHitLocations::CHitLocation::SetEnabled 0x00816390.
// NF65 (round 7): ported onto the shared headers with the macro block of the landed sibling Update 0x00974AA0 (CAIStateGroupBase retail
//   GetCreature / GetWorld, CAIStateGroup_JackOfBladesBase PTargetThing +0x24 / SpeechCounter +0x30, CWorld::GetCombatManager, CTCHitLocations
//   WITHDEFNAME, NHitLocations::CHitLocation, overlay opt-in FABLE_RETAIL_JACK_STAGE2_ONACTIVATE). The JackMethods / JackThing / JackTable /
//   HitLocations / HitLocation / CCombatManager / JackPointer stand-ins and the renaming-macro include of engine/CAIStateGroupBase.h are gone;
//   0x00815820 now binds ?GetPHitLocationWithDefName@CTCHitLocations@@QAEPAVCHitLocation@NHitLocations@@ABVCCharString@@@Z (nf41:e15).
#define FABLE_RETAIL_CTCHITLOCATIONS_WITHDEFNAME
#define FABLE_RETAIL_CHITLOCATION_SETENABLED
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CAISTATEGROUP_JACKOFBLADESBASE
#define FABLE_RETAIL_JACKOFBLADESBASE_STAGE2_UPDATE
#define FABLE_RETAIL_JACKOFBLADESBASE_SUMMONPILLARS
#define FABLE_RETAIL_JACKOFBLADESBASE_SUMMONPILLARSOLD
#define FABLE_RETAIL_CTCBOSS_ISREADYFORPHASECHANGE
#define FABLE_RETAIL_CTCBOSS_KILLHELPERS
#define FABLE_RETAIL_CTCBOSS_LIVINGHELPERS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_GFANGLES_GET_ROUNDED_ANGLE_FORCEINLINE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_FORCEINLINE_CTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_INLINE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGPHYSICAL_HITSETTERS
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCCOMBAT
#define FABLE_RETAIL_CTCCOMBAT_SHOTOVERRIDE
#define FABLE_RETAIL_CCOMBATABILITY
#define FABLE_RETAIL_CCOMBATABILITYDATA
#define FABLE_RETAIL_CTCTALK
#define FABLE_RETAIL_CTCTALK_AISPEECHTEXT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_SCALE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_SPEECH
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_COMBATMANAGER
#define FABLE_RETAIL_JACK_STAGE2_UPDATE
#define FABLE_RETAIL_JACK_STAGE2_ONACTIVATE
#define FABLE_RETAIL_CCREATUREACTION_PLAYCOMBATANIMATION
#define FABLE_RETAIL_NCREATUREMODE_QUERIES
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCCombat.h"
#include "engine/CTCTalk.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefinitionManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CWorld.h"
#include "engine/CThingBaseDef.h"
#include "engine/CMainGameComponent.h"
#include "engine/GFAngles.h"
#include "engine/CJackOfBladesBattleDef.h"
#include "engine/CTCBoss.h"
#include "engine/CTCWillResponse.h"
#include "engine/CAIStateGroup_JackOfBladesBase.h"
#include "engine/CTCHitLocations.h"
#include "engine/NHitLocations_CHitLocation.h"
#include "engine/CCombatManager.h"
#include "engine/CAIStateGroup_JackOfBladesStage2.h"
#include "engine/CCreatureAction_JackFloatUp.h"
#include "engine/CCreatureAction_JackShotFloat.h"
#include "engine/CCreatureAction_JackFlash.h"
#include "engine/CCreatureAction_PlayCombatAnimation.h"
#include "engine/NCreatureMode.h"
#include "engine/fable_ai_action_enums.h"

void CAIStateGroup_JackOfBladesStage2::OnActivate()
{
    GetWorld().GetCombatManager().RegisterCombatant(GetCreature(), *PTargetThing.Get(), false);
    CTCHitLocations* hits;
    GetCreature().GetTC(TCI_HIT_LOCATIONS, &hits);
    NHitLocations::CHitLocation* location = hits->GetPHitLocationWithDefName("HIT_LOCATION_JACK_OF_BLADES");
    location->SetEnabled(false);
    location = hits->GetPHitLocationWithDefName("HIT_LOCATION_JACK_OF_BLADES_FORCEFIELD");
    location->SetEnabled(false);
    location = hits->GetPHitLocationWithDefName("HIT_LOCATION_JACK_OF_BLADES_BIG");
    location->SetEnabled(true);
    SpeechCounter = 0;
}
