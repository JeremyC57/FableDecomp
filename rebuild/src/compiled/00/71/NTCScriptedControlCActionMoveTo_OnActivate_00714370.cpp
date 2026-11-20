// NTCScriptedControl::CActionMoveTo::OnActivate() -- start the move: let the creature move, swap in the navigator physics (keeping the old component), claim a free action point of the target or widen the proximity by both radii, stop hero pushing, and start a pre-calculated route
// Inline-budget: header opt-in FABLE_RETAIL_CTCPARAMSPHYSICSBASE_INLINE_CTOR (add-only, CTCParamsPhysicsBase.h): retail expands the
//   CTCParamsPhysicsBase(position, rhs) ctor in place (0x00714428: CBaseClassNonCopyable() 0x0099A2F0, the copies, vtable 0x0125C818),
//   where the header default declares it only (the other callers call 0x006A06E0). Without it: DIFFER (README ablation). No flag.
// Leaf: ?OnActivate@CActionMoveTo@NTCScriptedControl@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00714370, 948 bytes, plain `ret`, frame 0x50; vtable slot 2 of CActionMoveTo.
// Identity: Ego_r.pdb ?OnActivate@CActionMoveTo@NTCScriptedControl@@UAEXXZ. Locals and blocks = Ego_d.pdb (debug length 2234): ptc_physics;
//   {old_position, tc_params, old_rhs}; {ptc_carryable {ptc_single_user_action_points {rhset}}}; {ptarget_thing}; {ptc_navigation
//   {ptc_pre_calculated_route}}. Ego_d's action-point arm has two failure blocks (lines 713..718, 723..728), as written here.
// Callees (catalog names): NCreatureMode::IsAbleToMove 0x0085F3E0 / RemoveMovementBlockingModes 0x0085F600 (__fastcall), CTCBase::Clone
//   0x00686FA0, CThing::AddTC 0x004C9D60, CCharString 0x0099EBF0 / != 0x0099E960 / ~ 0x0099EAE0, CBaseClassNonCopyable 0x0099A2F0 /
//   ~ 0x0099A300, CVectorMap::LowerBound 0x0040F020 / const 0x004365B0 (the expanded GetTC / PeekTC), CTCScriptedControl::
//   CleanupAllActions 0x00713630, CTCUserActionPointBase::GetFreeActionPoint 0x007FEDE0, CTCPreCalculatedNavigationRoute::HasRoute
//   0x00800500 (bool: NAME_FIXES) / GetRouteDestination 0x008004D0, CThing::GetPos 0x004C73D0, CTCCreatureNavigation::
//   InitialiseFromPreCalculatedRoute 0x006CB9C0; virtual CTCBase slot 22 GetClassTypeName, CTCPhysicsBase slots 72 GetRHSet / 83
//   IsPushableByHero / 84 SetAsPushableByHero, CScriptThing slots 11 GetPThing / 75 IsAlive.
// Shape: the carryable and route look-ups are the const PeekTC (retail calls the const LowerBound 0x004365B0 for 0x47 / 0xA6, the
//   non-const one for 0x2D / 7); the route component is used whether or not the look-up found it (retail 0x007146C8 reads the key slot).
//   Proximity = std::max (the STLport reference-returning max: retail selects the address, 0x007145F9..0x00714603); a plain conditional
//   expression gives DIFFER(936v948).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCSCRIPTEDCONTROL_ACTIONBASE
#define FABLE_RETAIL_CACTIONMOVETO_ONACTIVATE
#define FABLE_RETAIL_CTCPARAMSPHYSICSBASE_INLINE_CTOR
#define FABLE_RETAIL_CTCUSERACTIONPOINTBASE
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CTCBASE_REGISTRY
#define FABLE_RETAIL_CTCBASE_REGISTRY_LOOKUP
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGABLE
#define FABLE_RETAIL_CTCSCRIPTEDCONTROL_CLEANUPALLACTIONS
#define FABLE_RETAIL_CTCCARRYABLE_ISCARRIED
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "stlport/fable_containers.h"
#include "engine/CRightHandedSet.h"
#include "engine/CCountedPointer.h"
#include "engine/CThing.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCParamsPhysicsBase.h"
#include "engine/CTCCarryable.h"
#include "engine/CTCUserActionPointBase.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/NTCScriptedControl_CActionMoveTo.h"

namespace NCreatureMode {
bool FABLE_FASTCALL IsAbleToMove(const CThing& thing);                 // 0x0085F3E0
void FABLE_FASTCALL RemoveMovementBlockingModes(CThing& thing);        // 0x0085F600
}
// No engine header: Ego_r.pdb CTCPreCalculatedNavigationRoute (Route +0x20 retail), interface TCI 0xA6.
class CTCPreCalculatedNavigationRoute : public CTCBase {
public:
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return (ETCInterfaceType)0xa6; }
    bool HasRoute() const;                                               // 0x00800500
    C3DVector GetRouteDestination() const;                               // 0x008004D0
    unsigned char _pad[0x20 - sizeof(CTCBase)];
    std::vector<std::pair<C2DVector, unsigned char> > Route;            // +0x20
};

namespace NTCScriptedControl {
void CActionMoveTo::OnActivate()
{
    CTCScriptedControl::CActionBase::OnActivate();
    if (!NCreatureMode::IsAbleToMove(GetParentThing()))
        NCreatureMode::RemoveMovementBlockingModes(GetParentThing());
    PTCOldPhysics = NULL;
    CTCPhysicsBase* ptc_physics = GetParentThing().GetPhysicsTC();
    if (ptc_physics->GetClassTypeName() != "CTCPhysicsNavigator") {
        PTCOldPhysics = ptc_physics->Clone();
        C3DVector old_position = ptc_physics->GetPosition();
        CRightHandedSet old_rhs = ptc_physics->GetRHSet();
        CTCParamsPhysicsBase tc_params(old_position, old_rhs);
        GetParentThing().AddTC("CTCPhysicsNavigator", true, &tc_params);
    }
    if (UseTargetThing && PTargetThing.IsAlive()) {
        if (UseActionPoint) {
            const CTCCarryable* ptc_carryable;
            if (GetParentThing().PeekTC(CTCCarryable::StatGetInterfaceType(), &ptc_carryable) && ptc_carryable->IsCarried()) {
                Terminated = true;
                if (CancelAllActionsOnFailure)
                    PTCScript->CleanupAllActions();
                return;
            }
            CTCSingleUserActionPoints* ptc_single_user_action_points;
            if (PTargetThing.GetPThing()->GetTC(&ptc_single_user_action_points)) {
                CRightHandedSet rhset;
                if (ptc_single_user_action_points->GetFreeActionPoint(GetParentThing(), Pos, rhset, ActionPointIndex)) {
                    GotActionPointIndex = true;
                    UseTargetThing = false;
                } else {
                    Terminated = true;
                    if (CancelAllActionsOnFailure)
                        PTCScript->CleanupAllActions();
                    return;
                }
            } else {
                Terminated = true;
                if (CancelAllActionsOnFailure)
                    PTCScript->CleanupAllActions();
                return;
            }
        } else {
            CThing* ptarget_thing = PTargetThing.GetPThing();
            if ((ptarget_thing->GetTypeFlags() & 8) || ptarget_thing->GetType() == 5 || ptarget_thing->GetType() == 3) {
                float radii = GetParentThing().GetPhysicsTC()->GetRadius() + ptarget_thing->GetPhysicsTC()->GetRadius();
                Proximity = std::max(Proximity, radii);
            }
        }
    }
    if (!AvoidDynamicObstacles) {
        PreviousPushableByHero = GetParentThing().GetPhysicsTC()->IsPushableByHero();
        GetParentThing().GetPhysicsTC()->SetAsPushableByHero(false);
    }
    if (UseRouteThing && PRouteThing.IsAlive()) {
        CTCCreatureNavigation* ptc_navigation;
        if (GetParentThing().GetTC(&ptc_navigation)) {
            const CTCPreCalculatedNavigationRoute* ptc_pre_calculated_route;
            PRouteThing.GetPThing()->PeekTC(CTCPreCalculatedNavigationRoute::StatGetInterfaceType(), &ptc_pre_calculated_route);
            if (ptc_pre_calculated_route->HasRoute()) {
                Pos = ptc_pre_calculated_route->GetRouteDestination();
                ptc_navigation->InitialiseFromPreCalculatedRoute(GetParentThing().GetPos(), ptc_pre_calculated_route->Route, Pos);
            } else
                Terminated = true;
        }
    }
}
}