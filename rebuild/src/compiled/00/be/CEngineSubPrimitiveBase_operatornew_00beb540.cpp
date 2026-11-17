// CEngineSubPrimitiveBase::operator new(unsigned int size) -- allocate a sub-primitive from the class's memory pool
// Leaf: ??2CEngineSubPrimitiveBase@@SIPAXI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BEB540, 13 bytes: `push ecx ; mov ecx,[0143BAC4h] ; call 0x00A5C910 ; ret` (__fastcall size in ecx,
//   then CSimpleMemoryPool::Allocate(size) on the static pool).
// Identity (two sources): Ego_r.pdb 0x00AA37F0 + FableWin.pdb 0x02ED8C20 ??2CEngineSubPrimitiveBase@@SIPAXI@Z (engine/CEngineSubPrimitiveBase.h
//   declares it); Ego_r donor order operator new / operator delete 0x00AA37F0 / 0x00AA3800 = retail 0x00BEB540 / 0x00BEB550 (the delete is a
//   bare `ret`: the pool is a frame pool). The callers 0x00B9A4A0 / 0x00BE1BA0 construct CEngineSubPrimitiveBase-derived objects on the result.
//   The pool 0x0143BAC4 is the private static ?MemoryPool@CEngineSubPrimitiveBase@@0PAVCSimpleMemoryPool@@A (Ego_d.pdb PUB32 + the
//   shared header's private static MemoryPool); Allocate 0x00A5C910 is ?Allocate@CSimpleMemoryPool@@QAEPAXK@Z (engine/CSimpleMemoryPool.h).
// NF79 (round 7, q10974): re-authored on the shared headers; was the free fastcall New_beb540 over a local G / g_beb540 model.
#define FABLE_RETAIL_CSIMPLEMEMORYPOOL
#include "engine/CSimpleMemoryPool.h"
#include "engine/CEngineSubPrimitiveBase.h"
void* __fastcall CEngineSubPrimitiveBase::operator new(unsigned int size)
{
    return MemoryPool->Allocate(size);
}
