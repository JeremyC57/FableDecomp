// CCountedPointer::Reset(T* p) -- (CCountedPointer<CASoundBank >) drop the current share through ReleaseData, then adopt p with a fresh count block
// Leaf: ?Reset@?$CCountedPointer@VCASoundBank@@
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0042FD6F, 59 bytes (size-optimised TU, odd entry), thiscall, `ret 4`.
// Identity: CCountedPointer<CASoundBank >::Reset(CASoundBank*) -- probable; evidence: CNewFrontendGameComponent's front-end sound init 0x0042E17C resets this+0x50 (Ego_r.pdb CNewFrontendGameComponent::PSampleBank, CCountedPointer<CASoundBank> at +80) with a new 0x20-byte bank, and RegisterSoundBanks 0x009919C0 / 0x00991C10 reset local bank pointers with it; DeleteData 0x0042F8A4 deletes through the virtual destructor. (FableWin.pdb has no Reset for this pointee.) Family G054 (CCountedPointer
//   Reset bodies; the propagated "CCountedPointer<std::vector<CCharString>>::Reset" is right only for 0x005C1E91).
// Callees: ReleaseData() 0x0042F9A0 (this instantiation's own COMDAT), operator new 0x00BFEA1A (12-byte count block);
//   DeleteData 0x0042F8A4 (static __fastcall deleter stored in the block).
// Shape: Data is stored before the null test; the block takes Data re-read through `this`; a failed new stores 0.
#pragma optimize("s", on)
void* __cdecl operator new(unsigned int size);             // 0x00BFEA1A
class CASoundBank;
#include "engine/CCountedPointer.h"
template void CCountedPointer<CASoundBank >::Reset(CASoundBank*);