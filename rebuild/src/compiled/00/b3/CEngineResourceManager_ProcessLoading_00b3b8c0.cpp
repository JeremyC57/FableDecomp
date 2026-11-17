// CEngineResourceManager::ProcessLoading() -- the CAsyncLoadingProcessCallback override: load with no deadline (ProcessLoading(false, 1e30)).
// Leaf: ?ProcessLoading@CEngineResourceManager@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B3B8C0, 23 bytes (`fld qword [0x012A1B28] ; sub esp,8 ; fstp [esp] ; push 0 ;
//   add ecx,-0Ch ; call 0x00B3B7F0 ; ret`).
// Identity (NF56, round 7, queue q10476): Ego_r.pdb 0x009D9F20 / FableWin.pdb 0x02DD3C00 ?ProcessLoading@CEngineResourceManager@@UAEXXZ, the
//   override of CAsyncLoadingProcessCallback::ProcessLoading (engine/CEngineResourceManager.h: : CEngineComponent, CAsyncLoadingProcessCallback
//   at +0x0C, so the slot is entered with `this` on that base: `add ecx,-0Ch`). It calls the non-virtual overload
//   ?ProcessLoading@CEngineResourceManager@@QAEX_NN@Z 0x00B3B7F0 (FABLE_RETAIL_CENGINERESOURCEMANAGER_PROCESSLOADING_UNTIL) with false and the
//   double 0x012A1B28 = 1.0000000150474662e+30, the float 1e30f widened. Was the stand-in free function CEngineResourceManager_ProcessLoading
//   over a local SubObj::Do(int, double) and an extern g_procLoadTime.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_PROCESSLOADING_UNTIL
#include "fable_string.h"
#include "engine/CEngineResourceManager.h"

void CEngineResourceManager::ProcessLoading()
{
    ProcessLoading(false, 1e30f);
}
