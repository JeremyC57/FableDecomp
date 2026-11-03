// CTCVillage::CalculateSecuritySweepTasks() -- lay out the guards' security sweep: count the villagers with the guard property,
// take the first guard's navigation TC, record where the player stands, find the nearest region exit or teleporter, shrink the
// sweep radius by 0.4 per elapsed second (and to the nearest guard's 2D distance, which also lowers InitialSecuritySweepRadius),
// clamp it to 2 (forced when SecuritySweepUrgent) and to 0.8 of the exit distance, then for each guard step the exit direction
// round the player by 1/guards turns, nudging the angle by a fifth of a step until a navigable point is found, and add a
// CVillageTaskSecuritySweep(point, 5, player position, 0.25) for it.
// Grade: DIFFER(1074v1074) -- 8 diff lines, all stack-slot choice: the uninitialised HasGuardProperty predicate is pushed from
//   count's slot [ebp-0x18] (retail [ebp-0x1c], its own slot, so retail pushes it before storing the count), and the
//   CVectorMap key temporary of the inlined GetTC<CTCCreatureNavigation> lands in navigation's slot [ebp-0xc] (retail [ebp-0x14],
//   the dead iterator's slot). Instructions, x87 order, frame (0x50), every other slot and all relocations match.
// Leaf: ?CalculateSecuritySweepTasks@CTCVillage@@AAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00659800, 1074 bytes, plain `ret`. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees (retail): std::count_if<..,HasGuardProperty> 0x0065F6A6, std::find_if<..,HasGuardProperty> 0x0066000A,
//   CBaseIntelligentPointer::GetPItem 0x00A01B50, std::bitset<274>::test 0x00410DE0 (inline CThing::HasTC),
//   CVectorMap<..>::LowerBound 0x0040F020 (inline Find), CTCBase::GetPlayerManager 0x00686D40, CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetPControlledCreature 0x00487DC0,
//   CThing::GetPos 0x004C73D0, CWorld::PeekMyPlayer 0x0049D9D0, CTCBase::GetWorld 0x00686A70, CTCBase::PeekWorld 0x00686A80,
//   CWorld::GetFrame 0x0049D870 (static), CThingSearchTools::GetPBestThing<binary_compose<..>,CThingCompare_Nearest,CThing> 0x0065F6DA /
//   <CThingFilter_HasTC,CThingCompare_Nearest,CThingAICreature> 0x0065F744, CTCCreatureNavigation::GetNearestNavigablePosTo
//   0x006CB8E0, C3DVector::RotateXY 0x00A14040, operator new 0x00BFEA1A, CVillageTaskSecuritySweep ctor 0x0065DCE8,
//   CCountedPointer<CVillageTaskSecuritySweep> ctor 0x0065E2E0 / dtor 0x0065D5CC, list<..>::push_back 0x00660587.
// Levers found (attempt log in LOG.tsv): (1) VC7.1 packs address-taken locals by SCOPE, so the predicate+iterator and the
//   nearest-guard search sit in their own blocks and the exit pointer's block ends before Normalise (step / the inverse-sqrt bit
//   temporary then reuse its slot) -- frame 0x64 -> 0x50; (2) one source call site `while (!GetNearestNavigablePosTo(rotated + Pos,
//   ..))`: VC duplicates the loop test (both copies share one candidate temporary, the first copy-propagates offset for rotated);
//   (3) `angle >= 0` / `angle >= step` give retail's `test ah,1; jne`; (4) a named `distanceSquared` local gives retail's x87
//   spills of the offset (Y to a temp, X to its home); (5) the 2D guard distance as `(Pos - guardPos).GetMagnitude2D()` (Y read
//   through ebx); (6) the player position bound to a const reference so exit = 0 lands between GetPos and the copy.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CBASICSTRING_EQ_OUTOFLINE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING_GETSTRING
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_VILLAGE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_PROPERTY
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGBUILDING
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCTHINGOWNER
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_MEMBERS
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_GAMETIME
#define FABLE_RETAIL_CTCBASE_GETWORLD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BEST
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_C3DVECTOR_NORMALISE
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CVILLAGETASKPATROL_CTOR
#define FABLE_RETAIL_CVILLAGETASKPICKCROPS_CTOR
#define FABLE_RETAIL_CVILLAGETASKSECURITYSWEEP_CTOR
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CTCBase.h"
#include "engine/CThing.h"
#include "engine/CDefClassBase.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CDefString.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingBuilding.h"
#include "engine/CThingMarker.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCThingOwner.h"
#include "engine/CTCVillageMember.h"
#include "engine/CTCVillage.h"
#include "engine/CWorld.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_HasTC.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/CVillageTaskSecuritySweep.h"
#include "fable_game.h"
#include <math.h>
// ---- TU-local models (no engine header) ----
class CThingCompare_Nearest {                            // Ego_r.pdb CThingCompare_Nearest { C3DVector Pos; } (12 bytes)
public:
    CThingCompare_Nearest(const C3DVector& pos) : Pos(pos) {}   // ??0CThingCompare_Nearest@@QAE@ABVC3DVector@@@Z (inline)
    C3DVector Pos;
};
namespace std {                                          // STLport 4.6 <stl/_function.h>
template <class _Tp> struct logical_or : public binary_function<_Tp, _Tp, bool> {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x || __y; }
};
template <class _Operation1, class _Operation2, class _Operation3>
class binary_compose : public unary_function<typename _Operation2::argument_type, typename _Operation1::result_type> {
protected:
    _Operation1 _M_op1;
    _Operation2 _M_op2;
    _Operation3 _M_op3;
public:
    binary_compose(const _Operation1& __x, const _Operation2& __y, const _Operation3& __z) : _M_op1(__x), _M_op2(__y), _M_op3(__z) { }
};
template <class _Operation1, class _Operation2, class _Operation3>
inline binary_compose<_Operation1, _Operation2, _Operation3> __fastcall
compose2(const _Operation1& __op1, const _Operation2& __op2, const _Operation3& __op3)
{
    return binary_compose<_Operation1, _Operation2, _Operation3>(__op1, __op2, __op3);
}
}
struct HasGuardProperty {                                // FableWin.pdb UHasGuardProperty@@ (a struct); operator() 0x0065F152
    bool operator()(CIntelligentPointer<CThingAICreature> thing) const;   // ??RHasGuardProperty@@QBE_NV?$CIntelligentPointer@VCThingAICreature@@@@@Z
};
namespace std {
// The out-of-line algorithm COMDATs the function calls (STLport 4.6 _algo.h; __fastcall as FableWin.pdb's YI spellings):
// count_if 0x0065F6A6 and find_if 0x0066000A over list<CIntelligentPointer<CThingAICreature> >::iterator.
template <class _InputIter, class _Predicate> long __fastcall count_if(_InputIter __first, _InputIter __last, _Predicate __pred);
template <class _InputIter, class _Predicate> _InputIter __fastcall find_if(_InputIter __first, _InputIter __last, _Predicate __pred);
}
void CTCVillage::CalculateSecuritySweepTasks()
{
    long numberOfGuards;
    CThingAICreature* firstGuard;
    {
        HasGuardProperty isGuard;
        numberOfGuards = std::count_if(VillagePeople.begin(), VillagePeople.end(), isGuard);
        std::list<CIntelligentPointer<CThingAICreature> >::iterator guard =
            std::find_if(VillagePeople.begin(), VillagePeople.end(), isGuard);
        if (guard == VillagePeople.end())
            return;
        firstGuard = guard->Get();
    }
    CTCCreatureNavigation* navigation = 0;
    firstGuard->GetTC(&navigation);
    CThingPlayerCreature* player = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    if (!player)
        return;
    float radius;
    const C3DVector* exitPosition;
    {
    const C3DVector& playerPosition = player->GetPos();
    CThing* exit = 0;
    PositionPlayerWasInWhenSecuritySweepCalculated = playerPosition;
    CMainGameComponent::Get()->PeekWorld().PeekMyPlayer()->GetPControlledCreature();
    GetWorld().GetThingSearchTools().GetPBestThing(
        std::compose2(std::logical_or<bool>(), CThingFilter_HasTC(TCI_REGION_EXIT), CThingFilter_HasTC(TCI_TELEPORTER)),
        CThingCompare_Nearest(PositionPlayerWasInWhenSecuritySweepCalculated), &exit);
    if (!exit)
        return;
    radius = InitialSecuritySweepRadius -
        (float)((PeekWorld().GetFrame() - FrameStartedSecuritySweep) / CMainGameComponent::GetConstantFPS()) * 0.4f;
    {
    CThingAICreature* nearestGuard;
    if (GetWorld().GetThingSearchTools().GetPBestThing(CThingFilter_HasTC(TCI_GUARD),
            CThingCompare_Nearest(PositionPlayerWasInWhenSecuritySweepCalculated), &nearestGuard)) {
        float distance = (PositionPlayerWasInWhenSecuritySweepCalculated - nearestGuard->GetPos()).GetMagnitude2D();
        if (radius > distance) {
            InitialSecuritySweepRadius -= radius - distance;
            radius = distance;
        }
    }
    }
    const float minimumRadius = 2.0f;
    if (SecuritySweepUrgent)
        radius = minimumRadius;
    if (radius < minimumRadius)
        radius = minimumRadius;
    exitPosition = &exit->GetPos();
    }
    C3DVector offset = *exitPosition - PositionPlayerWasInWhenSecuritySweepCalculated;
    float distanceSquared = offset.GetSquaredMagnitude2D();
    if (distanceSquared < radius * radius)
        radius = (float)sqrt(distanceSquared) * 0.8f;
    offset.Normalise();
    offset *= radius;
    float step = 1.0f / numberOfGuards;
    for (long i = 0; i < numberOfGuards; ++i) {
        float angle = 0.0f;
        C3DVector rotated = offset;
        C3DVector position;
        while (!navigation->GetNearestNavigablePosTo(rotated + PositionPlayerWasInWhenSecuritySweepCalculated, 5.0f, position)) {
            rotated = offset;
            if (angle >= 0.0f) {
                angle += step * 0.2f;
                if (angle >= step)
                    goto nextGuard;
            }
            else
                angle *= -1.0f;
            rotated.RotateXY(angle);
        }
        SecuritySweepTasks.push_back(CCountedPointer<CVillageTaskSecuritySweep>(
            new CVillageTaskSecuritySweep(position, 5.0f, PositionPlayerWasInWhenSecuritySweepCalculated, 0.25f)));
    nextGuard:
        offset.RotateXY(step);
    }
}
