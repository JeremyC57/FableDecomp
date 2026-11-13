// CAIStateGroup_MazeTutorialBattleTeleport::Update() -- the Maze tutorial battle's teleport tick: gather the valid positions within 15
//   of the creature, drop those nearer the target than twice the maze def's EnflameRange and those within 7 of the creature, then pick
//   random candidates until one is navigable from the creature (discarding unreachable ones; falls back to LastTeleportPos when they run
//   out). With no candidates at all it terminates and teleports in place. Then CCreatureAction_TeleportOut + PerformActionAndWait,
//   stamp FrameOfLastTeleport / LastTeleportPos and Terminate(). The vector and the CMazeBattleDef sub-def pointer die at the end.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00956060, length 786 bytes (vtable 0x0128C2D8 slot 12), /Gr AI TU, no EH frame.
// Callees: CThing::GetPos @ 0x004C73D0, CDefClassBase::GetSubDef<CMazeBattleDef> @ 0x008C3510 (visible body kept out of line),
//   CAIStateGroupBase::GetWorld @ 0x008FCFC0, CWorld::GetWorldMap (vslot 12), NCreatureHiding::GetValidPositions @ 0x0074A5D0,
//   CBaseIntelligentPointer::GetPItem @ 0x00A01B50, std::remove_if<C3DVector*, unary_negate<CVectorFilter_FurtherFromPointThan> >
//   @ 0x008AC060, STLport __copy<C3DVector*> @ 0x0074CA20 (visible body kept out of line), CAIStateGroupBase::Terminate @ 0x008FCF40,
//   CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (expanded GetTC), CWorld::GetWorldSeed @ 0x0049D810,
//   GFROR13 @ 0x00497890 (inline GFRandom), CTCCreatureNavigation::IsAbleToNavigateFromPosToPos @ 0x006CB620,
//   CCreatureAction_TeleportOut ctor @ 0x008BCF20, ~CCombatActionBase @ 0x0062E900, CAIStateGroupBase::AIAction_PerformActionAndWait
//   @ 0x0097DB70, CWorld::GetFrame @ 0x0049D870, free @ 0x00BFEA14, the def's slot-1 Delete.
// Shape: each filter is `v.erase(remove_if(...), v.end())`; the nav TC is fetched once before the retry loop; GFRandom keeps its
//   zero-range test. Needs header overlay opt-ins FABLE_RETAIL_CAISTATEGROUP_MAZETUTORIALBATTLETELEPORT (+ _MAZEBASE),
//   FABLE_RETAIL_CMAZEBATTLEDEF and FABLE_RETAIL_STLP_COPY_NOINLINE (retail called __copy out of line from all three erase bodies).
// Leaf: ?Update@CAIStateGroup_MazeTutorialBattleTeleport@@UAEXXZ
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_INDEX
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_MAZETUTORIALBATTLETELEPORT
#define FABLE_RETAIL_CMAZEBATTLEDEF
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_CCREATUREACTION_TELEPORT
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_STLP_REMOVE_COPY_IF
#define FABLE_RETAIL_STLP_NOT1
#define FABLE_RETAIL_STLP_COPY_NOINLINE
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGABLE
#include "rebuild_abi.h"
#include "engine/C3DVector.h"
#include "engine/CDefPointer.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingBaseDef.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/CWorld.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_TeleportOut.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_MazeTutorialBattleTeleport.h"
#include "engine/_quarantine/CMazeBattleDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CVectorFilter_FurtherFromPointThan.h"
#include "engine/NCreatureHiding.h"
#include "engine/GFRandom.h"
#include "stlport/position_algorithms.h"
void CAIStateGroup_MazeTutorialBattleTeleport::Update()
{
    C3DVector teleportPos = GetCreature().GetPos();
    CDefPointer<const CMazeBattleDef> def;
    GetCreature().PeekBaseDef().GetSubDef(&def);
    std::vector<C3DVector> validPositions;
    NCreatureHiding::GetValidPositions(validPositions, GetWorld().GetWorldMap(), GetWorld().PeekThingSearchTools(), GetCreature().GetPos(), 15.0f, false, 0);
    validPositions.erase(std::remove_if(validPositions.begin(), validPositions.end(), std::not1(CVectorFilter_FurtherFromPointThan(PTargetThing->GetPos(), def->EnflameRange * 2.0f))), validPositions.end());
    validPositions.erase(std::remove_if(validPositions.begin(), validPositions.end(), std::not1(CVectorFilter_FurtherFromPointThan(GetCreature().GetPos(), 7.0f))), validPositions.end());
    if (validPositions.size() == 0) {
        Terminate();
    } else {
        CTCCreatureNavigation* nav;
        GetCreature().GetTC(TCI_NAVIGATION, &nav);
        for (;;) {
            unsigned long index = GFRandom(validPositions.size(), GetWorld().GetWorldSeed());
            if (nav->IsAbleToNavigateFromPosToPos(GetCreature().GetPos(), validPositions[index], 0.0f)) {
                teleportPos = validPositions[index];
                break;
            }
            validPositions.erase(validPositions.begin() + index);
            if (validPositions.size() == 0) {
                teleportPos = LastTeleportPos;
                break;
            }
        }
    }
    AIAction_PerformActionAndWait(CCreatureAction_TeleportOut(GetCreature(), teleportPos, false), false);
    FrameOfLastTeleport = GetWorld().GetFrame();
    LastTeleportPos = teleportPos;
    Terminate();
}