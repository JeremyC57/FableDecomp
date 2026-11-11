// CCountedPointer::CCountedPointer(const CCountedPointer<U>&) -- converting copy: take Data and Info, then add a reference (T = NPlayerGui::CDrawQuestInfo::CQuestInfoElement, U = NPlayerGui::CDrawQuestInfo::CQuestInfoElementCounter)
// Leaf: ??$?0VCQuestInfoElementCounter@CDrawQuestInfo@NPlayerGui@@@?$CCountedPointer@VCQuestInfoElement@CDrawQuestInfo@NPlayerGui@@@@QAE@ABV?$CCountedPointer@VCQuestInfoElementCounter@CDrawQuestInfo@NPlayerGui@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00654BC0, 25 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Was landed as _global::CCountedPointer_CopyCtor_00654bc0 (stand-in name).
// T = NPlayerGui::CDrawQuestInfo::CQuestInfoElement, proven by: AddCounter 0x0064FA0F and AddMiniGameBar 0x0064FC23 bind this converting ctor for a CCountedPointer<CQuestInfoElementCounter>; it follows ReleaseData<CQuestInfoElementCounter> 0x00654B94; FableWin.pdb has this exact member-template symbol.
// Member: the in-class copy constructor of engine/CCountedPointer.h (member template), emitted out of line as its own COMDAT.
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES
#define FABLE_RETAIL_CDRAWQUESTINFO_ELEMENTS
#define FABLE_RETAIL_CDRAWQUESTINFO_COUNTER
#include "engine/NPlayerGui_CDrawQuestInfo.h"
template CCountedPointer<NPlayerGui::CDrawQuestInfo::CQuestInfoElement>::CCountedPointer(const CCountedPointer<NPlayerGui::CDrawQuestInfo::CQuestInfoElementCounter>&);
