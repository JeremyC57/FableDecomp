// CMovableResourceMemoryPool::CMovableResourceMemoryPool() -- the CHandleBasedMemoryPool base, then this class's vtable
// Leaf: ??0CMovableResourceMemoryPool@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A62E20, 18 bytes: push esi ; mov esi,ecx ; call CHandleBasedMemoryPool::CHandleBasedMemoryPool
//   0x00A02D20 ; mov [esi],offset CMovableResourceMemoryPool::`vftable' (0x0129D6A0) ; mov eax,esi ; pop esi ; ret.
// Identity: Ego_r.pdb 0x00BDE5F0 / FableWin.pdb 0x03385E10 ??0CMovableResourceMemoryPool@@QAE@XZ; base CHandleBasedMemoryPool from the shared
//   engine/CMovableResourceMemoryPool.h (raw PDB bases). Port (NF133, q4730): landed earlier as one of the generated bodies of this shape
//   with a made-up CEnginePrimitiveTypeInfo base, which bound ??0CEnginePrimitiveTypeInfo@@QAE@XZ (the real constructor of that class is
//   0x00B8FAA0) to 0x00A02D20; now the base call binds Ego_r.pdb / FableWin.pdb ??0CHandleBasedMemoryPool@@QAE@XZ, as the re-authored
//   sibling 0x00A148D0 CAsyncLoadingMemoryPool does.
#define FABLE_RETAIL_CHANDLEBASEDMEMORYPOOL
#define FABLE_RETAIL_CMOVABLERESOURCEMEMORYPOOL
#include "engine/CMovableResourceMemoryPool.h"

CMovableResourceMemoryPool::CMovableResourceMemoryPool()
    : CHandleBasedMemoryPool()
{
}
