#pragma optimize("s",on)
// CCountedPointer::ReleaseData() -- drop one reference (delete the pointee and the count block at zero), then zero Data and Info (T = NPlayerGui::CDrawQuestInfo::CQuestInfoElementCounterList)
// Leaf: ?ReleaseData@?$CCountedPointer@VCQuestInfoElementCounterList@CDrawQuestInfo@NPlayerGui@@@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00654BD9, 44 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Was landed as _global::CCountedPointer_Release_00654bd9 (stand-in name).
// T = NPlayerGui::CDrawQuestInfo::CQuestInfoElementCounterList, proven by: CDrawQuestInfo::AddCounterList 0x0064FA77 releases its new element here (binds ~CCountedPointer<CQuestInfoElementCounterList>) after the converting copy 0x00654C05 (bound <CQuestInfoElementCounterList>); jmp thunk 0x00655376; FableWin.pdb ReleaseData name.
// Member: ReleaseData, not the destructor. Retail's ~CCountedPointer is `{ ReleaseData(); }` (Ego_r.pdb 5-byte ??1<NUISystem::CComponent>
//   = retail jmp 0x00429C53 -> ReleaseData 0x004291DE); VC7.1 expands that destructor at the call site, so a size-TU caller and
//   every ShareData / Reset call ReleaseData itself (probe: /O1 scalar deleting dtor and scope end call ReleaseData, /O2 ones call ??1).
// Model: engine/CCountedPointer.h opt-in FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (destructor = ReleaseData(); ReleaseData zeroes Data first).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES
#define FABLE_RETAIL_CDRAWQUESTINFO_ELEMENTS
#define FABLE_RETAIL_CDRAWQUESTINFO_COUNTER
#include "engine/NPlayerGui_CDrawQuestInfo.h"
template void CCountedPointer<NPlayerGui::CDrawQuestInfo::CQuestInfoElementCounterList>::ReleaseData();
