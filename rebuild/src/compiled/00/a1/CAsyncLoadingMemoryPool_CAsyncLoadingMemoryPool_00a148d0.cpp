// CAsyncLoadingMemoryPool::CAsyncLoadingMemoryPool() -- the CHandleBasedMemoryPool base, then this class's vtable
// Leaf: ??0CAsyncLoadingMemoryPool@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A148D0, 18 bytes: push esi ; mov esi,ecx ; call CHandleBasedMemoryPool::CHandleBasedMemoryPool
//   0x00A02D20 ; mov [esi],offset CAsyncLoadingMemoryPool::`vftable' (0x0129CA6C) ; mov eax,esi ; pop esi ; ret.
// Identity: Ego_r.pdb ??0CAsyncLoadingMemoryPool@@QAE@XZ (0x00B82350, then ~ 0x00B82370 and ProcessLoading 0x00B82380 as retail
//   0x00A14900 follows); RTTI .?AVCAsyncLoadingMemoryPool@@ : CHandleBasedMemoryPool. The base call is ??0CHandleBasedMemoryPool@@QAE@XZ.
// (Landed earlier as one of 265 generated bodies of this shape with a made-up CEnginePrimitiveTypeInfo base, which bound that class's
//   constructor name to 0x00A02D20 -- the real one is 0x00B8FAA0; re-authored 2026-11-16 on the shared headers.)
#define FABLE_RETAIL_CHANDLEBASEDMEMORYPOOL
#define FABLE_RETAIL_CASYNCLOADINGMEMORYPOOL_CTOR
#include "engine/CAsyncLoadingMemoryPool.h"

CAsyncLoadingMemoryPool::CAsyncLoadingMemoryPool()
    : CHandleBasedMemoryPool()
{
}
