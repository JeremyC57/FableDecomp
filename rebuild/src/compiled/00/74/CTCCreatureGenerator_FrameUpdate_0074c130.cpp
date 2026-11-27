// 0074c130 CTCCreatureGenerator::FrameUpdate
// Leaf: ?FrameUpdate@CTCCreatureGenerator@@UAEXXZ
// Flags: none
#define FABLE_RETAIL_STLP_FIND_RANDOM_DISPATCH
#define FABLE_RETAIL_CINTELLIGENTPOINTER_EQUAL_MIXED
#define FABLE_RETAIL_CTHING_LOADSINGLE
#define FABLE_RETAIL_CTHING_ISPLACED
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_GENERATOR_FRAME_PASS29
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_WORLDSEED
#define FABLE_RETAIL_CTCCREATUREGENERATOR
#define FABLE_RETAIL_CTCCREATUREGENERATOR_UNDEAD
#define FABLE_RETAIL_CTCCREATUREGENERATOR_TRIGGERERS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_DRAWABLE
#define FABLE_RETAIL_CTHING_GETTC_DONOR_NAMES_PASS23
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCTARGETED
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_CAPPEARANCEDEF_SIZE_BODY
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_SETMAINGRAPHIC
#define FABLE_RETAIL_CAPPEARANCEDEF_COMPLETE
#define FABLE_RETAIL_CDEFSTRING
#define FABLE_RETAIL_CDEFSTRING_NOTEQUAL
#define FABLE_RETAIL_CDEFSTRING_OUTOFLINE_CONVERSION
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_CREATE
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_REMOVE_DECL
#define FABLE_RETAIL_GFRANDOM_OUTOFLINE
#include "fable_string.h"
#include "engine/CDefIndex.h"
#include "engine/CArray.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CPlayer.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/CTCDegradableBase.h"
#include "engine/CTCTargeted.h"
#include "engine/CAppearanceDef.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CCreatureGeneratorDef.h"
#include "engine/CTCCreatureGenerator.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/ThingAlive.h"
#include "engine/GFRandom.h"
#define FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
#include "stlport/targeting_algorithms.h"
extern bool DrawCreatureGeneratorDebug;
// 00668b60 owns the speed-built pointer constructor. Its 0123e7d8
// vtable selects the size-built deleting destructor at 004d6915.
template<> inline CIntelligentPointer<CThingCreatureBase>::CIntelligentPointer(CThingCreatureBase* pointer) { SetPointer(pointer); }
#ifdef _MSC_VER
#pragma optimize("s",on)
#endif
template CIntelligentPointer<CThingCreatureBase>::CIntelligentPointer(CThingCreatureBase*);
#ifdef _MSC_VER
#pragma optimize("t",on)
#endif
void CTCCreatureGenerator::FrameUpdate()
{
    if (!PeekWorld().IsCreatureGenerationEnabled(GetParentThing()))
        return;
    if (!Destroyed) {
        const CTCDegradableBase* ptc_degradable;
        if (GetParentThing().PeekTC(&ptc_degradable) && ptc_degradable->IsFullyDegraded()) {
            Destroyed = true;
            CTCTargeted* ptc_targeted;
            if (GetParentThing().GetTC(&ptc_targeted)) ptc_targeted->SetTargetable(false);
        }
    }
    if (Destroyed)
        return;
    CThingPlayerCreature* phero = GetWorld().PeekMyPlayer()->GetPPlayerCharacter();
    if (IS_THING_ALIVE(phero))
        AddCreatureTriggerer(*phero);
    if (ReadyToInitialise && !Initialised)
        InitialiseCreatureGenerator(false);
    if (DrawCreatureGeneratorDebug && GetParentThing().GetTCGraphicAppearance() == 0) {
        GetParentThing().AddTC("CTCGraphicAppearance", false, 0);
        GetParentThing().AddTC("CTCGraphicAppearanceStaticMesh", false, 0);
        CDefPointer<const CAppearanceDef> pappearance_def(NULL_DEF);
        GetParentThing().PeekBaseDef().GetSubDef(&pappearance_def);
        GetParentThing().GetTCGraphicAppearance()->SetMainGraphic(pappearance_def->Graphic);
        GetParentThing().SetAsDrawable(true);
    }
    long creatureCount = CreatedCreatures.size();
    CreatedCreatures.remove(CIntelligentPointer<CThing>(0));
    CreatureTriggerers.erase(std::remove(CreatureTriggerers.begin(), CreatureTriggerers.end(), CIntelligentPointer<CThingCreatureBase>(0)),
                             CreatureTriggerers.end());
    RegionCreationCount += CreatedCreatures.size() - creatureCount;
    if (IsUndeadGenerator())
        UpdateUndead();
    if (Initialised)
        UpdateCreatureTrigger(GetWorld().GetFrame());
    if (PDef.Get()) {
        if (!PDef->RandomEffects.empty() && --FramesUntilRandomEffect <= 0) {
            if (IS_THING_ALIVE(PRandomParticle))
                PRandomParticle->Kill(true);
            long random_index = GFRandom(PDef->RandomEffects.size(), GetPWorldSeed());
            PRandomParticle = CTCDParticleEmitter::Create(PDef->RandomEffects[random_index], GetParentThing().GetPos(), false);
            if (PRandomParticle != 0) {
                CTCDParticleEmitter* ptcdParticleEmitter;
                PRandomParticle->GetTC(&ptcdParticleEmitter);
                ptcdParticleEmitter->AttachToThing(GetParentThing(), 0x20, "", 0, 0.0f);
            }
            FramesUntilRandomEffect = PDef->SecondsBetweenEffects * CMainGameComponent::GetConstantFPS();
        }
        if (PDef->RandomSoundCriteria != "" && --FramesUntilRandomSoundEffect <= 0) {
            CTCSoundPlayer* ptc_sound_player;
            if (GetParentThing().GetTC(&ptc_sound_player))
                ptc_sound_player->PlayAttachedCriteria(PDef->RandomSoundCriteria, true, false, false, -1);
            FramesUntilRandomSoundEffect = PDef->SecondsBetweenSoundEffects * CMainGameComponent::GetConstantFPS();
        }
    }
}