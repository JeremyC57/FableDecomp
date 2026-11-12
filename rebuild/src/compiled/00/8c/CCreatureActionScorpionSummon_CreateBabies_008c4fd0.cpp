// CCreatureAction_ScorpionSummon::CreateBabies() -- the king scorpion's summon: find the valid positions within 3.0 of the king and,
//   when there are any, create the KING_SCORPION_SUMMON_FAMILY creatures the boss is still short of (the family's creature list less
//   its living helpers), each at a random valid position, named SUMMONED_CREATURE for player 4; a live new creature rises up out of
//   the ground and becomes one of the boss's helpers
// Leaf: ?CreateBabies@CCreatureAction_ScorpionSummon@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008C4FD0, 722 bytes, thiscall, plain `ret`.
// Identity: Ego_r.pdb ?CreateBabies@CCreatureAction_ScorpionSummon@@AAEXXZ (Ego_r 0x008FCCE0); called by the landed FinishAction
//   0x008C52B0.
// Callees: CCharString(const char*, long) 0x0099EBF0 / copy 0x0099EC30 / operator=(const char*) 0x0099EFE0 / ~ 0x0099EAE0,
//   CGameDefinitionManager::Get 0x0044C6B0 (static), CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410,
//   CCreatureActionBase::GetWorld 0x00692940, CWorld::GetWorldMap (slot 12, +0x30), CWorld::GetThingSearchTools (inline, +0x20),
//   CBaseIntelligentPointer::GetPItem 0x00A01B50 (PCreature->), CThing::GetPos 0x004C73D0, NCreatureHiding::GetValidPositions
//   0x0074A5D0 (__fastcall), CDefinitionManager::GetDef<CCreatureGenerationFamilyDef> 0x0074D220, CVectorMap LowerBound 0x0040F020
//   (the unchecked GetTC<CTCBoss>), CTCBoss::CountLivingHelpers 0x007AEEC0 / SetHelperCreature 0x007AF310, CWorld::GetWorldSeed
//   0x0049D810, GFROR13 0x00497890 (the inline GFRandom), CThingAICreature::Create 0x00833800 (__fastcall), the CCreatureAction_RiseUp
//   ctor 0x008BD880 and its implicit dtor (~CCombatActionBase 0x0062E900), CThingCreatureBase::SetCurrentAction 0x006644F0, free
//   0x00BFEA14 (the position vector).
// Shape: the inline CThingAICreatureInit default ctor (its two "" CCharString temporaries copied in, then destroyed); the creature
//   loop is a do-while (retail enters the body without a first test, 0x008C511B); the family def is reached through the out-of-line
//   GetDef<T> template, its creature list is the Ego_r.pdb Creatures vector (retail +0x3C).
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATUREINIT_CTOR_BODY
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTIONBASE_ACCESSORS
#define FABLE_RETAIL_CCREATUREACTION_SCORPIONSUMMON_CLASS
#define FABLE_RETAIL_CCREATUREACTION_SCORPIONSUMMON_CREATEBABIES
#define FABLE_RETAIL_CTCBOSS_SETHELPER
#define FABLE_RETAIL_CTCBOSS_LIVINGHELPERS
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingAICreatureInit.h"
#include "engine/CDefPointer.h"
#include "engine/CParentDefClassBase.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CWorld.h"
#include "engine/CTCBoss.h"
#include "engine/GFRandom.h"
#include "engine/NCreatureHiding.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_RiseUp.h"
#include "engine/CCreatureAction_ScorpionSummon.h"

// No shared header: Ego_r.pdb CCreatureGenerationFamilyDef (Creatures vector<long> at Ego_r +0x40, retail +0x3C after the 0x39-byte
// CParentDefClassBase). Only the member this row reads.
class CCreatureGenerationFamilyDef : public CParentDefClassBase {
public:
    std::vector<long> Creatures;                                         // +0x3c
};

// Same-TU instance (as the landed CreateMinion 0x0079ACC0 does for GetDef<CBrainDef>): the GetDef<T> body (landed 0x0074D220, the COMDAT
// copy the linker kept) visible and kept out of line, so VC7.1 sees that the family CDefPointer does not escape (retail's in-place
// `dec dword ptr [ecx+4]` release with no null store, and the name temporary's frame slot shared with it).
template<> __declspec(noinline) bool CDefinitionManager::GetDef<CCreatureGenerationFamilyDef>(long index, CDefPointer<const CCreatureGenerationFamilyDef>* out) const
{
    if (index > 0) {
        CDefPointer<const CDefClassBase> def = GetPDefFromGlobalIndex(index);
        if (def.Get()) {
            out->SetDynamicCast(def);
            return true;
        }
    }
    return false;
}

void CCreatureAction_ScorpionSummon::CreateBabies()
{
    long family_index = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("KING_SCORPION_SUMMON_FAMILY");
    std::vector<C3DVector> positions;
    CWorld& world = GetWorld();
    CThingSearchTools& search_tools = GetWorld().GetThingSearchTools();
    NCreatureHiding::GetValidPositions(positions, world.GetWorldMap(), search_tools, PCreature->GetPos(), 3.0f, false, 0);
    if (positions.size() == 0)
        return;
    CDefPointer<const CCreatureGenerationFamilyDef> family;
    if (CGameDefinitionManager::Get().GetDef(family_index, &family)) {
        CTCBoss* boss;
        PCreature->GetTC(&boss);
        long count = family->Creatures.size() - boss->CountLivingHelpers();
        long i = 0;
        do {
            C3DVector pos = positions[GFRandom(positions.size(), GetWorld().GetWorldSeed())];
            long def_index = family->Creatures[i++];
            CThingAICreatureInit init;
            init.ScriptName = "SUMMONED_CREATURE";
            CThingAICreature* creature = CThingAICreature::Create(def_index, pos, 4, init);
            if (IS_THING_ALIVE(creature)) {
                creature->SetCurrentAction(CCreatureAction_RiseUp(*creature));
                boss->SetHelperCreature(*creature);
            }
        } while (i < count);
    }
}