// CCountedPointer::CCountedPointer(const CCountedPointer<U>&) -- converting copy: take Data and Info, then add a reference (T = NPlayerGui::CDrawQuestInfo::CQuestInfoElement, U = NPlayerGui::CDrawQuestInfo::CQuestInfoElementBar)
// Leaf: ??$?0VCQuestInfoElementBar@CDrawQuestInfo@NPlayerGui@@@?$CCountedPointer@VCQuestInfoElement@CDrawQuestInfo@NPlayerGui@@@@QAE@ABV?$CCountedPointer@VCQuestInfoElementBar@CDrawQuestInfo@NPlayerGui@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00654B36, 25 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Was landed as _global::CCountedPointer_CopyCtor_00654b36 (stand-in name).
// T = NPlayerGui::CDrawQuestInfo::CQuestInfoElement, proven by: AddBar 0x0064F7FC / AddBarHealth 0x0064F885 pass their CCountedPointer<CQuestInfoElementBar> to AddElement(CCountedPointer<CQuestInfoElement>) through it (0x0064F885 binds this converting ctor); it follows ReleaseData<CQuestInfoElementBar> 0x00654B0A; FableWin.pdb has this exact member-template symbol.
// Member: the in-class copy constructor of engine/CCountedPointer.h (member template), emitted out of line as its own COMDAT.
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES
#define FABLE_RETAIL_CDRAWQUESTINFO_ELEMENTS
#define FABLE_RETAIL_CDRAWQUESTINFO_COUNTER
#include "engine/NPlayerGui_CDrawQuestInfo.h"
template CCountedPointer<NPlayerGui::CDrawQuestInfo::CQuestInfoElement>::CCountedPointer(const CCountedPointer<NPlayerGui::CDrawQuestInfo::CQuestInfoElementBar>&);
