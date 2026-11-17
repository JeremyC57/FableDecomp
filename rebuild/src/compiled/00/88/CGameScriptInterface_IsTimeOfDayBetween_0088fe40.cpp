// CGameScriptInterface::IsTimeOfDayBetween(long start, long end) const -- script query: whether the world's game clock lies between the two times of day
// Leaf: ?IsTimeOfDayBetween@CGameScriptInterface@@UBE_NJJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0088FE40, size-optimised TU (`#pragma optimize("s",on)`, as the landed row had it).
// Identity: Ego_r.pdb / FableWin.pdb ?IsTimeOfDayBetween@CGameScriptInterface@@UBE_NJJ@Z, slot 650 of engine/CGameScriptInterface.h
//   (vtable 0x01260F0C). Callee: CGameTimeManager::IsTimeBetween(long, long) const 0x006BB390 (Ego_r.pdb 0x006F6B80 + FableWin.pdb
//   0x024607E0 ?IsTimeBetween@CGameTimeManager@@QBE_NJJ@Z) on World (+0x04) -> PGameTimeManager (+0x1C), through the inline FableWin.pdb
//   accessor CWorld::PeekGameTimeManager().
// Name fix (agentNF55, 2026-11-17): re-authored on the shared headers. It was the free stand-in
//   ?CGameScriptInterface_IsTimeOfDayBetween@@YI_NPAXJJ@Z over two overlay structs, calling the made-up
//   `anonymous`::Game_IsTimeOfDayInRange(void*, long, long).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CGAMETIMEMANAGER
#define FABLE_RETAIL_CGAMETIMEMANAGER_ISTIMEBETWEEN
#include "engine/CGameScriptInterface.h"
#include "engine/CWorld.h"
#include "engine/CGameTimeManager.h"
#pragma optimize("s",on)

bool CGameScriptInterface::IsTimeOfDayBetween(long start, long end) const
{
    return World->PeekGameTimeManager().IsTimeBetween(start, end);
}
