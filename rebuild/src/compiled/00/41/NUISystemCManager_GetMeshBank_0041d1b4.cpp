// NUISystem::CManager::GetMeshBank() -- a counted copy of the UI mesh bank (PMeshBank +0x18), returned through the hidden result
// Leaf: ?GetMeshBank@CManager@NUISystem@@QAE?AV?$CCountedPointer@VCMeshDataBank@@@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0041D1B4, 20 bytes, size TU (unaligned entry): `add ecx,18h ; push ecx ; mov ecx,[esp+8] ;
//   call 0x00428905 ; mov eax,[esp+4] ; ret 4` (the copy constructor ??0?$CCountedPointer@VCMeshDataBank@@@@QAE@ABV0@@Z, out of line).
// NF104 (q4912, 2026-11-19): moved off its `T::GetBankHandle(Bank*)` / `Bank::Register` stand-in (catalog CBankFile::GetBankHandle, propagated)
//   onto the shared engine/NUISystem_CManager.h (FABLE_RETAIL_CMANAGER declares GetMeshBank, PMeshBank the second counted pointer after PGraphicsBank)
//   and engine/CCountedPointer.h (FABLE_RETAIL_CCOUNTEDPOINTER_COPY_DECL_ONLY: the copy constructor called out of line, bound as the landed
//   0x0042DD28 binds it). Name: Ego_r.pdb 0x0042D9E8 + FableWin.pdb 0x01AE4B91 ?GetMeshBank@CManager@NUISystem@@QAE?AV?$CCountedPointer@VCMeshDataBank@@@@XZ;
//   CMesh::Draw 0x005502E0 calls it twice on CManager::GetInstance() with a hidden CCountedPointer slot, then reads the bank.
#pragma optimize("s",on)
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CCOUNTEDPOINTER_COPY_DECL_ONLY
#include "engine/CCountedPointer.h"
#include "engine/NUISystem_CManager.h"

namespace NUISystem {
CCountedPointer<CMeshDataBank> CManager::GetMeshBank()
{
    return PMeshBank;
}
}
