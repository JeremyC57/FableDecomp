// CTCCreatureNavigation::CalculateNavigableWidthOfLine -- measure the navigable width either side of a line: forwards to the creature's current navigator (PNavigator +0x0C), CNavigatorBase vtable slot 19 CalculateNavigableWidthOfLine
// Leaf: ?CalculateNavigableWidthOfLine@CTCCreatureNavigation@@QAEXABVC3DVector@@0AAM1@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006CBA10, 8 bytes: `mov ecx,[ecx+0Ch] ; mov eax,[ecx] ; jmp dword ptr [eax+4Ch]` (a sibling call:
//   the arguments stay in place).
// Identity: FableWin.pdb ?CalculateNavigableWidthOfLine@CTCCreatureNavigation@@QAEXABVC3DVector@@0AAM1@Z (Ego_r has the same member). Retail
//   compiles the CTCCreatureNavigation members that only delegate to the navigator to this one shape; the slot fixes the member:
//   CNavigatorBase slot 19 = CalculateNavigableWidthOfLine (0x00A19AE0; engine/CNavigatorBase.h, named from the CNavigatorAStar overrides). FableWin orders the
//   TU InterruptNavigation, .., GetNearestNavigablePosTo, .., IsNewDestinationGoingToResetNavigation, GetCurrentDestination,
//   CalculateRouteFromTo, InitialiseFromPreCalculatedRoute, .., CalculateNavigableWidthOfLine -- retail 0x006CB6B0, 0x006CB8E0,
//   0x006CB990, 0x006CB9A0, 0x006CB9B0, 0x006CB9C0, 0x006CBA10. Callers: CAIStateGroup_MinionReactToPlayerInteraction::Update 0x009306E4.
//   The catalog label "CScriptThing::operator==" is propagated across the byte-twin shape and is wrong.
// Header: engine/CTCCreatureNavigation.h FABLE_RETAIL_CTCCREATURENAVIGATION (overlay hdr/), engine/CNavigatorBase.h FABLE_RETAIL_CNAVIGATORBASE.
#define FABLE_RETAIL_CNAVIGATORBASE
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#include "engine/CNavigatorBase.h"
#include "engine/CTCCreatureNavigation.h"

void CTCCreatureNavigation::CalculateNavigableWidthOfLine(const C3DVector& from, const C3DVector& to, float& left, float& right)
{
    PNavigator->CalculateNavigableWidthOfLine(from, to, left, right);
}
