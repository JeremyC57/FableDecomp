// CTCQuestCompletionUI::GetQuestDescription(NUISystem::CComponent* pcontainer) -- populate the quest description UI
// Grade: RELOCATION_MATCH -- actual COFF operands checked against their native targets.
// Flags: /d2inlT60 /d2inlS48
// Leaf: ?GetQuestDescription@CTCQuestCompletionUI@@AAEXPAVCComponent@NUISystem@@@Z
// Retail: 0x005E4BE0, 4939 bytes, thiscall.
// Identity: both original PDBs; donor022c6030 and native caller005e7123.
// Layout: original member types with retail STLport containers; shared opt-in headers.
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CTCQUESTCOMPLETIONUI_DESCRIPTION
#define FABLE_RETAIL_CTCQUESTCOMPLETIONUI_DRAW
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CWIDESTRING_NULLSTRING
#define FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#include "engine/ETCInterfaceType.h"
#include "engine/CTCQuestCompletionUI.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CText.h"
#include "engine/NUISystem_CSprite.h"
#include "engine/NUISystem_CUIState.h"
#include "engine/NUISystem_CUIMiscThingsDef.h"
#include "engine/CMainGameComponent.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CThingObjectDef.h"
#include "engine/CInventoryItemDef.h"
// Both original PDBs and donor0187b680: n,temp; no elision assumption.
inline CWideString FABLE_FASTCALL GFIntToWideString(long n)
{
    CWideString temp;
    temp.Sprintf(L"%d", n);
    return temp;
}
void CTCQuestCompletionUI::GetQuestDescription(NUISystem::CComponent* pcontainer)
{
    const NGameText::CDataBank& rTextBank = CMainGameComponent::Get()->GetTextBank();
    NUISystem::CManager* pmanager = NUISystem::CManager::GetInstance();
    CCountedPointer<NUISystem::CComponent> ptext;
    CWideString text;
    CDefPointer<const NUISystem::CUIMiscThingsDef> pmiscthingsdef;
    long misc_thing_def_index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("UI_MISC_THINGS_DEF", 1);
    CGameDefinitionManager::Get().GetDef(misc_thing_def_index, &pmiscthingsdef);
    if (Completion == 3) {
        ptext.Reset(pmanager->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE_QUEST_COMPLETION", false));
        pcontainer->AddChild(ptext);
        NUISystem::CText* ptext_real = (NUISystem::CText*)ptext.Get();
        ptext_real->SetValue("TEXT_GUI_MENU_DEATH_EXPLANATION", true, true);
    } else {
    if (Completion == 2 && FailureReason != L"") {
        ptext.Reset(pmanager->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE_QUEST_COMPLETION", false));
        pcontainer->AddChild(ptext);
        NUISystem::CText* ptext_real = (NUISystem::CText*)ptext.Get();
        ptext_real->SetValue(FailureReason, true, true);
    }
    if (Completion == 0) {
        CCountedPointer<NUISystem::CComponent> picon_couple;
        if (PCardDef.Get()) {
            ptext.Reset(pmanager->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE_QUEST_COMPLETION", false));
            NUISystem::CText* ptext_real = (NUISystem::CText*)ptext.Get();
            pcontainer->AddChild(ptext);
            if (QuestObjective != L"") ptext_real->SetValue(QuestObjective, true, true);
            else ptext_real->SetValue(QuestSummary, true, true);
            picon_couple.Reset(pmanager->CreateComponent("PC_GOLD_ICON_AND_NAME", false));
            ptext_real = (NUISystem::CText*)picon_couple->FindChild("PC_QUEST_NAME");
            text = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_GUILD_REWARD") + pmiscthingsdef->ColonSeparator + pmiscthingsdef->SpaceSeparator + GFIntToWideString(GoldReward) + pmiscthingsdef->SpaceSeparator + rTextBank.GetTextBySymbol("TEXT_GUI_GOLD");
            ptext_real->SetValue(text, true, true);
            pcontainer->AddChild(picon_couple);
            picon_couple.Reset(pmanager->CreateComponent("PC_RENOWN_ICON_AND_NAME", false));
            ptext_real = (NUISystem::CText*)picon_couple->FindChild("PC_QUEST_NAME");
            text = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_RENOWN_REWARD") + pmiscthingsdef->ColonSeparator + pmiscthingsdef->SpaceSeparator + GFIntToWideString(RenownReward);
            ptext_real->SetValue(text, true, true);
            pcontainer->AddChild(picon_couple);
        }
    }
    if (Completion == 1) {
        NUISystem::CText* ptext_real;
        if (SuccessSummary != CWideString::NullString) {
            ptext.Reset(pmanager->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE_QUEST_COMPLETION", false));
            NUISystem::CText* ptext_real = (NUISystem::CText*)ptext.Get();
            pcontainer->AddChild(ptext);
            ptext_real->SetValue(SuccessSummary, true, true);
        }
        CCountedPointer<NUISystem::CComponent> picon_couple;
        if (PCardDef.Get()) {
            for (std::vector<long>::const_iterator it = PCardDef->RewardObjects.begin(); it != PCardDef->RewardObjects.end(); ++it) {
                picon_couple.Reset(pmanager->CreateComponent("PC_CORE_ICON_AND_NAME", false));
                ptext_real = (NUISystem::CText*)picon_couple->FindChild("PC_QUEST_NAME");
                CDefPointer<const CThingObjectDef> pobject_def;
                CDefPointer<const CInventoryItemDef> pitem_def;
                CGameDefinitionManager::Get().GetDef(*it, &pobject_def);
                if (!pobject_def.IsNull()) {
                    pobject_def->GetSubDef(&pitem_def);
                    if (!pitem_def.IsNull()) {
                        NUISystem::CSprite* picon = (NUISystem::CSprite*)picon_couple->FindChild("UI_CORE_COUPLE");
                        if (picon && pitem_def->Graphic.Type == 1) picon->SetGraphicIndex(pitem_def->Graphic.BankIndex);
                        ptext_real->SetValue(pitem_def->ItemDescription, L"", true);
                        pcontainer->AddChild(picon_couple);
                    }
                }
            }
            picon_couple.Reset(pmanager->CreateComponent("PC_GOLD_ICON_AND_NAME", false));
            ptext_real = (NUISystem::CText*)picon_couple->FindChild("PC_QUEST_NAME");
            text = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_GUILD_REWARD") + pmiscthingsdef->ColonSeparator + pmiscthingsdef->SpaceSeparator + GFIntToWideString(GoldReward) + pmiscthingsdef->SpaceSeparator + rTextBank.GetTextBySymbol("TEXT_GUI_GOLD");
            ptext_real->SetValue(text, true, true);
            pcontainer->AddChild(picon_couple);
            picon_couple.Reset(pmanager->CreateComponent("PC_GOLD_ICON_AND_NAME", false));
            ptext_real = (NUISystem::CText*)picon_couple->FindChild("PC_QUEST_NAME");
            long extra_gold_to_display = TargetGoldGained - GoldReward;
            if (extra_gold_to_display < 0) extra_gold_to_display = 0;
            text = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_EXTRA_GOLD") + pmiscthingsdef->ColonSeparator + pmiscthingsdef->SpaceSeparator + GFIntToWideString(extra_gold_to_display) + pmiscthingsdef->SpaceSeparator + rTextBank.GetTextBySymbol("TEXT_GUI_GOLD");
            ptext_real->SetValue(text, true, true);
            pcontainer->AddChild(picon_couple);
            picon_couple.Reset(pmanager->CreateComponent("PC_RENOWN_ICON_AND_NAME", false));
            ptext_real = (NUISystem::CText*)picon_couple->FindChild("PC_QUEST_NAME");
            text = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_RENOWN_REWARD") + pmiscthingsdef->ColonSeparator + pmiscthingsdef->SpaceSeparator + GFIntToWideString(RenownReward);
            ptext_real->SetValue(text, true, true);
            pcontainer->AddChild(picon_couple);
            picon_couple.Reset(pmanager->CreateComponent("PC_RENOWN_ICON_AND_NAME", false));
            ptext_real = (NUISystem::CText*)picon_couple->FindChild("PC_QUEST_NAME");
            long extra_renown_to_display = TargetRenownGained - RenownReward;
            if (extra_renown_to_display < 0) extra_renown_to_display = 0;
            text = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_EXTRA_RENOWN") + pmiscthingsdef->ColonSeparator + pmiscthingsdef->SpaceSeparator + GFIntToWideString(extra_renown_to_display);
            ptext_real->SetValue(text, true, true);
            pcontainer->AddChild(picon_couple);
            picon_couple.Reset(pmanager->CreateComponent("PC_EXPERIENCE_ICON_AND_NAME", false));
            ptext_real = (NUISystem::CText*)picon_couple->FindChild("PC_QUEST_NAME");
            text = rTextBank.GetTextBySymbol("TEXT_GUI_STATS_LABEL_EXPER") + pmiscthingsdef->ColonSeparator + pmiscthingsdef->SpaceSeparator + GFIntToWideString(TargetExperienceGained);
            ptext_real->SetValue(text, true, true);
            pcontainer->AddChild(picon_couple);
        }
    }
    if (Completion == 1 || Completion == 0) {
        for (std::list<CQuestManager::CBoast>::iterator it = Boasts.begin(); it != Boasts.end(); ++it) {
            if (it->GetScriptName() == ScriptName) {
                ptext.Reset(pmanager->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE_QUEST_COMPLETION", false));
                pcontainer->AddChild(ptext);
                NUISystem::CText* ptext_real = (NUISystem::CText*)ptext.Get();
                text = it->GetText() + pmiscthingsdef->ColonSeparator + pmiscthingsdef->SpaceSeparator;
                if (Completion == 1) {
                    if (it->IsFailed()) {
                        text += rTextBank.GetTextBySymbol("TEXT_GUI_MENU_FAILED") + pmiscthingsdef->SpaceSeparator + pmiscthingsdef->OpenBracket + pmiscthingsdef->Minus + GFIntToWideString(it->GetCost()) + pmiscthingsdef->CloseBracket;
                        ptext_real->SetValue(text, true, true);
                        ptext_real->GetStates()[0].Colour.B *= 0.0f;
                        ptext_real->GetStates()[0].Colour.G *= 0.0f;
                    } else {
                        text += rTextBank.GetTextBySymbol("TEXT_GUI_MENU_SUCCEEDED") + pmiscthingsdef->SpaceSeparator + pmiscthingsdef->OpenBracket + pmiscthingsdef->Plus + GFIntToWideString(it->GetReward()) + pmiscthingsdef->CloseBracket;
                        ptext_real->SetValue(text, true, true);
                        ptext_real->GetStates()[0].Colour.B *= 0.0f;
                    }
                } else {
                    text += pmiscthingsdef->OpenBracket + pmiscthingsdef->Plus + GFIntToWideString(it->GetReward()) + pmiscthingsdef->CloseBracket;
                    ptext_real->SetValue(text, true, true);
                    ptext_real->GetStates()[0].Colour.B *= 0.0f;
                }
            }
        }
    }
    if (Completion == 1 && PCardDef.Get() && PCardDef->QuestEpilogue) {
        ptext.Reset(pmanager->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE_QUEST_COMPLETION", false));
        NUISystem::CText* ptext_real = (NUISystem::CText*)ptext.Get();
        pcontainer->AddChild(ptext);
        ptext_real->SetValue(PCardDef->QuestEpilogue, L"", true);
    }
    }
}