#pragma optimize("s",on)
// CCountedPointer::SetStaticCast<NUISystem::CComponent>(const CCountedPointer<NUISystem::CComponent>&) -- share a down-cast component pointer (T = NUISystem::CList)
// Leaf: ??$SetStaticCast@VCComponent@NUISystem@@@?$CCountedPointer@VCList@NUISystem@@@@QAEXABV?$CCountedPointer@VCComponent@NUISystem@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042A2DB, 17 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Was landed as _global::CCountedPointer_Assign_0042a2db (stand-in name).
// T = NUISystem::CList, proven by: 0x00558770 binds SetStaticCast<CComponent> on CCountedPointer<NUISystem::CList>; it calls ShareData 0x00429486, whose release is 0x00429288, the landed CCountedPointer<NUISystem::CList> release; FableWin.pdb has this exact member-template symbol.
// Member: the member template SetStaticCast of engine/CCountedPointer.h (ShareData(static_cast<T*>(other.Data), other.Info)); in this
//   size TU ShareData stays out of line, so the COMDAT is the two pushes and the call.
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#include "engine/NUISystem_CList.h"
template void CCountedPointer<NUISystem::CList>::SetStaticCast(const CCountedPointer<NUISystem::CComponent>&);
