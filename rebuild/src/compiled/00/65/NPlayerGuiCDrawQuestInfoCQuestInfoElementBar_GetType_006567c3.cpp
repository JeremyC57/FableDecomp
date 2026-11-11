// NPlayerGui::CDrawQuestInfo::CQuestInfoElementBar::GetType() const -- the element type of the quest-info bar element, QUEST_INFO_ELEMENT_BAR
// Leaf: ?GetType@CQuestInfoElementBar@CDrawQuestInfo@NPlayerGui@@UBE?AW4EElementType@23@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006567C3, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 3 of NPlayerGui::CDrawQuestInfo::CQuestInfoElementBar's vtable 0x0125A444 (the slot holds this body in 1 vtable(s), owner NPlayerGui::CDrawQuestInfo::CQuestInfoElementBar); the Xbox 360 Test PDB virtual order (third source) aligned on 1 TLC-named slots of that vtable puts GetType(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetType@CQuestInfoElementBar@CDrawQuestInfo@NPlayerGui@@UBE?AW4EElementType@23@XZ; `ret` matches the signature.
// Was: NPlayerGui::CDrawQuestInfo::CQuestInfoElementBar::vfunc_3 (rebuild/src/compiled/00/65/NPlayerGuiCDrawQuestInfoCQuestInfoElementBar_vfunc3_006567c3.cpp).
#pragma optimize("s", on)
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES
#define FABLE_RETAIL_CDRAWQUESTINFO_ELEMENTS
#define FABLE_RETAIL_CDRAWQUESTINFO_COUNTER
#include "engine/NPlayerGui_CDrawQuestInfo.h"
namespace NPlayerGui {
CDrawQuestInfo::EElementType CDrawQuestInfo::CQuestInfoElementBar::GetType() const
{
    return QUEST_INFO_ELEMENT_BAR;
}
}
