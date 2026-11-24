#define FABLE_RETAIL_CJACKOFBLADESBATTLEDEF_TYPENAME_PASS26
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_ORIGINAL_SCOPES_PASS22
// Leaf: ?OnActivate@CModeJackOfBladesFlash@NCreatureMode@@UAEXXZ
// Flags: (none)
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_PDCB_TYPED_MEMBERS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODEBASE_PARENTTHING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_ENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT_LIGHTINGFLASH
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_DUMMYOBJECT
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_CREATE
#define FABLE_RETAIL_C3DVECTOR
#include "engine/CDefString.h"
#include "engine/CVectorMap.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingBaseDef.h"
#include "engine/CDefPointer.h"
#include "engine/NCreatureMode_CModeJackOfBladesFlash.h"
#include "engine/CJackOfBladesBattleDef.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CMainGameComponent.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CWorld.h"
#include "engine/CEnvironment.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/CRightHandedSet.h"
#include "engine/CDefClassBase_GetSubDef.h"
namespace {extern long JACK_FLASH_DURATION,JACK_POWERUP_DURATION,JACK_FADE_UP_TIME,JACK_TOTAL_DURATION,JACK_GLOW_START_FRAME,JACK_FADE_DOWN_TIME;}
void NCreatureMode::CModeJackOfBladesFlash::OnActivate()
{
 CDefPointer<const CJackOfBladesBattleDef> p_jack_def;
 GetParentThing().PeekBaseDef().GetSubDef(&p_jack_def);
 JACK_FLASH_DURATION=(long)(CMainGameComponent::GetConstantFPS()*p_jack_def->FlashDuration);
 JACK_POWERUP_DURATION=(long)(CMainGameComponent::GetConstantFPS()*p_jack_def->FlashBuildUpTime);
 JACK_TOTAL_DURATION=JACK_FLASH_DURATION+JACK_POWERUP_DURATION;
 JACK_GLOW_START_FRAME=JACK_TOTAL_DURATION-JACK_POWERUP_DURATION;
 JACK_FADE_DOWN_TIME=JACK_POWERUP_DURATION;
 JACK_FADE_UP_TIME=10;
 CTCCreatureModeManager::CModeBase::OnActivate();
 Timer=JACK_POWERUP_DURATION+JACK_FLASH_DURATION;
 BurnCounter=0;
 CDefPointer<const CSpecialEffectsDef> pspecial_fx;
 long fx_index=0;
 if(GetParentThing().PeekBaseDef().GetSubDef(&pspecial_fx)&&pspecial_fx->GetEffectFromString("FLASH_SWORD_POWER_UP",fx_index)) {
  C3DVector weapon_pos=PCreature->GetPos();
  CRightHandedSet rhs;
  PCreature->GetTCGraphicAppearance()->GetDummyObject("WEAPON_FOCUS_01",0,0,weapon_pos,rhs,1.0f);
  CThing* pEmitter=CTCDParticleEmitter::Create(fx_index,weapon_pos,false);
  if(pEmitter)PEmitter=CIntelligentPointer<CThing>(pEmitter);
 }
 long glow_def_num=CGameDefinitionManager::Get().GetDefGlobalIndexFromName("ENVIRONMENT_JACK_OF_BLADES_FADE");
 GetWorld().GetEnvironment().TransitionToThemeAllChannels(3,glow_def_num,(float)JACK_FADE_DOWN_TIME/30.0f);
}