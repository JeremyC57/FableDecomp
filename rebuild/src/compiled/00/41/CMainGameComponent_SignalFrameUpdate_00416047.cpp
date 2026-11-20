// CMainGameComponent::SignalFrameUpdate() -- clear FirstWorldFrameUpdate, then tell the memory allocator a new frame started (tail call)
// Leaf: ?SignalFrameUpdate@CMainGameComponent@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00416047, 15 bytes: `mov byte ptr [ecx+44h],0 ; mov ecx,[0x013CAA90] ; jmp 0x009F26D0`.
// Naming (NF161, 2026-11-20, q4196): the object at 0x013CAA90 is the protected static ?PMemoryAllocator@CMemoryAllocator@@1PAV1@A
//   (Ego_r.pdb / FableWin.pdb / Ego_d.pdb), reached through the public inline ?Get@CMemoryAllocator@@SIPAV1@XZ (FableWin.pdb / Ego_d.pdb) of
//   the shared engine/CMemoryAllocator.h; the tail-called 0x009F26D0 is ?NewFrame@CMemoryAllocator@@QAEXXZ (Ego_r.pdb / FableWin.pdb;
//   Ego_r donor order ClearPerformanceStats / NewFrame = retail 0x009F26B0 / 0x009F26D0). Was the stand-in Iface::Add on ?g_iface@@3PAUIface@@A.
// Naming (NF198, 2026-11-20, q12005): the row is CMainGameComponent::SignalFrameUpdate: Ego_r.pdb 0x00415265 + FableWin.pdb 0x018A8D6F
//   ?SignalFrameUpdate@CMainGameComponent@@QAEXXZ (15 bytes in Ego_r too; Ego_r order InitDefinitions, SignalFrameUpdate,
//   InitEventPackageLoading), the byte +0x44 is CMainGameComponent::FirstWorldFrameUpdate (Ego_r.pdb layout; engine/CMainGameComponent.h),
//   and the landed caller 0x00418289 (CMainGameComponent::Update) binds this spelling. Moved off the stand-in T::AddToInterface (catalog
//   CEditInputProcessCopyPaste::AddToInterface, a propagated label) onto engine/CMainGameComponent.h (FABLE_RETAIL_CMAINGAMECOMPONENT_UPDATE).
#define FABLE_RETAIL_CMEMORYALLOCATOR_VARIABLESIZE
#define FABLE_RETAIL_CMEMORYALLOCATOR_NEWFRAME
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_UPDATE
#include "rebuild_abi.h"
#include "engine/CMemoryAllocator.h"
#include "engine/CMainGameComponent.h"
void CMainGameComponent::SignalFrameUpdate()
{
    FirstWorldFrameUpdate = false;
    CMemoryAllocator::Get()->NewFrame();
}
