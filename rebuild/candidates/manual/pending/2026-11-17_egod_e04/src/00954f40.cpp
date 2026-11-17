// CAIStateGroup_MazeTeleport::Update() -- teleport the boss out: to the "MazeTeleportPos<phase>" marker found within 200 units (remembered as the boss's phase start position), else to a random valid hiding position within 20 units (terminating first when there is none), then terminate
// Leaf: ?Update@CAIStateGroup_MazeTeleport@@UAEXXZ
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) 0x00954F40, 604 bytes, thiscall `ret`, /GS cookie (__security_cookie 0x0139C8A8, check 0x00BFE9F9).
// Names: Ego_r.pdb / Ego_d.pdb ?Update@CAIStateGroup_MazeTeleport@@UAEXXZ; Ego_d ai_state_group_maze (MazeTeleport Initialise /
//   IsRunnable / OnActivate / Update): function-scope pMarkerThing, ptc_boss, boss_phase, marker_name[32], teleport_pos and the else
//   block's validPositions -- named and ordered as Ego_d (first declared = highest debug frame word; the char array moved by /GS).
// Callees: bitset test inline + CVectorMap::LowerBound 0x0040F020 (GetTC<CTCBoss>, TCI 173), CTCBoss::GetBossPhase 0x007AED10, sprintf
//   0x00BFEB10, CCharString 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0 (the implicit char* -> CCharString temporary copied into
//   CThingFilter_HasScriptName), CThing::GetPos 0x004C73D0, _ftol 0x00BFEA70 (C3DVector::ToC2DCoordI), CAIStateGroupBase::
//   PeekThingSearchTools 0x008FD000, CThingSearchTools::GetPFirstThingInRoughArea<CThingFilter_HasScriptName,CThing> 0x007FD110
//   (Ego_r.pdb instance name; the landed binding), CAIStateGroupBase::GetWorld 0x008FCFC0, CWorld virtual GetWorldMap (slot 12),
//   NCreatureHiding::GetValidPositions 0x0074A5D0, CWorld::GetWorldSeed 0x0049D810, GFROR13 0x00497890 (inline GFRandom), free
//   0x00BFEA14, CCreatureAction_TeleportOut(CThingCreatureBase&, C3DVector&, bool) 0x008BCF20 / ~CCombatActionBase 0x0062E900,
//   CAIStateGroupBase::AIAction_PerformActionAndWait 0x0097DB70, Terminate 0x008FCF40. String "MazeTeleportPos%d" 0x012949F0.
// Shape: ported off the parked local model onto the shared headers (the old "frame slot order" residue): GetTC<CTCBoss>'s key and
//   ptc_boss share retail's frame word; `pMarkerThing = 0;` assigned right before the search (declared first as Ego_d, the zero store
//   lands at 0x00954FC4); the filter built from `marker_name` by the implicit conversion (retail re-addresses the temporary for the
//   copy); FableWin ?SetPhaseStartPos@CTCBoss@@ for the +0x22 copy with the owner-approved FABLE_RETAIL_C3DVECTOR_PACKED (alignment 1,
//   CTCBoss PhaseStartPos +0x22 as the landed JackOfBladesStage1 0x00972990). Class: engine/CAIStateGroup_MazeTeleport.h retail opt-in
//   FABLE_RETAIL_CAISTATEGROUP_MAZETELEPORT over the existing engine/_quarantine/CAIStateGroup_MazeBase.h (opt-in FABLE_RETAIL_CAISTATEGROUP_MAZEBASE, as landed 0x00956060).
#include <stdio.h>
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTION_TELEPORT
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTROUGH
#define FABLE_RETAIL_CTCBOSS_SETPHASESTARTPOS
#define FABLE_RETAIL_CAISTATEGROUP_MAZETELEPORT
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/C2DCoordI.h"
#include "engine/C2DBoxI.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CTCBoss.h"
#include "engine/CWorld.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_HasScriptName.h"
#include "engine/NCreatureHiding.h"
#include "engine/GFRandom.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_TeleportOut.h"
#include "engine/CAIStateGroup_MazeTeleport.h"

void CAIStateGroup_MazeTeleport::Update()
{
    CThing* pMarkerThing;
    CTCBoss* ptc_boss;
    GetCreature().GetTC(&ptc_boss);
    long boss_phase = ptc_boss->GetBossPhase();
    char marker_name[32];
    sprintf(marker_name, "MazeTeleportPos%d", boss_phase);
    pMarkerThing = 0;
    PeekThingSearchTools().GetPFirstThingInRoughArea(C2DBoxI(GetCreature().GetPos().ToC2DCoordI(), 200),
        CThingFilter_HasScriptName(marker_name), &pMarkerThing);
    C3DVector teleport_pos;
    if (pMarkerThing) {
        teleport_pos = pMarkerThing->GetPos();
        ptc_boss->SetPhaseStartPos(teleport_pos);
    } else {
        std::vector<C3DVector> validPositions;
        NCreatureHiding::GetValidPositions(validPositions, GetWorld().GetWorldMap(), GetWorld().GetThingSearchTools(), GetCreature().GetPos(), 20.0f, false, 0);
        if (validPositions.size() == 0)
            Terminate();
        else
            teleport_pos = validPositions[GFRandom(validPositions.size(), GetWorld().GetWorldSeed())];
    }
    AIAction_PerformActionAndWait(CCreatureAction_TeleportOut(GetCreature(), teleport_pos, false), false);
    Terminate();
}
