// CCountedPointer::Reset(T* p) -- (CCountedPointer<NInventory::CPerItemData >) drop the current share through ReleaseData, then adopt p with a fresh count block
// Leaf: ?Reset@?$CCountedPointer@VCPerItemData@NInventory@@
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C1946, 59 bytes (size-optimised TU, odd entry), thiscall, `ret 4`.
// Identity: CCountedPointer<NInventory::CPerItemData >::Reset(NInventory::CPerItemData*) -- proven; evidence: its callers (NInventory item code 0x005BEC85 / 0x005BECD7 / 0x005BEE35 / 0x005BEEFE ...) reset the item's +0x08 pointer (Ego_r.pdb CItem::PPerItemData, CCountedPointer<NInventory::CPerItemData>) with a new CPerItemData (default ctor 0x005C14A6: Augmentations vector, counters, -1 ref count field; copy ctor 0x005C14C4). FableWin.pdb name set holds ?Reset@?$CCountedPointer@VCPerItemData@NInventory@@@@QAEXPAVCPerItemData@NInventory@@@Z. Family G054 (CCountedPointer
//   Reset bodies; the propagated "CCountedPointer<std::vector<CCharString>>::Reset" is right only for 0x005C1E91).
// Callees: ReleaseData() 0x005C04BF (this instantiation's own COMDAT), operator new 0x00BFEA1A (12-byte count block);
//   DeleteData 0x005C18E3 (static __fastcall deleter stored in the block).
// Shape: Data is stored before the null test; the block takes Data re-read through `this`; a failed new stores 0.
#pragma optimize("s", on)
void* __cdecl operator new(unsigned int size);             // 0x00BFEA1A
namespace NInventory { class CPerItemData; }
#include "engine/CCountedPointer.h"
template void CCountedPointer<NInventory::CPerItemData >::Reset(NInventory::CPerItemData*);