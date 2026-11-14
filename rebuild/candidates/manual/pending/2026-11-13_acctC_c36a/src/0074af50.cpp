// CTCCreatureGenerator::CreateCreaturesUndead(CThingCreatureBase* triggerer) -- pick a random creature family and make each of its creatures (when its generation group is enabled) as an undead near the triggerer, idling below ground, until the creation limit is hit
// Leaf: ?CreateCreaturesUndead@CTCCreatureGenerator@@IAEXPAVCThingCreatureBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0074AF50, 856 bytes, `ret 4` (last instruction 0x0074B2A5).
// Name: Ego_r.pdb 0x0075DD00 + FableWin.pdb ?CreateCreaturesUndead@CTCCreatureGenerator@@IAEXPAVCThingCreatureBase@@@Z (protected); caller UpdateUndead 0x0074ACE0.
// Callees: HasHitCreationLimit 0x0074A190, CTCBase::PeekWorld 0x00686A80, CWorld::GetWorldSeed 0x0049D810, GFROR13 0x00497890 (inline GFRandom),
//   puts 0x00BFEAF2 / abort 0x00BFEAEC (the expanded STLport vector::at), CTCBase::GetDefinitionManager 0x006869D0, GetDef<CCreatureGenerationFamilyDef>
//   0x0074D220, CThing::PeekDefinitionManager 0x004C7990, GetDef<CThingCreatureDef> 0x00513160, GetSubDef<CCreatureDef> 0x00485815,
//   CWorld::IsCreatureGenerationEnabledForGroup 0x0049D910, GetUndeadGenerationPoint 0x0074A260, CThing::GetPos 0x004C73D0,
//   CTCShapeManager::IsPosInAnyShape 0x00760870, CCharString 0x0099EBF0 / ~ 0x0099EAE0, CThingAICreatureInit 0x004FACD0 / ~ 0x004FAD50,
//   CThingAICreature::Create 0x00833800, CThing::AddTC 0x004C9D60, CTCGeneratedCreature::SetGenerator 0x0074D0D0, IS_THING_ALIVE<CThingCreatureBase*>
//   0x005C05C0, CCreatureAction_UndeadIdleUnderground 0x0074DD20, CThingCreatureBase::SetCurrentAction 0x006644F0, ~CCombatActionBase 0x0062E900,
//   CIntelligentPointer<CThing>(CThing*) 0x004A96F0 / ~CBaseIntelligentPointer 0x00A01C10, list push_back 0x004BA4E0. Global g_GeneratedCreatureCount 0x013BAD54.
// Shape: every failure inside the loop is a `break` (the family handle is released ONCE, at 0x0074B28D, and never held in a register);
//   push_back takes the creature through the implicit CIntelligentPointer<CThing> conversion (`lea eax,[esp+30h]` re-taken after the ctor,
//   0x0074B1BB); `size() <= 0` (`jbe`, 0x0074AF65); the definition of HasHitCreationLimit precedes this function in the TU (no `mov ecx,esi`
//   before PeekWorld, 0x0074AF8A): FABLE_RETAIL_CTCCREATUREGENERATOR_HASHITCREATIONLIMIT_DEFINITION.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_ISCREATUREGENERATIONENABLEDFORGROUP
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CCREATUREDEF_DIALOGUEVOICES
#define FABLE_RETAIL_CCREATUREDEF_SCREENINVENTORIES
#define FABLE_RETAIL_CCREATUREDEF_RESPAWNWAIT
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHINGAICREATUREINIT_DTOR
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_STLP_VECTOR_AT_INLINE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_CTCSHAPEMANAGER
#define FABLE_RETAIL_CTCCREATUREGENERATOR
#define FABLE_RETAIL_CTCCREATUREGENERATOR_UNDEAD
#define FABLE_RETAIL_CTCCREATUREGENERATOR_HASHITCREATIONLIMIT_DEFINITION
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingAICreatureInit.h"
#include "engine/ThingAlive.h"
#include "engine/CArray.h"
#include "engine/CDefIndex.h"
#include "engine/CWorld.h"
#include "engine/GFRandom.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CCreatureGenerationFamilyDef.h"
#include "engine/CThingCreatureDef.h"
#include "engine/CCreatureDef.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_UndeadIdleUnderground.h"
#include "engine/CTCShapeManager.h"
#include "engine/CTCGeneratedCreature.h"
#include "engine/CTCCreatureGenerator.h"

void CTCCreatureGenerator::CreateCreaturesUndead(CThingCreatureBase* triggerer)
{
    if (CreatureFamilies.size() <= 0 || HasHitCreationLimit())
        return;
    UndeadTriggered = true;
    long index = GFRandom(CreatureFamilies.size(), PeekWorld().GetWorldSeed());
    CDefPointer<const CCreatureGenerationFamilyDef> family;
    GetDefinitionManager().GetDef(CreatureFamilies.at(index), &family);
    for (std::vector<long>::const_iterator it = family->Creatures.begin(); it != family->Creatures.end(); ++it) {
        CDefPointer<const CThingCreatureDef> thing_def;
        if (GetParentThing().PeekDefinitionManager().GetDef(*it, &thing_def)) {
            CDefPointer<const CCreatureDef> creature_def;
            if (thing_def->GetSubDef(&creature_def) && PeekWorld().IsCreatureGenerationEnabledForGroup(creature_def->CreatureGroup)) {
                C3DVector pos;
                if (triggerer) {
                    if (!GetUndeadGenerationPoint(*triggerer, pos))
                        break;
                } else {
                    pos = GetParentThing().GetPos();
                    if (PExclusionArea && PExclusionArea->IsPosInAnyShape(pos))
                        break;
                }
                CThingAICreatureInit init(0, 0, false, GenParams.ScriptNameOfAllGeneratedCreatures);
                CThingAICreature* creature = CThingAICreature::Create(*it, pos, GetParentThing().GetPlayerNumber(), init);
                if (!creature || creature->IsDead())
                    break;
                CTCGeneratedCreature* generated = static_cast<CTCGeneratedCreature*>(creature->AddTC("CTCGeneratedCreature", false, 0));
                generated->SetGenerator(GetParentThing());
                if (IS_THING_ALIVE(triggerer))
                    creature->SetCurrentAction(CCreatureAction_UndeadIdleUnderground(*creature));
                CreatedCreatures.push_back(creature);
                ++g_GeneratedCreatureCount;
                ++CreationCount;
                if (HasHitCreationLimit())
                    break;
            }
        }
    }
}
