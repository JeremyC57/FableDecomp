// NUISystem::CManager::Clear() -- destroy the UI manager singleton and forget it
// Leaf: ?Clear@CManager@NUISystem@@SIXXZ
// Retail: Fable.exe @ 0x0042FB40, 39 bytes, static __fastcall; Ego_r.pdb ?Clear@CManager@NUISystem@@SIXXZ. Catalog label
// std::pair<CCharString, CTCBase::CRegisteredTCInfo>::swap is wrong: the body is `delete GPUniqueInstance; GPUniqueInstance = 0` on the
// singleton pointer 0x013B8710 -- the scalar deleting destructor 0x0042DB20 (??_GCManager@NUISystem@@IAEPAXI@Z: the out-of-line
// non-virtual ~CManager 0x0041E641, Ego_r.pdb ??1CManager@NUISystem@@IAE@XZ, protected and non-virtual, then operator delete 0x00BFE9BC),
// expanded here. Callers 0x0042EA1E / 0x005954E5.
// Names (NF225, 2026-11-22): the singleton is the protected static ?GPUniqueInstance@CManager@NUISystem@@1PAV12@A (Ego_r.pdb, FableWin.pdb,
//   Ego_d.pdb; was a made-up public ?Instance@CManager@NUISystem@@2PAV12@A on a row-local model of NUISystem::CManager / CObservable).
// Model: engine/NUISystem_CManager.h opt-ins FABLE_RETAIL_CMANAGER_CLEAR_INLINE + FABLE_RETAIL_CMANAGER_GPUNIQUEINSTANCE. Clear is an
//   inline member (Ego_r.pdb / FableWin.pdb S_FRAMEPROC "marked inline"; its only copy in new_frontend_game_component.obj, not CManager's
//   ui_manager.obj), so this row's leaf is its pick-any COMDAT, as in the 0x0042DB20 row (same TU in retail). The anonymous-namespace
//   scaffold below makes the TU emit it (descriptive internal name, not retail; the 0x00415B80 pattern).
#define FABLE_RETAIL_CMANAGER_CLEAR_INLINE
#define FABLE_RETAIL_CMANAGER_GPUNIQUEINSTANCE
#include "engine/NUISystem_CManager.h"
namespace {
void ReferenceManagerClear()
{
    NUISystem::CManager::Clear();
}
}
