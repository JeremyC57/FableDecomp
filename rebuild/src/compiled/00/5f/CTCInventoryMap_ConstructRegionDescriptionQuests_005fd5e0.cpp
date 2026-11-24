// 005fd5e0 CTCInventoryMap::ConstructRegionDescriptionQuests
// Leaf: ?ConstructRegionDescriptionQuests@CTCInventoryMap@@IAEXPAVCComponent@NUISystem@@AAVCRegion@@_N@Z
// Flags: none
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYMAP_REGIONDESCRIPTION_ALL
#define FABLE_RETAIL_CREGION
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ORIGINAL_SETPTR_PASS22
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CTCQUESTCARD
#define FABLE_RETAIL_CTCQUESTCARD_ACCESSORS
#define FABLE_RETAIL_CTCQUESTCARD_STATGETINTERFACETYPE
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_INSERT_DISPATCH_NOINLINE
#include "fable_string.h"
#include "engine/CRegion.h"
#include "engine/CThing.h"
#include "engine/CQuestCardDef.h"
#include "engine/CTCQuestCard.h"
#include "engine/CQuestManager.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CText.h"
#include "engine/CTCInventoryMap.h"

// The component pointer's release at 004291DE is size-optimized. Its own
// destructor at 00429C53 delegates to the release, as in FableWin.
#ifdef _MSC_VER
#pragma optimize("s", on)
#endif
template<> inline void CCountedPointer<NUISystem::CComponent>::ReleaseData()
{
    if (Info && --Info->RefCount == 0) {
        Info->DeleteFunc(Info->Data);
        delete Info;
    }
    Data = 0;
    Info = 0;
}
template<> inline CCountedPointer<NUISystem::CComponent>::~CCountedPointer()
{
    ReleaseData();
}
#ifdef _MSC_VER
#pragma optimize("", on)
#endif

void CTCInventoryMap::ConstructRegionDescriptionQuests(NUISystem::CComponent* pcontainer,CRegion& region,bool pc_style)
{
    NUISystem::CManager* pmanager=NUISystem::CManager::GetInstance();
    CCountedPointer<NUISystem::CComponent> pcomposite;
    std::list<CIntelligentPointer<CThing> > quests(CQuestManager::Get().GetActiveQuestCards());
    for(std::list<CIntelligentPointer<CThing> >::const_iterator it=quests.begin();it!=quests.end();++it) {
        CTCQuestCard* ptc_card;
        (*it)->GetTC(&ptc_card);
        if(ptc_card->GetRegionName()==region.GetName()) {
            if(ptc_card->IsCore()) {
                if(pc_style) pcomposite.Reset(pmanager->CreateComponent("PC_CORE_ICON_AND_NAME",false));
                else pcomposite.Reset(pmanager->CreateComponent("UI_CORE_ICON_AND_NAME",false));
            } else if(ptc_card->GetPDef()->IsVignette) {
                if(pc_style) pcomposite.Reset(pmanager->CreateComponent("PC_VIGNETTE_ICON_AND_NAME",false));
                else pcomposite.Reset(pmanager->CreateComponent("UI_VIGNETTE_ICON_AND_NAME",false));
            } else if(ptc_card->IsOptional()) {
                if(pc_style) pcomposite.Reset(pmanager->CreateComponent("PC_OPTIONAL_ICON_AND_NAME",false));
                else pcomposite.Reset(pmanager->CreateComponent("UI_OPTIONAL_ICON_AND_NAME",false));
            } else {
                if(pc_style) pcomposite.Reset(pmanager->CreateComponent("PC_CORE_ICON_AND_NAME",false));
                else pcomposite.Reset(pmanager->CreateComponent("UI_CORE_ICON_AND_NAME",false));
            }
            NUISystem::CComponent* ptext;
            if(pc_style) ptext=pcomposite->FindChild("PC_QUEST_NAME");
            else ptext=pcomposite->FindChild("UI_QUEST_NAME");
            if(ptext) {
                NUISystem::CText* ptext_real=static_cast<NUISystem::CText*>(ptext);
                ptext_real->SetValue(ptc_card->GetQuestName(),true,true);
            }
            pcontainer->AddChild(pcomposite);
        }
    }
}