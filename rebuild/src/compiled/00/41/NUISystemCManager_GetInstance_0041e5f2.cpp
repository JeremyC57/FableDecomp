// NUISystem::CManager::GetInstance() -- returns the front-end UI manager singleton, creating it (0xD0 bytes) on first use
// Leaf: ?GetInstance@CManager@NUISystem@@SIPAV12@XZ
// Retail: Fable.exe @ 0x0041E5F2, 41 bytes, static. Callees: operator new 0x00BFEA1A, the CManager constructor 0x0041E3F6
// (stores vtable 0x01230134 = NUISystem::CManager : NUISystem::CObservable by RTTI; the catalog's module
// "CFrontEndManager" names the caller family). Global: the instance pointer 0x013B8710.
// Names (NF225, 2026-11-22): the singleton is the protected static ?GPUniqueInstance@CManager@NUISystem@@1PAV12@A (Ego_r.pdb, FableWin.pdb,
//   Ego_d.pdb), respelled in place in this row's model (was a made-up namespace global ?g_pUIManagerInstance@NUISystem@@3PAVCManager@1@A).
// Names (NF226, 2026-11-22, q12567): ported onto engine/NUISystem_CManager.h (FABLE_RETAIL_CMANAGER_GPUNIQUEINSTANCE; sizeof 0xD0 as
//   retail's allocation), so the constructor call binds the protected ??0CManager@NUISystem@@IAE@XZ (Ego_r.pdb 0x00438A19, FableWin.pdb
//   0x01AE4BC6), moved with its self row 0x0041E3F6 (was ??0CManager@NUISystem@@QAE@XZ on the row-local model, now gone).
// Shape: size TU (`pop ecx` after the allocation call); VC7.1's null-tested new expression; the instance is stored from eax on both paths and eax is returned.
#define FABLE_RETAIL_CMANAGER_GPUNIQUEINSTANCE
#pragma optimize("s", on)
#include "engine/NUISystem_CManager.h"
namespace NUISystem {
CManager* CManager::GetInstance()
{
    if (GPUniqueInstance == 0)
        GPUniqueInstance = new CManager;
    return GPUniqueInstance;
}
}
