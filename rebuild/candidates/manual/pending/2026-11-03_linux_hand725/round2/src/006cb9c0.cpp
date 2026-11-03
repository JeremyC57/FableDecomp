// CTCCreatureNavigation::InitialiseFromPreCalculatedRoute -- start following a precalculated route: forwards to the creature's current navigator (PNavigator +0x0C), CNavigatorBase vtable slot 15 InitialiseFromPreCalculatedRoute
// Leaf: ?InitialiseFromPreCalculatedRoute@CTCCreatureNavigation@@QAEXABVC3DVector@@ABV?$vector@U?$pair@VC2DVector@@E@std@@V?$allocator@U?$pair@VC2DVector@@E@std@@@2@@std@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006CB9C0, 8 bytes: `mov ecx,[ecx+0Ch] ; mov eax,[ecx] ; jmp dword ptr [eax+3Ch]` (a sibling call:
//   the arguments stay in place).
// Identity: FableWin.pdb ?InitialiseFromPreCalculatedRoute@CTCCreatureNavigation@@QAEXABVC3DVector@@ABV?$vector@U?$pair@VC2DVector@@E@std@@V?$allocator@U?$pair@VC2DVector@@E@std@@@2@@std@@0@Z (Ego_r has the same member). Retail
//   compiles the CTCCreatureNavigation members that only delegate to the navigator to this one shape; the slot fixes the member:
//   CNavigatorBase slot 15 = InitialiseFromPreCalculatedRoute (pure; engine/CNavigatorBase.h, named from the CNavigatorAStar overrides). FableWin orders the
//   TU InterruptNavigation, .., GetNearestNavigablePosTo, .., IsNewDestinationGoingToResetNavigation, GetCurrentDestination,
//   CalculateRouteFromTo, InitialiseFromPreCalculatedRoute, .., CalculateNavigableWidthOfLine -- retail 0x006CB6B0, 0x006CB8E0,
//   0x006CB990, 0x006CB9A0, 0x006CB9B0, 0x006CB9C0, 0x006CBA10. Callers: CActionMoveTo::OnActivate 0x0071470B and CAIStateGroupBase::SetUpPreCalcNav 0x0097F658 (position, route, CThing::GetPosition). Landed spelling CTCNavigation::SetUpRoute is wrong (NAME_FIXES).
//   The catalog label "CScriptThing::operator==" is propagated across the byte-twin shape and is wrong.
// Header: engine/CTCCreatureNavigation.h FABLE_RETAIL_CTCCREATURENAVIGATION (overlay hdr/), engine/CNavigatorBase.h FABLE_RETAIL_CNAVIGATORBASE.
#define FABLE_RETAIL_CNAVIGATORBASE
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#include "engine/CNavigatorBase.h"
#include "engine/CTCCreatureNavigation.h"

void CTCCreatureNavigation::InitialiseFromPreCalculatedRoute(const C3DVector& start, const std::vector<std::pair<C2DVector, unsigned char>, std::allocator<std::pair<C2DVector, unsigned char> > >& route, const C3DVector& end)
{
    PNavigator->InitialiseFromPreCalculatedRoute(start, route, end);
}
