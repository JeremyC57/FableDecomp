// Clear a byte flag, then tail-call a member of a fixed global object (0x013CAA90).
// __fastcall this=ecx, no args.
// Naming (NF161, 2026-11-20, q4196): the object at 0x013CAA90 is the protected static ?PMemoryAllocator@CMemoryAllocator@@1PAV1@A
//   (Ego_r.pdb / FableWin.pdb / Ego_d.pdb), reached through the public inline ?Get@CMemoryAllocator@@SIPAV1@XZ (FableWin.pdb / Ego_d.pdb) of
//   the shared engine/CMemoryAllocator.h; the tail-called 0x009F26D0 is ?NewFrame@CMemoryAllocator@@QAEXXZ (Ego_r.pdb / FableWin.pdb;
//   Ego_r donor order ClearPerformanceStats / NewFrame = retail 0x009F26B0 / 0x009F26D0). Was the stand-in Iface::Add on ?g_iface@@3PAUIface@@A.
#define FABLE_RETAIL_CMEMORYALLOCATOR_VARIABLESIZE
#define FABLE_RETAIL_CMEMORYALLOCATOR_NEWFRAME
#include "rebuild_abi.h"
#include "engine/CMemoryAllocator.h"
#pragma pack(push,1)
struct T { char pad[0x44]; char flag; void AddToInterface(); };
#pragma pack(pop)
void T::AddToInterface() {
    this->flag = 0;
    CMemoryAllocator::Get()->NewFrame();
}
