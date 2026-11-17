// CCombatAction_StrikeResponseJackOfBladesIntoRush::GetValidRushPosition(C3DVector&) -- gather the valid positions within the battle def's rush-away distance (+2) of Jack, drop those closer than the rush-away distance to Jack and those within 21 of the boss phase start position, then try random candidates until one Jack can navigate to (copied to rush_pos); returns whether one was found
// Leaf: ?GetValidRushPosition@CCombatAction_StrikeResponseJackOfBladesIntoRush@@IAE_NAAVC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008C1600, 761 bytes (+ int3 padding), `ret 4`.
// Name (NF78, q10945): Ego_r.pdb 0x008FA430 / FableWin.pdb 0x02A0EBE0 ?GetValidRushPosition@CCombatAction_StrikeResponseJackOfBladesIntoRush@@IAE_NAAVC3DVector@@@Z
//   (protected, C3DVector&); the landed caller FinishAction 0x008C2550 binds it so through the shared header.
// Port (NF78, round 7; q10945 / q10946): every class from the shared headers (the 0x008C2550 block: CCombatAction_StrikeResponseJackOfBladesIntoRush.h
//   FINISHACTION declares the protected GetValidRushPosition(C3DVector&)); the local CThing / CWorld / CTCBoss / CCombatActionBase /
//   CIntelligentPointer / CVectorMap / CDefPointer / vector models and the stand-in bodies are gone. p_jack_def, ptc_boss, validPositions,
//   ptc_nav, found, random_index and rush_pos are the Ego_d.pdb names (NF74's 0x008C2550 text).
// Shape (FITTED, carried over from the landed row's Shape levers; not Ego_d locals): the named world / creature / search locals give
//   retail's GetWorld, GetPItem, GetWorld call order before GetValidPositions; last (end() captured after the size test), me, distance and
//   the named too_close predicate give the first erase; found declared before the navigation GetTC; the named self pointer is read before
//   the candidate element (grades 1-4: 744, 760, 761 DIFFER, then RM).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PCreature +8), CDefClassBase::GetSubDef<CJackOfBladesBattleDef> 0x008C33E0 (same-TU
//   noinline header body), CVectorMap::LowerBound 0x0040F020 (GetTC expanded), CCreatureActionBase::GetWorld 0x00692940, CWorld virtual
//   slot 12 (world map), NCreatureHiding::GetValidPositions 0x0074A5D0, CThing::GetPos 0x004C73D0, STLport remove_if 0x008AC060
//   (unary_negate<CVectorFilter_FurtherFromPointThan>, both PDBs) and 0x008C34A0 (its landed row's descriptive CIsOutsideRadius: the
//   phase filter's class is unproven, NF74 REJECTED.tsv), STLport __copy<C3DVector> 0x0074CA20, CWorld::GetWorldSeed 0x0049D810,
//   GFROR13 0x00497890, CTCCreatureNavigation::IsAbleToNavigateFromPosToPos 0x006CB620, free 0x00BFEA14, def release via slot 1.
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_REMOVE_COPY_IF
#define FABLE_RETAIL_STLP_NOT1
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGABLE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTCBOSS_GETPHASESTARTPOS
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED   // CTCBoss::PhaseStartPos at +0x22 (alignment 1, engine/CTCBoss.h; as 0x009547D0)
#include "fable_compiler.h"
#include <math.h>
#include "engine/C3DVector.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#include "engine/CJackOfBladesBattleDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCPhysicsBase.h"
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCBOSS_GETGENERICCOUNTER
#include "engine/CTCBoss.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CIntelligentPointer.h"
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#include "engine/CThingCreatureBase.h"
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_PCREATURE
#define FABLE_RETAIL_CCREATUREACTIONBASE_ACCESSORS
#define FABLE_RETAIL_CCOMBATACTIONBASE
#define FABLE_RETAIL_CCOMBATACTIONBASE_RECOVER
#define FABLE_RETAIL_CCOMBATACTION_GENERICSTRIKERESPONSEBASE_TYPED
#define FABLE_RETAIL_CCOMBATACTION_GENERICSTRIKERESPONSEBASE_FINISHACTION
#define FABLE_RETAIL_CCOMBATACTION_STRIKERESPONSEJACKOFBLADESINTORUSH
#define FABLE_RETAIL_CCOMBATACTION_STRIKERESPONSEJACKOFBLADESINTORUSH_FINISHACTION
#include "engine/CCombatAction_StrikeResponseJackOfBladesIntoRush.h"
#include "stlport/fable_containers.h"
#include "engine/CVectorFilter_FurtherFromPointThan.h"
#include "engine/NCreatureHiding.h"
#include "engine/CWorld.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/GFRandom.h"
// NF81 (round 7, q10988): the body (and the CIsOutsideRadius predicate and the remove_if declaration) moved unchanged into the NF81
//   overlay header engine/CCombatAction_StrikeResponseJackOfBladesIntoRush_GetValidRushPosition.h, the one text this row and its caller
//   FinishAction 0x008C2550 (a visible noinline COMDAT there) share; this row defines it out of line through the _DEFINE macro.
#define FABLE_RETAIL_CCOMBATACTION_STRIKERESPONSEJACKOFBLADESINTORUSH_GETVALIDRUSHPOSITION_DEFINE
#include "engine/CCombatAction_StrikeResponseJackOfBladesIntoRush_GetValidRushPosition.h"
