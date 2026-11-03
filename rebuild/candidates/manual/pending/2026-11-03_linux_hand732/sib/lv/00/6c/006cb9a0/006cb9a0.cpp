// CTCCreatureNavigation::GetCurrentDestination -- the route's current destination: forwards to the creature's current navigator (PNavigator +0x0C), CNavigatorBase vtable slot 12 GetCurrentDestination
// Leaf: ?GetCurrentDestination@CTCCreatureNavigation@@QBEABVC3DVector@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006CB9A0, 8 bytes: `mov ecx,[ecx+0Ch] ; mov eax,[ecx] ; jmp dword ptr [eax+30h]` (a sibling call:
//   the arguments stay in place).
// Identity: FableWin.pdb ?GetCurrentDestination@CTCCreatureNavigation@@QBEABVC3DVector@@XZ (Ego_r has the same member). Retail
//   compiles the CTCCreatureNavigation members that only delegate to the navigator to this one shape; the slot fixes the member:
//   CNavigatorBase slot 12 = GetCurrentDestination (pure; engine/CNavigatorBase.h, named from the CNavigatorAStar overrides). FableWin orders the
//   TU InterruptNavigation, .., GetNearestNavigablePosTo, .., IsNewDestinationGoingToResetNavigation, GetCurrentDestination,
//   CalculateRouteFromTo, InitialiseFromPreCalculatedRoute, .., CalculateNavigableWidthOfLine -- retail 0x006CB6B0, 0x006CB8E0,
//   0x006CB990, 0x006CB9A0, 0x006CB9B0, 0x006CB9C0, 0x006CBA10. Callers: CAIStateGroupBase::AIAction_MoveToThingProximity 0x0097E524 (no arguments, right after IsNewDestinationGoingToResetNavigation).
//   The catalog label "CScriptThing::operator==" is propagated across the byte-twin shape and is wrong.
// Header: engine/CTCCreatureNavigation.h FABLE_RETAIL_CTCCREATURENAVIGATION (overlay hdr/), engine/CNavigatorBase.h FABLE_RETAIL_CNAVIGATORBASE.
#define FABLE_RETAIL_CNAVIGATORBASE
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#include "engine/CNavigatorBase.h"
#include "engine/CTCCreatureNavigation.h"

const C3DVector& CTCCreatureNavigation::GetCurrentDestination() const
{
    return PNavigator->GetCurrentDestination();
}