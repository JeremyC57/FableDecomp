// NUISystem::CManager::`scalar deleting destructor'(unsigned int) -- run the protected non-virtual ~CManager, free the object when bit 0 of the flags is set, return this
// Leaf: ??_GCManager@NUISystem@@IAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042DB20, 30 bytes, thiscall `ret 4`. Callees: ~CManager 0x0041E641 (??1CManager@NUISystem@@IAE@XZ,
//   Ego_r.pdb / FableWin.pdb / Ego_d.pdb), operator delete 0x00BFE9BC. Its only caller is the delete in the inline CManager::Clear (0x0042FB40).
// Names (NF225, 2026-11-22): was a row-local `struct CManager` stand-in at global scope with a made-up ?Dtor@CManager@@QAEXXZ member
//   (leaf ?ScalarDeletingDestructor@CManager@@...). Leaf from FableWin.pdb (S_GPROC32 NUISystem::CManager::`scalar deleting destructor',
//   new_frontend_game_component.obj, marked inline) and Ego_d.pdb; protected (`I`) because ~CManager is protected in all three PDBs.
// Model: engine/NUISystem_CManager.h opt-ins FABLE_RETAIL_CMANAGER_CLEAR_INLINE + FABLE_RETAIL_CMANAGER_GPUNIQUEINSTANCE. ~CManager is
//   protected and non-virtual, so only a member of the class can emit this ??_G: the header's inline Clear (both PDBs: marked inline, its
//   only copy in the new front-end compiland, as this function's) deletes GPUniqueInstance. The anonymous-namespace scaffold below makes
//   the TU emit Clear and the ??_G it calls as pick-any COMDATs (descriptive internal name, not retail; the 0x00415B80 pattern).
#define FABLE_RETAIL_CMANAGER_CLEAR_INLINE
#define FABLE_RETAIL_CMANAGER_GPUNIQUEINSTANCE
#include "engine/NUISystem_CManager.h"
namespace {
void ReferenceManagerClear()
{
    NUISystem::CManager::Clear();
}
}
