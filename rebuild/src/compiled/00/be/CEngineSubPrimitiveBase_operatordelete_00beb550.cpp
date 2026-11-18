// CEngineSubPrimitiveBase::operator delete(void*) -- a no-op: sub-primitives live in the per-frame CSimpleMemoryPool (operator new
// 0x00BEB540 allocates from CEngineSubPrimitiveBase::MemoryPool), which is reset as a whole, so a single object is never freed
// Leaf: ??3CEngineSubPrimitiveBase@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BEB550, 1 byte (`ret`).
// Identity (NF83, round 7, q11036 self): Ego_r.pdb 0x00AA3800 / FableWin.pdb 0x02ED8C40 ??3CEngineSubPrimitiveBase@@SIXPAX@Z, the
//   symbol right after operator new 0x00AA37F0 = retail 0x00BEB540 in the donor order; engine/CEngineSubPrimitiveBase.h declares it
//   (static __fastcall); the landed 0x00BC7A80 binds this address under this spelling. Was the stand-in _global::Nop_ret0_00beb550
//   (leaf ?M@C00beb550@@QAEXXZ, a member of a local empty class).
#include "engine/CEngineSubPrimitiveBase.h"
void __fastcall CEngineSubPrimitiveBase::operator delete(void*)
{
}
