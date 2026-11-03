// CTCCreatureNavigation::GetNearestNavigablePosTo -- find the nearest navigable position to a point: forwards to the creature's current navigator (PNavigator +0x0C), CNavigatorBase vtable slot 8 GetNearestNavigablePosTo
// Leaf: ?GetNearestNavigablePosTo@CTCCreatureNavigation@@QBE_NABVC3DVector@@MAAV2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006CB8E0, 8 bytes: `mov ecx,[ecx+0Ch] ; mov eax,[ecx] ; jmp dword ptr [eax+20h]` (a sibling call:
//   the arguments stay in place).
// Identity: FableWin.pdb ?GetNearestNavigablePosTo@CTCCreatureNavigation@@QBE_NABVC3DVector@@MAAV2@@Z (Ego_r has the same member). Retail
//   compiles the CTCCreatureNavigation members that only delegate to the navigator to this one shape; the slot fixes the member:
//   CNavigatorBase slot 8 = GetNearestNavigablePosTo (pure; engine/CNavigatorBase.h, named from the CNavigatorAStar overrides). FableWin orders the
//   TU InterruptNavigation, .., GetNearestNavigablePosTo, .., IsNewDestinationGoingToResetNavigation, GetCurrentDestination,
//   CalculateRouteFromTo, InitialiseFromPreCalculatedRoute, .., CalculateNavigableWidthOfLine -- retail 0x006CB6B0, 0x006CB8E0,
//   0x006CB990, 0x006CB9A0, 0x006CB9B0, 0x006CB9C0, 0x006CBA10. Callers: CTCVillage::CalculateSecuritySweepTasks 0x00659800 (chunk 720 NAME_FIXES). Landed spellings: the right one, and CTCWalk::FindNearestWalkablePos (wrong).
//   The catalog label "CScriptThing::operator==" is propagated across the byte-twin shape and is wrong.
// Header: engine/CTCCreatureNavigation.h FABLE_RETAIL_CTCCREATURENAVIGATION (overlay hdr/), engine/CNavigatorBase.h FABLE_RETAIL_CNAVIGATORBASE.
#define FABLE_RETAIL_CNAVIGATORBASE
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#include "engine/CNavigatorBase.h"
#include "engine/CTCCreatureNavigation.h"

bool CTCCreatureNavigation::GetNearestNavigablePosTo(const C3DVector& pos, float radius, C3DVector& result) const
{
    return PNavigator->GetNearestNavigablePosTo(pos, radius, result);
}
