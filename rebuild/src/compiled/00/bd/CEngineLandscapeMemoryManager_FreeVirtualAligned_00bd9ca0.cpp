// CEngineLandscapeMemoryManager::FreeVirtualAligned(void*) -- release a block from AllocVirtualAligned: step back by the alignment offset stored in the dword before the block and free the raw allocation
// Leaf: ?FreeVirtualAligned@CEngineLandscapeMemoryManager@@QAEXPAX@Z
// Retail: Fable.exe @ 0x00BD9CA0, 29 bytes, `ret 4`; ?FreeVirtualAligned@CEngineLandscapeMemoryManager@@QAEXPAX@Z (Ego_r 009c2430 /
//   FableWin 02cb7a40 name sets; Ego_r's AllocVirtualAligned 009c23d0 / FreeVirtualAligned 009c2430 / DeleteTextureBlockWhileBusy 009c2450
//   run = retail 0x00BD9C40 / 0x00BD9CA0 / 0x00BD9CC0). Called three times by CLocalDetailPrimitiveRepeatedMesh's destructor 0x00BFAAE0
//   (0x00BFAB06 / 0x00BFAB11 / 0x00BFAB27, its aligned arrays) next to FreeVirtual; the catalog label _Cons_val<pair<CCharString,
//   CCharString>> is propagated.
// Callees: CEngineLandscapeMemoryManager::FreeVirtual(void*) 0x00BD9B90 (a tail jump: the adjusted pointer overwrites the argument slot).
// Layout: engine/CEngineLandscapeMemoryManager.h retail branch (declaration opt-in FREEVIRTUALALIGNED); this method touches no member.
// Shape: AllocVirtualAligned 0x00BD9C40 over-allocates and stores (aligned - raw) at aligned[-1]; a null block is ignored.
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER_FREEVIRTUALALIGNED   // FreeVirtualAligned 0x00BD9CA0 (Ego_r + FableWin)
#include "engine/CEngineLandscapeMemoryManager.h"

void CEngineLandscapeMemoryManager::FreeVirtualAligned(void* data)
{
    if (data != 0)
        FreeVirtual((char*)data - ((long*)data)[-1]);
}
