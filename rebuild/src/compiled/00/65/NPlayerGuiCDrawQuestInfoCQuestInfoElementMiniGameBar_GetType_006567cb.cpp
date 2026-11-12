// NPlayerGui::CDrawQuestInfo::CQuestInfoElementMiniGameBar::GetType() const -- the element type of the quest-info minigame-bar element, QUEST_INFO_ELEMENT_MINIGAME_BAR
// Leaf: ?GetType@CQuestInfoElementMiniGameBar@CDrawQuestInfo@NPlayerGui@@UBE?AW4EElementType@23@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006567CB, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 3 of NPlayerGui::CDrawQuestInfo::CQuestInfoElementMiniGameBar's vtable 0x0125A46C (the slot holds this body in 1 vtable(s), owner NPlayerGui::CDrawQuestInfo::CQuestInfoElementMiniGameBar); the Xbox 360 Test PDB virtual order (third source) aligned on 1 TLC-named slots of that vtable puts GetType(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetType@CQuestInfoElementMiniGameBar@CDrawQuestInfo@NPlayerGui@@UBE?AW4EElementType@23@XZ; `ret` matches the signature.
// Was: NPlayerGui::CDrawQuestInfo::CQuestInfoElementMiniGameBar::vfunc_3 (rebuild/src/compiled/00/65/NPlayerGuiCDrawQuestInfoCQuestInfoElementMiniGameBar_vfunc3_006567cb.cpp).
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
CDrawQuestInfo::EElementType CDrawQuestInfo::CQuestInfoElementMiniGameBar::GetType() const
{
    return QUEST_INFO_ELEMENT_MINIGAME_BAR;
}
}
