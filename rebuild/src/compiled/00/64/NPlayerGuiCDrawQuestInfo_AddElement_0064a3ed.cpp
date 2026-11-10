// NPlayerGui::CDrawQuestInfo::AddElement -- append a quest text element with its layout coordinates.
// Retail: 0x0064A3ED, 99 bytes; thiscall.
// Grade: VERIFIED_MATCH; see ROW.json and PROBES.md for verification or remaining residue.
// Leaf: ?AddElement@CDrawQuestInfo@NPlayerGui@@AAEXV?$CCountedPointer@VCQuestInfoElement@CDrawQuestInfo@NPlayerGui@@@@@Z
// Flags: none
#pragma optimize("s",on)
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CQUESTMANAGER_ACTIVEQUEST
#define FABLE_RETAIL_CDRAWQUESTINFO_ADDELEMENT
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES
#include "engine/NPlayerGui_CDrawQuestInfo.h"
#include "engine/CQuestManager.h"
namespace NPlayerGui {
void CDrawQuestInfo::AddElement(CCountedPointer<CQuestInfoElement> pelement)
{
    CQuestManager::CActiveQuest* pactive_quest=CQuestManager::Get().GetPActiveQuest();
    if(pactive_quest)pelement->ScriptID=pactive_quest->GetID();
    long highest_id=0;
    for(std::list<CCountedPointer<CQuestInfoElement> >::iterator it=Elements.begin();it!=Elements.end();++it){
        CQuestInfoElement* ploop_element=it->Get();
        if(ploop_element->ID>highest_id)highest_id=ploop_element->ID;
    }
    pelement->ID=highest_id+1;
    pelement->Scale*=0.7f;
    Elements.push_back(pelement);
}
}