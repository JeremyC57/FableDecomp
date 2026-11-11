// CCountedPointer::CCountedPointer(const CCountedPointer<U>&) -- converting copy: take Data and Info, then add a reference (T = NPlayerGui::CDrawQuestInfo::CQuestInfoElement, U = NPlayerGui::CDrawQuestInfo::CQuestInfoElementMiniGameBar)
// Leaf: ??$?0VCQuestInfoElementMiniGameBar@CDrawQuestInfo@NPlayerGui@@@?$CCountedPointer@VCQuestInfoElement@CDrawQuestInfo@NPlayerGui@@@@QAE@ABV?$CCountedPointer@VCQuestInfoElementMiniGameBar@CDrawQuestInfo@NPlayerGui@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00654C8F, 25 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Was landed as _global::CCountedPointer_CopyCtor_00654c8f (stand-in name).
// T = NPlayerGui::CDrawQuestInfo::CQuestInfoElement, proven by: AddMiniGameBar 0x0064FC23 binds this converting ctor for its CCountedPointer<CQuestInfoElementMiniGameBar>; it follows ReleaseData<CQuestInfoElementMiniGameBar> 0x00654C63; FableWin.pdb has this exact member-template symbol.
// Member: the in-class copy constructor of engine/CCountedPointer.h (member template), emitted out of line as its own COMDAT.
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES
#define FABLE_RETAIL_CDRAWQUESTINFO_ELEMENTS
#define FABLE_RETAIL_CDRAWQUESTINFO_COUNTER
#include "engine/NPlayerGui_CDrawQuestInfo.h"
template CCountedPointer<NPlayerGui::CDrawQuestInfo::CQuestInfoElement>::CCountedPointer(const CCountedPointer<NPlayerGui::CDrawQuestInfo::CQuestInfoElementMiniGameBar>&);
