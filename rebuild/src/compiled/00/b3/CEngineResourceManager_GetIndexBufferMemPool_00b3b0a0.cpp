// CEngineResourceManager::GetIndexBufferMemPool() -- the memory pool engine index buffers are created in on PC: IB_POOL_MANAGED
// Leaf: ?GetIndexBufferMemPool@CEngineResourceManager@@QAE?AW4EIndexBufferPool@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B3B0A0, `mov eax,1 ; ret` (6 bytes).
// Name (NF152, 2026-11-20, q4563): Ego_r.pdb 0x009D96D0 / FableWin.pdb 0x02DD4440
//   ?GetIndexBufferMemPool@CEngineResourceManager@@QAE?AW4EIndexBufferPool@@XZ (public; the third of the out-of-line run after
//   GetVertexBufferUsage 0x00B3B030 / GetIndexBufferUsage 0x00B3B060 and GetVertexBufferMemPool 0x00B3B090). Its result feeds the pool
//   argument of CIndexBuffer::Initialise in the landed callers (0x00B51570, 0x00B620A0, 0x00BBF380, 0x00BE3110 ...), every one through
//   NEngineInternal::GPResourceManager. Was the label-propagated free function AddChildPrimitive (__fastcall, int).
// Value: IB_POOL_MANAGED (engine/EIndexBufferPool.h, the Ego_r / FableWin TPI enumerators). `this` is unused.
#include "fable_compiler.h"
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#include "engine/EIndexBufferPool.h"
#include "engine/CEngineResourceManager.h"

EIndexBufferPool CEngineResourceManager::GetIndexBufferMemPool()
{
    return IB_POOL_MANAGED;
}
