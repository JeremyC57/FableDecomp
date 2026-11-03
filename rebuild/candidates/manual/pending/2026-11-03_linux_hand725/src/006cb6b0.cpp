// CTCCreatureNavigation::InterruptNavigation -- stop the creature's current route: forwards to the creature's current navigator (PNavigator +0x0C), CNavigatorBase vtable slot 18 Interrupt
// Leaf: ?InterruptNavigation@CTCCreatureNavigation@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006CB6B0, 8 bytes: `mov ecx,[ecx+0Ch] ; mov eax,[ecx] ; jmp dword ptr [eax+48h]` (a sibling call:
//   the arguments stay in place).
// Identity: FableWin.pdb ?InterruptNavigation@CTCCreatureNavigation@@QAEXXZ (Ego_r has the same member). Retail
//   compiles the CTCCreatureNavigation members that only delegate to the navigator to this one shape; the slot fixes the member:
//   CNavigatorBase slot 18 = Interrupt (0x00A19AD0; engine/CNavigatorBase.h, named from the CNavigatorAStar overrides). FableWin orders the
//   TU InterruptNavigation, .., GetNearestNavigablePosTo, .., IsNewDestinationGoingToResetNavigation, GetCurrentDestination,
//   CalculateRouteFromTo, InitialiseFromPreCalculatedRoute, .., CalculateNavigableWidthOfLine -- retail 0x006CB6B0, 0x006CB8E0,
//   0x006CB990, 0x006CB9A0, 0x006CB9B0, 0x006CB9C0, 0x006CBA10. Callers: CThingCreatureBase::AutoPlaceNear 0x00665E5A / AutoPlaceNearBuilding, CTCScriptedControl::OnDie 0x00713777, CAIStateGroupBase::CleanupMain 0x008FD095 (ecx = the GetTC result). Landed spellings: ?InterruptNavigation@CTCCreatureNavigation@@QAEXXZ (right); CTCBase/CTCBaseAny::OnStateGroupRemoved, CTCType7/CTCWalk::Reset (wrong, NAME_FIXES).
//   The catalog label "CScriptThing::operator==" is propagated across the byte-twin shape and is wrong.
// Header: engine/CTCCreatureNavigation.h FABLE_RETAIL_CTCCREATURENAVIGATION (overlay hdr/), engine/CNavigatorBase.h FABLE_RETAIL_CNAVIGATORBASE.
#define FABLE_RETAIL_CNAVIGATORBASE
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#include "engine/CNavigatorBase.h"
#include "engine/CTCCreatureNavigation.h"

void CTCCreatureNavigation::InterruptNavigation()
{
    PNavigator->Interrupt();
}
