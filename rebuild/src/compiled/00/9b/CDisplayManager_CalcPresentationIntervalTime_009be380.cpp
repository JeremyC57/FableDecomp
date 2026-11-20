// CDisplayManager::CalcPresentationIntervalTime(long refresh_rate, long vsync_count) -- the presentation interval in seconds: vsync_count frames at refresh_rate Hz
// Leaf: ?CalcPresentationIntervalTime@CDisplayManager@@AAEMJJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009BE380, 11 bytes: `fild dword [esp+8] ; fidiv dword [esp+4] ; ret 8` (this is never read).
//   Expanded by CDisplayManager::CreateDevice (0x009BFA01 `fild [VSyncCount]` / 0x009BFA13 `fidiv [refresh]`, whose row carries this
//   text as a same-TU copy).
// Identity (re-port, audit 2026-11-20; landed 2026-10-22 as "_global::FUN_009be380" on a row-local stand-in class): Ego_r.pdb /
//   FableWin.pdb ?CalcPresentationIntervalTime@CDisplayManager@@AAEMJJ@Z; Ego_d.pdb procedure of the same name in
//   lib_display_manager.obj (debug length 339, file id 600 = the display manager's .cpp, lines 862..865; parameters refresh_rate,
//   vsync_count, both type 0x12 long): line 863 a debug-only assert (274 bytes), 864 the return (12 bytes, our /Od 6: VC10 adds a
//   float store / reload; casting both operands gives the same bytes, the single cast is the text landed in 2026-10). It sits between
//   ReadDeviceRefreshRate 0x009BE340 (Ego_d lines 847..858) and DestroyDevice 0x009BE390 in retail, the Ego_d source order.
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER_CREATEDEVICE
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTEXTURE
#include "engine/CDisplayManager.h"

float CDisplayManager::CalcPresentationIntervalTime(long refresh_rate, long vsync_count)
{
    return (float)vsync_count / refresh_rate;
}