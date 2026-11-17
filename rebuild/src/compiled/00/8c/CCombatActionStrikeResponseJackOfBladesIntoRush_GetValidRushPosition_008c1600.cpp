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
namespace std {
// STLport 4.6 _algo.h remove_if over C3DVector*, out of line in retail (0x008AC060 unary_negate<CVectorFilter_FurtherFromPointThan>,
// 0x008C34A0 the descriptive CIsOutsideRadius of its landed row). Declaration only.
template<class _ForwardIter, class _Predicate> _ForwardIter __fastcall remove_if(_ForwardIter first, _ForwardIter last, _Predicate pred);
}
// The boss phase-start filter of remove_if 0x008C34A0 / remove_copy_if 0x008C2ED0: { float Radius; C3DVector Centre }, 3D length > Radius.
// Descriptive name (the landed 0x008C34A0 / 0x008C2ED0 rows' spelling): which PDB class it is stays unproven (NF74 REJECTED.tsv).
struct CIsOutsideRadius {
    float     Radius;                           // +0x00
    C3DVector Centre;                           // +0x04
    CIsOutsideRadius(float radius, const C3DVector& centre) : Radius(radius), Centre(centre) { }
    CIsOutsideRadius(const CIsOutsideRadius& o) : Radius(o.Radius), Centre(o.Centre) { }
};

bool CCombatAction_StrikeResponseJackOfBladesIntoRush::GetValidRushPosition(C3DVector& rush_pos)
{
    CDefPointer<const CJackOfBladesBattleDef> p_jack_def;
    reinterpret_cast<CThing*>(PCreature.GetPItem())->PeekBaseDef().GetSubDef<CJackOfBladesBattleDef>(&p_jack_def);
    CTCBoss* ptc_boss;
    reinterpret_cast<CThing*>(PCreature.GetPItem())->GetTC(TCI_BOSS, &ptc_boss);
    std::vector<C3DVector> validPositions;
    CWorld& world = GetWorld();
    CThing* creature = reinterpret_cast<CThing*>(PCreature.GetPItem());
    CThingSearchTools& search = GetWorld().GetThingSearchTools();
    NCreatureHiding::GetValidPositions(validPositions, world.GetWorldMap(), search, creature->GetPos(),
        p_jack_def->RushAwayDistance + 2.0f, false, 0);
    if (validPositions.size() == 0)
        return false;
    std::vector<C3DVector>::iterator last = validPositions.end();
    CThing* me = reinterpret_cast<CThing*>(PCreature.GetPItem());
    float distance = p_jack_def->RushAwayDistance;
    std::unary_negate<CVectorFilter_FurtherFromPointThan> too_close(CVectorFilter_FurtherFromPointThan(me->GetPos(), distance));
    validPositions.erase(std::remove_if(validPositions.begin(), validPositions.end(), too_close), last);
    validPositions.erase(std::remove_if(validPositions.begin(), validPositions.end(),
        CIsOutsideRadius(21.0f, ptc_boss->GetPhaseStartPos())), validPositions.end());
    bool found = false;
    CTCCreatureNavigation* ptc_nav;
    reinterpret_cast<CThing*>(PCreature.GetPItem())->GetTC(TCI_NAVIGATION, &ptc_nav);
    while (!found && validPositions.size() != 0) {
        long random_index = GFRandom(validPositions.size(), GetWorld().GetWorldSeed());
        CThing* self = reinterpret_cast<CThing*>(PCreature.GetPItem());
        if (ptc_nav->IsAbleToNavigateFromPosToPos(self->GetPos(), validPositions[random_index], 0.0f)) {
            rush_pos = validPositions[random_index];
            found = true;
        } else
            validPositions.erase(validPositions.begin() + random_index);
    }
    return found;
}
