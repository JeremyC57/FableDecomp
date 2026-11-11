// CCountedPointer::ReleaseData() -- drop one reference (delete the pointee and the count block at zero), then zero Data and Info (T = NPlayerGui::CDrawQuestInfo::CQuestInfoElement)
// Leaf: ?ReleaseData@?$CCountedPointer@VCQuestInfoElement@CDrawQuestInfo@NPlayerGui@@@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0043E1C0, 53 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Was landed as _global::CCountedPointer_Release_0043e1c0 (stand-in name).
// T = NPlayerGui::CDrawQuestInfo::CQuestInfoElement, proven by: CDrawQuestInfo::AddElement 0x0064A3ED and list<CCountedPointer<CQuestInfoElement>>::erase 0x00655EEA (both size TUs: the destructor expands to a ReleaseData call) release the element pointer here (Elements is std::list<CCountedPointer<CQuestInfoElement>>, engine header + Ego_r _Node); the list erase binds CCountedPointer<NPlayerGui::CDrawQuestInfo::CQuestInfoElement>; FableWin.pdb ReleaseData name.
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
template void CCountedPointer<NPlayerGui::CDrawQuestInfo::CQuestInfoElement>::ReleaseData();
