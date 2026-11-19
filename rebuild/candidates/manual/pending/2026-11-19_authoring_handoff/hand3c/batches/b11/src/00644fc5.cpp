// NPlayerGui::CDrawQuestInfo::GetElementByIndex(long index) const -- the quest-info element whose ID is index, or null
// Leaf: ?GetElementByIndex@CDrawQuestInfo@NPlayerGui@@QBEPAVCQuestInfoElement@12@J@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00644FC5, 32 bytes, no frame pointer, `ret 4`. No callees: a top-tested walk of Elements (+0x58)
//   comparing each element's ID (+4).
// Re-port (hand3c b11, coordinator-approved): the landed row was a local model with the non-const ?GetElementByIndex@..QAE.. spelling;
//   Ego_r.pdb / FableWin.pdb give the const QBE member. Body: FableWin.pdb 0x020EC8C4 (Ego_d locals it / pelement; its ASSERT(pelement)
//   compiled out), the same text row 0x0064D404 DrawMiniGameInfo defines earlier in its TU (retail source order). An ordinary member
//   definition: no scaffolding needed.
// Pragmas: `#pragma optimize("s", on)` only (the HUD TU's mode; `optimize("y", off)` adds a frame pointer: DIFFER(35v32)).
// Headers: NPlayerGui_CDrawQuestInfo.h + FABLE_RETAIL_CDRAWQUESTINFO_DRAWMINIGAMEINFO (the GetElementByIndex declaration).
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CDRAWQUESTINFO_DRAWMINIGAMEINFO
#include "engine/NPlayerGui_CDrawQuestInfo.h"

namespace NPlayerGui {
CDrawQuestInfo::CQuestInfoElement* CDrawQuestInfo::GetElementByIndex(long index) const
{
    for (std::list<CCountedPointer<CQuestInfoElement> >::const_iterator it = Elements.begin(); it != Elements.end(); ++it)
    {
        CQuestInfoElement* pelement = (*it).Get();
        if (pelement->ID == index)
            return pelement;
    }
    return 0;
}
}
