// CEngineLandscapeMemoryManager::FreeVirtualAligned(void*) -- release a block from AllocVirtualAligned: step back by the alignment offset stored in the dword before the block and free the raw allocation
// Retail: Fable.exe @ 0x00BD9CA0, 29 bytes, `ret 4`; ?FreeVirtualAligned@CEngineLandscapeMemoryManager@@QAEXPAX@Z (Ego_r/FableWin PDB names).
//   Called by the landscape primitives' destructors (0x00BFAB06, ...) next to FreeVirtual; the catalog label _Cons_val<pair<CCharString,
//   CCharString>> is propagated.
// Callees: CEngineLandscapeMemoryManager::FreeVirtual(void*) 0x00BD9B90 (a tail jump: the adjusted pointer overwrites the argument slot).
// Layout: engine/CEngineLandscapeMemoryManager.h; this method touches no member (this passes through in ecx).
// Shape: AllocVirtualAligned 0x00BD9C40 over-allocates and stores (aligned - raw) at aligned[-1]; a null block is ignored.
#define CEngineLandscapeMemoryManager CEngineLandscapeMemoryManagerLayout
#include "engine/CEngineLandscapeMemoryManager.h"
#undef CEngineLandscapeMemoryManager

class CEngineLandscapeMemoryManager : public CEngineLandscapeMemoryManagerLayout
{
public:
    void FreeVirtual(void*);   // 0x00BD9B90
};
struct CEngineLandscapeMemoryManagerMethods : CEngineLandscapeMemoryManager {   // method projection over the engine layout
    void FreeVirtualAligned(void* data);
};
void CEngineLandscapeMemoryManagerMethods::FreeVirtualAligned(void* data)
{
    if (data != 0)
        FreeVirtual((char*)data - ((long*)data)[-1]);
}