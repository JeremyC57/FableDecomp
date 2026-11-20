// CEngineLandscapeMemoryManager::AllocateIndexBuffer(CMovableResource* owner, long count, unsigned long usage, EIndexBufferFormat format, EPrimitiveType type) -- after the disposal-list step, allocates a CIndexBuffer from the global allocator and initialises it in the managed pool; null when the allocation fails
// Leaf: ?AllocateIndexBuffer@CEngineLandscapeMemoryManager@@QAEPAVCIndexBuffer@@PAVCMovableResource@@JKW4EIndexBufferFormat@@W4EPrimitiveType@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BDA360, length 107 bytes (`ret 14h`; the owner is unused).
// Identity: Ego_r.pdb + FableWin.pdb ?AllocateIndexBuffer@CEngineLandscapeMemoryManager@@QAEPAVCIndexBuffer@@PAVCMovableResource@@JKW4EIndexBufferFormat@@W4EPrimitiveType@@@Z
//   (engine/CEngineLandscapeMemoryManager.h); the landed caller 0x00BE3660 binds this address under that spelling.
// Callees: ProcessDisposalList 0x00BDA150 (private), the inline CMemoryAllocator::Allocate (engine/CMemoryAllocator.h; Create 0x009F2870,
//   Enter / LeaveSynchronisation 0x009F2000 / 0x009F2010, InternalAllocate(0x14, 0, 0) 0x009F2A40: the body of the landed AllocVirtual
//   0x00BD9AF0), CIndexBuffer::CIndexBuffer 0x00AC1EC0 (out of line), CIndexBuffer::Initialise 0x00AC1F70.
// Shape: the new-expression's null test guards the out-of-line constructor; the result is tested again for the early `return 0`.
// Port (NF149, q4557): was a local CIndexBuffer model with a class operator new and the self leaf
//   ?AllocateIndexBuffer@CEngineLandscapeMemoryManager@@QAEPAVCIndexBuffer@@JKKJJ@Z (binding ?Initialise@CIndexBuffer@@QAEXKKJJJ@Z); now on
//   engine/CIndexBuffer.h + CEngineLandscapeMemoryManager.h (overlay opt-in ..._PROCESSDISPOSALLIST), the allocation a placement new.
#include <new>
#include "engine/CMemoryAllocator.h"
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER_PROCESSDISPOSALLIST
#define FABLE_RETAIL_CINDEXBUFFER
#include "engine/EIndexBufferFormat.h"
#include "engine/EIndexBufferPool.h"
#include "engine/EPrimitiveType.h"
#include "engine/CIndexBuffer.h"
#include "engine/CEngineLandscapeMemoryManager.h"

CIndexBuffer* CEngineLandscapeMemoryManager::AllocateIndexBuffer(CMovableResource* owner, long count, unsigned long usage, EIndexBufferFormat format, EPrimitiveType type)
{
    ProcessDisposalList();
    CIndexBuffer* buffer = new (CMemoryAllocator::Allocate(sizeof(CIndexBuffer), 0)) CIndexBuffer;
    if (!buffer)
        return 0;
    buffer->Initialise(count, usage, format, IB_POOL_MANAGED, type);
    return buffer;
}
