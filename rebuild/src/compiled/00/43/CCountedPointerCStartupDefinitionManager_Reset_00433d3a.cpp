// CCountedPointer<CStartupDefinitionManager>::Reset(CStartupDefinitionManager* data) -- drop the current reference, take ownership of data with a fresh shared-count record
// Leaf: ?Reset@?$CCountedPointer@VCStartupDefinitionManager@@@@QAEXPAVCStartupDefinitionManager@@@Z
// Retail: Fable.exe @ 0x00433D3A, 59 bytes, `ret 4`; resets CStartupDefinitionManager::PClass 0x013B8760 (sole caller 0x004336B6,
//   CStartupDefinitionManager::Set). Callees: ReleaseData<CStartupDefinitionManager> 0x00433CC0 (FableWin.pdb 0x01B32A76), operator new
//   0x00BFEA1A; the stored deleter is DeleteData<CStartupDefinitionManager> 0x00433CB5 (FableWin.pdb 0x01B32A38: the virtual delete
//   `test ecx,ecx / mov eax,[ecx] / push 1 / call [eax]`, so the pointee is polymorphic).
// Names (NF233, 2026-11-22, q12635/q12636): FableWin.pdb 0x01B32936 ?Reset@?$CCountedPointer@VCStartupDefinitionManager@@@@QAEXPAVCStartupDefinitionManager@@@Z,
//   the spelling the landed binder 0x004336B0 (CStartupDefinitionManager::Set, nf232) binds; was the row-local
//   CCountedPointer<CPolymorphicData_00433D3A> stand-in. Ported onto engine/CCountedPointer.h as the sibling Reset rows 0x004194D1 /
//   0x0042FC13 (NF222): explicit instantiation of the header Reset with RELEASEDATA_DECL_ONLY + DELETEDATA_DECL_ONLY. Size TU (unaligned entry).
#pragma optimize("s", on)
void* __cdecl operator new(unsigned int size);             // 0x00BFEA1A
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#define FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
class CStartupDefinitionManager;
template void CCountedPointer<CStartupDefinitionManager>::Reset(CStartupDefinitionManager*);
