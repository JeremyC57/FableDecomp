// CAIStateGroup_JackDragon::CreateIBC -- create and hide the dragon's invisible bodge creature.
// Leaf: ?CreateIBC@CAIStateGroup_JackDragon@@AAEXXZ
// Flags: (none)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_GETTCDRIVER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ORIGINAL_SETPTR_PASS22
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK_PROTECTED
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_JACKDRAGON
#define FABLE_RETAIL_CAISTATEGROUP_JACKDRAGON_CREATEIBC_PASS26
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_DUMMYOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE_ACTIONQUERY
#define FABLE_RETAIL_CTHINGCREATUREBASE_HIDDENMINIMAP_PASS26
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMATION
#define FABLE_RETAIL_CTHINGAICREATURE_CREATE
#define FABLE_RETAIL_CTHINGAICREATUREINIT_CTOR_BODY
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_IS_THING_ALIVE
#include "fable_string.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroup_JackDragon.h"
#include "engine/CAIBrain.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CRightHandedSet.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CThingAICreatureInit.h"
#include "engine/ThingAlive.h"
void CAIStateGroup_JackDragon::CreateIBC()
{
 long bodge_creature_index=CGameDefinitionManager::Get().GetDefGlobalIndexFromName("CREATURE_INVISIBLE_BODGE");
 C3DVector mouth_pos=GetCreature().GetPos();
 CRightHandedSet rhs;
 GetCreature().GetTCGraphicAppearance()->GetDummyObject("DRAGON_MOUTH_DUMMY",0,0,mouth_pos,rhs,1.0f);
 mouth_pos.Z=GetCreature().GetPos().Z;
 C3DVector create_pos=mouth_pos+GetCreature().GetPhysicsTC()->GetFacingVector()*1.0f;
 CThingAICreature* p_ibc=CThingAICreature::Create(bodge_creature_index,create_pos,GetCreature().GetPlayerNumber(),CThingAICreatureInit());
 if(IS_THING_ALIVE(p_ibc)) {
  p_ibc->RemoveTC(TCI_SOUND_PLAYER);
  p_ibc->SetHiddenOnMiniMap(true);
  PInvisibleBodgeCreature=CIntelligentPointer<CThingAICreature>(p_ibc);
 }
}