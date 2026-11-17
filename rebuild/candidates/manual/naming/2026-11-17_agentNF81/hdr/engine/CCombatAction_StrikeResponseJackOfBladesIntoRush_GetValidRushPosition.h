#ifndef FABLE_CCOMBATACTION_STRIKERESPONSEJACKOFBLADESINTORUSH_GETVALIDRUSHPOSITION_H
#define FABLE_CCOMBATACTION_STRIKERESPONSEJACKOFBLADESINTORUSH_GETVALIDRUSHPOSITION_H
// NEW header (NF81, round 7, q10988): the one text of Ego_r.pdb 0x008FA430 / FableWin.pdb 0x02A0EBE0
// ?GetValidRushPosition@CCombatAction_StrikeResponseJackOfBladesIntoRush@@IAE_NAAVC3DVector@@@Z (retail 0x008C1600), moved here unchanged
// from the landed row 0x008C1600 (NF78, RM; its Shape (FITTED) locals world / creature / search / last / me / distance / too_close /
// self are not Ego_d names). Retail compiled it in one object with its caller FinishAction 0x008C2550, which calls it out of line but
// sees the body (VC7.1 then knows rush_to_pos's address is not kept and reuses its dead words: frame 0x16C, 0x178 with a declaration
// only). So, as engine/CMap.h's FABLE_RETAIL_CMAP_GETSTRONGESTENGINETHEMEAT_BODY: a TU that includes this header gets the body
// `inline __declspec(noinline)` (a COMDAT, never expanded); the row 0x008C1600 defines
// FABLE_RETAIL_CCOMBATACTION_STRIKERESPONSEJACKOFBLADESINTORUSH_GETVALIDRUSHPOSITION_DEFINE and so emits the ordinary out-of-line
// definition. Include it after the headers it names below, with FABLE_RETAIL_CCOMBATACTION_STRIKERESPONSEJACKOFBLADESINTORUSH_FINISHACTION
// (the protected declaration).
#include "fable_compiler.h"
#include "engine/C3DVector.h"
#include "engine/CJackOfBladesBattleDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CTCBoss.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingCreatureBase.h"
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

#ifndef FABLE_RETAIL_CCOMBATACTION_STRIKERESPONSEJACKOFBLADESINTORUSH_GETVALIDRUSHPOSITION_DEFINE
inline __declspec(noinline)
#endif
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
#endif
