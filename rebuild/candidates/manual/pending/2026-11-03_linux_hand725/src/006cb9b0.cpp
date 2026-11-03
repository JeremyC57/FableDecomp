// CTCCreatureNavigation::CalculateRouteFromTo -- compute a route between two positions into a waypoint list: forwards to the creature's current navigator (PNavigator +0x0C), CNavigatorBase vtable slot 14 CalculateRouteFromTo
// Leaf: ?CalculateRouteFromTo@CTCCreatureNavigation@@QBEXABVC3DVector@@0AAV?$vector@U?$pair@VC2DVector@@E@std@@V?$allocator@U?$pair@VC2DVector@@E@std@@@2@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006CB9B0, 8 bytes: `mov ecx,[ecx+0Ch] ; mov eax,[ecx] ; jmp dword ptr [eax+38h]` (a sibling call:
//   the arguments stay in place).
// Identity: FableWin.pdb ?CalculateRouteFromTo@CTCCreatureNavigation@@QBEXABVC3DVector@@0AAV?$vector@U?$pair@VC2DVector@@E@std@@V?$allocator@U?$pair@VC2DVector@@E@std@@@2@@std@@@Z (Ego_r has the same member). Retail
//   compiles the CTCCreatureNavigation members that only delegate to the navigator to this one shape; the slot fixes the member:
//   CNavigatorBase slot 14 = CalculateRouteFromTo (pure; engine/CNavigatorBase.h, named from the CNavigatorAStar overrides). FableWin orders the
//   TU InterruptNavigation, .., GetNearestNavigablePosTo, .., IsNewDestinationGoingToResetNavigation, GetCurrentDestination,
//   CalculateRouteFromTo, InitialiseFromPreCalculatedRoute, .., CalculateNavigableWidthOfLine -- retail 0x006CB6B0, 0x006CB8E0,
//   0x006CB990, 0x006CB9A0, 0x006CB9B0, 0x006CB9C0, 0x006CBA10. Callers: CTCPreCalculatedNavigationRoute::CalculateRouteFromTo 0x008006B5 (three pushes). Landed spelling CTCNavigationBase::CalculateRouteFromTo(.., CRoute&) is wrong (NAME_FIXES).
//   The catalog label "CScriptThing::operator==" is propagated across the byte-twin shape and is wrong.
// Header: engine/CTCCreatureNavigation.h FABLE_RETAIL_CTCCREATURENAVIGATION (overlay hdr/), engine/CNavigatorBase.h FABLE_RETAIL_CNAVIGATORBASE.
#define FABLE_RETAIL_CNAVIGATORBASE
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#include "engine/CNavigatorBase.h"
#include "engine/CTCCreatureNavigation.h"

void CTCCreatureNavigation::CalculateRouteFromTo(const C3DVector& from, const C3DVector& to, std::vector<std::pair<C2DVector, unsigned char>, std::allocator<std::pair<C2DVector, unsigned char> > >& route) const
{
    PNavigator->CalculateRouteFromTo(from, to, route);
}
