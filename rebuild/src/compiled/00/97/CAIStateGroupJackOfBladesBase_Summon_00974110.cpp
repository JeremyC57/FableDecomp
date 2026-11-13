// CAIStateGroup_JackOfBladesBase::Summon(long, bool) -- Jack of Blades summons the creatures of a generation family def one by one: make sure he has a CTCSolidToThing (add one when missing); for each family creature (while the arena centre and the target are alive) pick a random valid position within 8 units of the arena centre that is more than 5 units away from it (remove_if drops the closer ones), create the creature there (CThingAICreature::Create, init defaults), make it non-solid to Jack, start its OnGenerate action, record it as a boss helper and point its targeting at Jack's target, then force-push the target away when asked and it is within 5 units (2D), else play SUMMON_LOOP; stop after the last creature or when the action returns 1; terminate when the arena centre or the target is gone
// Leaf: ?Summon@CAIStateGroup_JackOfBladesBase@@IAEXJ_N@Z
// Retail: Fable.exe (TLC, VC7.1 RTM) @ 0x00974110, 1631 bytes, `ret 8`, frame 0x3CC. Ego_r.pdb ?Summon@CAIStateGroup_JackOfBladesBase@@IAEXJ_N@Z
//   (protected; donor 0x009992F0 right after SummonPillars 0x00998F40).
// Callees (all checked against retail with rel.py): CVectorMap::LowerBound 0x0040F020 (the expanded GetTC), CThing::AddTC 0x004C9D60,
//   CGameDefinitionManager::Get 0x0044C6B0, CDefinitionManager::GetDef<CCreatureGenerationFamilyDef> 0x0074D220 (FableWin.pdb template
//   spelling), GetPItem 0x00A01B50, GetWorld 0x008FCFC0, CWorld::GetWorldMap (virtual +0x30), CThing::GetPos 0x004C73D0,
//   NCreatureHiding::GetValidPositions 0x0074A5D0, __find_if 0x008AAC50 / remove_copy_if 0x008AB710 (the expanded remove_if over
//   not1(CVectorFilter_FurtherFromPointThan)), CWorld::GetWorldSeed 0x0049D810 / GFROR13 0x00497890 (the inline GFRandom),
//   CCharString 0x0099EBF0 / 0x0099EC30 / 0x0099EAE0, CThingAICreature::Create 0x00833800, CTCSolidToThing::AddThingNotSolidTo 0x00770FA0,
//   CCombatActionBase::GetSoundPair 0x008561A0 / ctor 0x00858030 / dtor 0x0062E900, CThingCreatureBase::SetCurrentAction 0x006644F0,
//   CTCBoss::SetHelperCreature 0x007AF310, CBaseIntelligentPointer ctor / SetPItem / dtor 0x00A01B10 / 0x00A01B90 / 0x00A01C10,
//   CCreatureAction_PlayCombatAnimation 0x008B7FF0, AIAction_PerformAction 0x0097D8B0, Terminate 0x008FCF40, free 0x00BFEA14.
// Literals: "CTCSolidToThing" 0x0123B47C, "" 0x0122D70E, "SWORD_SWING" 0x0127AC80, "SUMMON_LOOP" 0x012981FC, 5.0f 0x01297600, 8.0f pushed.
// Shape: (1) the body sits inside `if (GetDef(...)) { ... }`, so the failed lookup shares the function's natural exit (the def release
//   at 0x0097474E); an early `return` puts its own release copy right after the call. (2) The GetDef<T> body is visible
//   (FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY, kept out of line): the family pointer then stays a plain frame word, released
//   with `dec [reg+4]`, and the colouring (family in edi, ebx = 0 after the erase loop) matches; declaration-only exposes it.
//   (3) The 2D distance is one expression, `GFFastSqrt(X*X + Y*Y)` on the named offset: VC7.1 then squares Y first and keeps both
//   components on the x87 stack until after fsqrt (0x009745E1); C3DVector::GetMagnitude2D's `squared = X*X; squared += Y*Y` squares X
//   first and pops it (3 instructions differ). (4) OnGenerate / WillForcePushInto constructors are expanded (inline in their headers,
//   retail's out-of-line COMDATs are 0x0074DBB0 / 0x008C38B0); remove_if is expanded, find_if's __find_if called.
// Layout: overlay opt-ins (hdr/README.md): CAIStateGroup_JackOfBladesBase (+ _SUMMON), CAIStateGroupBase PERFORMACTION_NOWAIT, CTCBoss
//   SETHELPERCREATURE, CTCTargetingAI SETSCRIPTTARGETOVERRIDE (inline), CCreatureAction_OnGenerate retail class, new
//   CCombatAction_WillForcePushInto.h / CCreatureGenerationFamilyDef.h, stlport/position_algorithms.h REMOVE_IF_INLINE.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION_NOWAIT
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_JACKOFBLADESBASE
#define FABLE_RETAIL_CAISTATEGROUP_JACKOFBLADESBASE_SUMMON
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_CCREATUREACTION_ONGENERATE
#define FABLE_RETAIL_CCREATUREACTION_PLAYCOMBATANIMATION
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_UPDATE_NEAREST_INTERESTING
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCSOLIDTOTHING
#define FABLE_RETAIL_CTCBOSS_SETHELPERCREATURE
#define FABLE_RETAIL_CTCTARGETINGAI_SETSCRIPTTARGETOVERRIDE
#define FABLE_RETAIL_CTHINGAICREATUREINIT_CTOR_BODY
#define FABLE_RETAIL_STLP_REMOVE_COPY_IF
#define FABLE_RETAIL_STLP_NOT1
#define FABLE_RETAIL_STLP_REMOVE_IF_INLINE
#define FABLE_RETAIL_CRESOURCELIST
#include "engine/CAIStateGroupBase.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "stlport/position_algorithms.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingAICreatureInit.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_JackOfBladesBase.h"
#include "engine/CDefPointer.h"
#include "engine/CDefinitionManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CCreatureGenerationFamilyDef.h"
#include "engine/CSoundPair.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_OnGenerate.h"
#include "engine/CCombatAction_WillForcePushInto.h"
#include "engine/CCreatureAction_PlayCombatAnimation.h"
#include "engine/CTCSolidToThing.h"
#include "engine/CTCBoss.h"
#include "engine/CTCTargetingAI.h"
#include "engine/CWorld.h"
#include "engine/CVectorFilter_FurtherFromPointThan.h"
#include "engine/NCreatureHiding.h"
#include "engine/GFRandom.h"
#include "engine/ThingAlive.h"

void CAIStateGroup_JackOfBladesBase::Summon(long family_def_index, bool force_push_close_enemy)
{
    CTCSolidToThing* solid_to_thing;
    if (!GetCreature().GetTC(&solid_to_thing))
        solid_to_thing = (CTCSolidToThing*)GetCreature().AddTC("CTCSolidToThing", false, 0);
    CDefPointer<const CCreatureGenerationFamilyDef> family;
    if (CGameDefinitionManager::Get().GetDef(family_def_index, &family)) {
        long creature_count = family->Creatures.size();
        long creature_index = 0;
        EAIActionReturn result;
        do {
            if (!IS_THING_ALIVE(PArenaCentre) || !IS_THING_ALIVE(PTargetThing)) {
                Terminate();
                return;
            }
            std::vector<C3DVector> positions;
            NCreatureHiding::GetValidPositions(positions, GetWorld().GetWorldMap(), GetWorld().PeekThingSearchTools(), PArenaCentre->GetPos(),
                                               8.0f, false, 0);
            if (positions.empty())
                return;
            positions.erase(std::remove_if(positions.begin(), positions.end(),
                                           std::not1(CVectorFilter_FurtherFromPointThan(PArenaCentre->GetPos(), 5.0f))),
                            positions.end());
            if (positions.empty())
                return;
            C3DVector pos = positions[GFRandom(positions.size(), GetWorld().GetWorldSeed())];
            long creature_def = family->Creatures[creature_index++];
            CThingAICreature* creature = CThingAICreature::Create(creature_def, pos, 4, CThingAICreatureInit());
            if (IS_THING_ALIVE(creature)) {
                if (solid_to_thing)
                    solid_to_thing->AddThingNotSolidTo(creature);
                creature->SetCurrentAction(CCreatureAction_OnGenerate(*creature));
                CTCBoss* boss;
                GetCreature().GetTC(&boss);
                boss->SetHelperCreature(*creature);
                CTCTargetingAI* targeting;
                creature->GetTC(&targeting);
                targeting->SetScriptTargetOverride(PTargetThing.Get());
            }
            C3DVector to_enemy = PTargetThing->GetPos() - GetCreature().GetPos();
            if (force_push_close_enemy && GFFastSqrt(to_enemy.X * to_enemy.X + to_enemy.Y * to_enemy.Y) < 5.0f)
                result = AIAction_PerformAction(CCombatAction_WillForcePushInto(GetCreature()));
            else
                result = AIAction_PerformAction(CCreatureAction_PlayCombatAnimation(GetCreature(), "SUMMON_LOOP", true));
        } while (creature_index < creature_count && result != 1);
    }
}