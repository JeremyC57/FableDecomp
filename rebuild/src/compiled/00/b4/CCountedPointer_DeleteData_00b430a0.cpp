// CCountedPointer<CStaticMapBankStateBlock>::DeleteData(void* data) -- the counted-pointer deleter: `delete static_cast<CStaticMapBankStateBlock*>(data)` (virtual deleting destructor)
// Leaf: ?DeleteData@?$CCountedPointer@VCStaticMapBankStateBlock@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B430A0, 11 bytes, static __fastcall (`test ecx,ecx ; je ; mov eax,[ecx] ; push 1 ; call [eax]`).
// Name (NF70, q10753): FableWin.pdb's CCountedPointer<T>::DeleteData `SIXPAX@Z` family (engine/CCountedPointer.h, 343 instantiations) for
//   T = CStaticMapBankStateBlock (RTTI .?AVCStaticMapBankStateBlock@@ : CBankStateBlock, vtable 0x012A1F3C, virtual dtor in slot 0);
//   was a free-function stand-in DeleteData(AutoTinyVirtualObject*). Same form as the landed DeleteData rows 0x00BB6330 / 0x00834A10.
#define FABLE_RETAIL_CSTATICMAPBANKSTATEBLOCK_CLASS
#include "fable_compiler.h"
#include "engine/CBankStateBlock.h"
#include "engine/CStaticMapBankStateBlock.h"
#include "engine/CCountedPointer.h"
template void FABLE_FASTCALL CCountedPointer<CStaticMapBankStateBlock>::DeleteData(void*);
