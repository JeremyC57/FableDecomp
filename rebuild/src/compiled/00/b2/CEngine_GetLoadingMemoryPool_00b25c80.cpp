// CEngine::GetLoadingMemoryPool() -- the memory pool held by the resource manager's counted pointer at +0x8e0, read through a by-value copy of the counted pointer (add-ref then release folded together)
// Leaf: ?GetLoadingMemoryPool@CEngine@@UAEPAVCMovableResourceMemoryPool@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B25C80, length 55 bytes.
// Callees: the pointer info's deleter at +4 (called with the object at +8 in ecx when the count reaches zero), operator delete @ 0x00BFE9BC for the info block.
// Shape: the CCountedPointer temporary's copy constructor (`++count`) and destructor (`--count`, destroy at zero) are both inlined and share one null test of
// the info pointer; the compiler keeps both count stores (`mov [esi],ecx ; mov [esi],eax`). The object pointer (edi) is the result.
// NF46 (round 7): identity Ego_r.pdb / FableWin.pdb ?GetLoadingMemoryPool@CEngine@@UAEPAVCMovableResourceMemoryPool@@XZ, a CIEngine override
//   (`this` unused; CEngine.h overlay opt-in _GETMEMORYPOOLS); was landed as a static member returning a local CMemoryPool through a local
//   counted-pointer stand-in on the g_pEngineData_01436e98 stand-in. The 0x01436E98 global is NEngineInternal::GPResourceManager
//   (?GPResourceManager@NEngineInternal@@3PAVCEngineResourceManager@@A) and +0x8e0 its LoadingMemoryPool (Ego_r.pdb), read through FableWin.pdb's
//   inline GetLoadingMemoryPool (CEngineResourceManager.h overlay opt-in).
#define FABLE_RETAIL_CENGINE
#define FABLE_RETAIL_CENGINE_GETMEMORYPOOLS
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_GETLOADINGMEMORYPOOL
#include "engine/CEngine.h"
#include "engine/CEngineResourceManager.h"

namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }   // 0x01436E98

CMovableResourceMemoryPool* CEngine::GetLoadingMemoryPool()
{
    return NEngineInternal::GPResourceManager->GetLoadingMemoryPool().Get();
}
