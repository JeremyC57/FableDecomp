// CEngineLandscapeMemoryManager::AllocateVertexBuffer(CMovableResource* owner, EVertexType type, long count, long stride, unsigned long usage) -- after the disposal-list step, allocates a CVertexBufferWin32 from the global allocator (constructor inline) and initialises it in the managed pool
// Leaf: ?AllocateVertexBuffer@CEngineLandscapeMemoryManager@@QAEPAVCVertexBufferWin32@@PAVCMovableResource@@W4EVertexType@@JJK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BDA3D0, length 148 bytes (`ret 14h`; the owner is unused; Initialise runs even on a failed
// allocation, as in retail).
// Identity: Ego_r.pdb + FableWin.pdb ?AllocateVertexBuffer@CEngineLandscapeMemoryManager@@QAEPAVCVertexBufferWin32@@PAVCMovableResource@@W4EVertexType@@JJK@Z
//   (engine/CEngineLandscapeMemoryManager.h); the landed callers 0x00BE3660 / 0x00BEAE50 bind this address under that spelling.
// Callees: ProcessDisposalList 0x00BDA150 (private), CMemoryAllocator::Create / EnterSynchronisation / LeaveSynchronisation / InternalAllocate
//   (the inline CMemoryAllocator::Allocate, engine/CMemoryAllocator.h, the body of the landed AllocVirtual 0x00BD9AF0), CBaseClassNonCopyable's
//   constructor 0x0099A2F0 and CVertexBufferWin32::Initialise 0x00A63150; vtable 0x0129BA20.
// Port (NF149, q4552 / q6203 family): was the stand-in CVertexBufferWin32_Retail (class operator new) on local CBaseClassNonCopyable /
//   CVertexCompressionParams / CVertexBufferBase models with the self leaf ?AllocateVertexBuffer@CEngineLandscapeMemoryManager@@QAEPAUCVertexBufferWin32_Retail@@JJJJK@Z;
//   now on engine/CVertexBufferWin32.h + CEngineLandscapeMemoryManager.h (overlay opt-in ..._PROCESSDISPOSALLIST), the allocation a placement new.
#include <new>
#include "engine/CMemoryAllocator.h"
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER_PROCESSDISPOSALLIST
#define FABLE_RETAIL_CVERTEXBUFFERWIN32
#include "engine/EVertexType.h"
#include "engine/EVertexBufferPool.h"
#include "engine/CVertexBufferWin32.h"
#include "engine/CEngineLandscapeMemoryManager.h"

CVertexBufferWin32* CEngineLandscapeMemoryManager::AllocateVertexBuffer(CMovableResource* owner, EVertexType type, long count, long stride, unsigned long usage)
{
    ProcessDisposalList();
    CVertexBufferWin32* buffer = new (CMemoryAllocator::Allocate(sizeof(CVertexBufferWin32), 0)) CVertexBufferWin32;
    buffer->Initialise(type, count, stride, usage, VB_POOL_MANAGED);
    return buffer;
}
