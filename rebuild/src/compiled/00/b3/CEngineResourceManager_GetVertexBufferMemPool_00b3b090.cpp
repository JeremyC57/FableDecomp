// CEngineResourceManager::GetVertexBufferMemPool() -- the PC build always answers pool 1 (`mov eax,1 ; ret`).
// Leaf: ?GetVertexBufferMemPool@CEngineResourceManager@@QAE?AW4EVertexBufferPool@@XZ
// NF37 (2026-11-16): moved off the free AddChildPrimitive stand-in onto the shared engine/CEngineResourceManager.h (VBPOOL opt-in, which names
// 0x00B3B090; Ego_r.pdb ?GetVertexBufferMemPool@CEngineResourceManager@@QAE?AW4EVertexBufferPool@@XZ).
#include "fable_compiler.h"
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_VBPOOL
#include "engine/EVertexType.h"
#include "engine/EPrimitiveType.h"
#include "engine/CEngineResourceManager.h"

EVertexBufferPool CEngineResourceManager::GetVertexBufferMemPool()
{
    return (EVertexBufferPool)1;
}
