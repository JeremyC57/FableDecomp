#pragma optimize("s",on)
// CCountedPointer::SetStaticCast<NUISystem::CComponent>(const CCountedPointer<NUISystem::CComponent>&) -- share a down-cast component pointer (T = NUISystem::CMesh)
// Leaf: ??$SetStaticCast@VCComponent@NUISystem@@@?$CCountedPointer@VCMesh@NUISystem@@@@QAEXABV?$CCountedPointer@VCComponent@NUISystem@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042A2B9, 17 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Was landed as _global::CCountedPointer_Assign_0042a2b9 (stand-in name).
// T = NUISystem::CMesh, proven by: NUISystem::CManager::Action 0x0041E6D3 binds SetStaticCast<CComponent> on CCountedPointer<NUISystem::CMesh> (`mesh.SetStaticCast(component)`); it calls ShareData 0x0042943A, whose release is ReleaseData<NUISystem::CMesh> 0x00429230; FableWin.pdb has this exact member-template symbol.
// Member: the member template SetStaticCast of engine/CCountedPointer.h (ShareData(static_cast<T*>(other.Data), other.Info)); in this
//   size TU ShareData stays out of line, so the COMDAT is the two pushes and the call.
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#include "engine/NUISystem_CMesh.h"
template void CCountedPointer<NUISystem::CMesh>::SetStaticCast(const CCountedPointer<NUISystem::CComponent>&);
