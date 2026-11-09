// CTCBoastUI::Activate() -- open the boast menu: close the quick-access menu, rebuild one branch per active quest that offers boasts (title, description with the quest, boast, wager and reward, and a "take boast" entry), push the mouse pointer and the list as UI components, drop the hero's block / fireball charge and put the player into boast mode
// Inline-budget: #pragma inline_depth(0) ... #pragma inline_depth() around BoastGroups.insert only -- retail calls make_pair 0x0056CB40 and the pair converter 0x0056CB60 there (0x0056BA3A / 0x0056BA4A); c2 scores them 64 / 68 (< 74) at that block's static frequency 0.049 (work/s5/want.py on work/inl/s5_l1.json). Every other call / expand split of retail is plain C++: the first quest loop is a while loop, which puts the second loop's body at retail's frequencies (TRACE Session 5, H1-H3).
// Grade: DIFFER(4275v4284) (probe r2_s5b = this file's bytes; session 6 fit unchanged) -- strict 773/1110 = 69.6% / loose 1061/1110 = 95.6%; prologue, entry colouring (this edi, zero ebp) and every call / expand split but the ~list at 0x0056AFFA match (238 v 238 calls); frame 0x200 v 0x208. Session 6 probe quantity: the empty-boasts ~list<CBoast> (0x0056AFFA, c1 19) expands only if that `continue` path is hotter than its fall-through (needs f > 0.047 there and <= 0.0439 on the main path) -- c2 gives that only to `if (a || b) continue;` with one 0.75 fall-through upstream (work/s6/fmodel.py, 114/114 solutions).
// Next: residue 3 -- with the `||` split (work/s6/d1.cpp: all 34 inline sites retail's, no device) the entry colouring flips, because the expanded continue dtor adds 3 loop-2 blocks where the activeQuests piece loses 6 and the zero piece 2 (c2prio pass 6: -7 v -4; retail needs activeQuests > zero, b0 -1 v -2). Find the source shape that gives the zero piece >= 4 more live-through cost (or activeQuests 4 more refs) there; then drop the inline_depth region. NAME_FIX 0x0042A2DB.
// Leaf: ?Activate@CTCBoastUI@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0056AD40, 4,284 bytes (early `ret` 0x0056AE99 when no active quest has boasts; final `ret`
//   0x0056BDFB). Frame `push ebp ; mov ebp,esp ; and esp,-8 ; sub esp,208h` (ours 0x210).
// Name: FableWin.pdb 0x0239BB40 / Ego_r.pdb 0x005E17E0 ?Activate@CTCBoastUI@@QAEXXZ; the landed caller GlobalUpdate 0x0056BE00 binds it
//   so (the catalog's "CQuestManager::UpdateUI_QuestMenu" is a forge-autoname label).
// Callees (Ego_r / FableWin spellings, hdr/README.md "Session 3"): CTCQuickAccessMenu::Deactivate 0x00625920 (NAME_FIX of the landed
//   RequestClose) + virtual GlobalUpdate(bool) slot 26; CQuestManager::GetBoastsForScript 0x004B19F0 / GetActiveQuestCardFromScriptName
//   0x004B0C80; CPlayerGui::Close(bool) 0x00438400 / ForceClearGui 0x004382C0 (NAME_FIX: landed as PrepareForLoad, which is 0x00438460);
//   CTCQuestCard::GetQuestName 0x00702310; NUISystem::CManager::GetInstance 0x0041E5F2 / CreateComponent 0x0041DB1D /
//   CreateMenuEntryComponent 0x00428494; CList::InitialiseOffsets (slot 156) / AddScrollingChildAndSetOffsets (152) / AddScrollingChild
//   (144); CMenuEntry::AddUIActionParam (slot 151, its own CActionParam base); CComponent AddChild / AddAction / AddActionOnBack /
//   Initialise / ChangeState; CText::SetValue(const CWideString&, bool, bool) (slot 144); CActionParamString(CWideString&) 0x0054DAE0;
//   CCharString::ToWideString 0x0099E480; GFIntToWideString 0x00405F00; NGameText::CDataBank::GetTextBySymbol 0x009C95E0;
//   CTCCombat::IsBlocking 0x007331E0 / UnsetBlocking 0x007338A0; CTCFireballSpell::CancelMove 0x007C8FD0; CTCBase::GetPlayerManager
//   0x00686D40; CPlayerManager::GetMainPlayer 0x00449970; CPlayer::GetPControlledCreature 0x00487DC0 / AddMode 0x006345C0.
// Shape facts (work/r2.ann): the ': ' / '\n' / ' ' / ', ' operands are explicit CWideString temporaries (`call 0x0099B6B0 ; push eax`);
//   the take-boast label is a named local (fetched before the CreateMenuEntryComponent arguments); the new group pair is
//   default-built then `first = group` (no vector temporary); the action types are UI_ACTION_TYPE_CLOSE_BOAST_MENU / _TAKE_BOAST /
//   _DELETE_PREVIOUS; both GetTC sites test the UsingComponents word in place (no FABLE_RETAIL_STLP_BITSET_TEST_DECL).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBOASTUI
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_BOASTUI
#define FABLE_RETAIL_CSCRIPTINFO_GETNAME
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CACTIONPARAMSLONG
#define FABLE_RETAIL_CACTIONPARAMSTRING_CTOR
#define FABLE_RETAIL_CMENUENTRY_ACTIONPARAMS
#define FABLE_RETAIL_CTCQUICKACCESSMENU_DEACTIVATE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCQUESTCARD
#define FABLE_RETAIL_CTCQUESTCARD_QUESTNAME
#define FABLE_RETAIL_CTCCOMBAT
#define FABLE_RETAIL_CCOMBATABILITY
#define FABLE_RETAIL_CCOMBATABILITYDATA
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCCOMBAT_BLOCKING
#define FABLE_RETAIL_CTCFIREBALLSPELL
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_CLOSE
#define FABLE_RETAIL_CSUBTITLERENDERER
#define FABLE_RETAIL_CSUBTITLERENDERER_INCUTSCENE
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_SCOPED_PTR
#include "stlport/fable_containers.h"
#include "engine/ETCInterfaceType.h"
#include "engine/EPlayerMode.h"
#include "engine/CThing.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCBoastUI.h"
#include "engine/CQuestManager.h"
#include "engine/CTCQuickAccessMenu.h"
#include "engine/CTCQuestCard.h"
#include "engine/CTCCombat.h"
#include "engine/CTCFireballSpell.h"
#include "engine/CPlayerGui.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CMainGameComponent.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CMenuEntry.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CText.h"
#include "engine/NUISystem_CActionParamSLONG.h"

CWideString FABLE_FASTCALL GFIntToWideString(long value);             // 0x00405F00 ?GFIntToWideString@@YI?AVCWideString@@J@Z

void CTCBoastUI::Activate()
{
    if (CPlayerGui::Get().GetSubtitleRenderer().IsDisplayingText() || CPlayerGui::Get().GetSubtitleRenderer().IsInCutscene())
        return;
    CTCQuickAccessMenu* quickAccess = 0;
    ParentThing->GetTC(&quickAccess);
    quickAccess->Deactivate();
    quickAccess->GlobalUpdate(true);
    BoastGroups.clear();
    if (Active)
        return;

    const std::list<CCountedPointer<CQuestManager::CActiveQuest> >& activeQuests = CQuestManager::Get().GetActiveQuests();
    unsigned long questsWithBoasts = 0;
    std::list<CCountedPointer<CQuestManager::CActiveQuest> >::const_iterator quest = activeQuests.begin();
    while (quest != activeQuests.end()) {
        if ((*quest)->HasScript()) {
            CCharString scriptName = (*quest)->GetScriptInfo().GetName();
            std::list<CQuestManager::CBoast> boasts;
            CQuestManager::Get().GetBoastsForScript(scriptName, boasts);
            if (!boasts.empty())
                ++questsWithBoasts;
        }
        ++quest;
    }
    if (questsWithBoasts == 0) {
        ActivationRequested = false;
        return;
    }

    CPlayerGui::Get().Close(true);
    CPlayerGui::Get().ForceClearGui();
    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    NUISystem::CComponent* mousePointer = manager->CreateComponent("UI_MOUSE_POINTER", false);
    NUISystem::CList* boastList = static_cast<NUISystem::CList*>(manager->CreateComponent("PC_BOAST_LIST", false));
    boastList->InitialiseOffsets(questsWithBoasts);

    for (std::list<CCountedPointer<CQuestManager::CActiveQuest> >::const_iterator quest = activeQuests.begin();
         quest != activeQuests.end(); ++quest) {
        if (!(*quest)->HasScript())
            continue;
        CCharString scriptName = (*quest)->GetScriptInfo().GetName();
        CThing* questCardThing = CQuestManager::Get().GetActiveQuestCardFromScriptName(scriptName);
        if (questCardThing == 0)
            continue;
        CTCQuestCard* questCard;
        questCardThing->GetTC(&questCard);
        CWideString questName = questCard->GetQuestName();
        std::list<CQuestManager::CBoast> boasts;
        CQuestManager::Get().GetBoastsForScript(scriptName, boasts);
        if (boasts.empty())
            continue;

        CCountedPointer<NUISystem::CComponent> branch(manager->CreateMenuEntryComponent(questName, 0.0f, 1.0f,
            "PC_MENU_ENTRY_BRANCH_TEMPLATE", 0.0f, "", 15.0f, "PC_BUTTON_BASTARD"));
        boastList->AddScrollingChildAndSetOffsets(branch);
        CCountedPointer<NUISystem::CAction> closeAction(new NUISystem::CAction);
        closeAction->ActionType = NUISystem::UI_ACTION_TYPE_CLOSE_BOAST_MENU;
        branch->AddActionOnBack(closeAction);
        CCountedPointer<NUISystem::CComponent> subList(manager->CreateComponent("PC_BOAST_SUB_LIST", false));
        NUISystem::CList* boastSubList = static_cast<NUISystem::CList*>(subList.Data);
        branch->AddChild(subList);
        boastSubList->InitialiseOffsets(boasts.size());
        std::map<long, std::vector<NUISystem::CMenuEntry*> > groupEntries;
        std::map<NUISystem::CMenuEntry*, NUISystem::CComponent*> takeEntries;
        for (std::list<CQuestManager::CBoast>::iterator boast = boasts.begin(); boast != boasts.end(); ++boast) {
            CWideString title = (*boast).Title;
            CWideString description = (*boast).Text;
            CCountedPointer<NUISystem::CComponent> entry(manager->CreateMenuEntryComponent(title, 0.0f, 1.0f,
                "PC_MENU_ENTRY_BRANCH_TEMPLATE", 0.0f, "", 15.0f, "PC_BUTTON_BASTARD"));
            boastSubList->AddScrollingChildAndSetOffsets(entry);
            CCountedPointer<NUISystem::CComponent> detailList(manager->CreateComponent("PC_BOAST_SUB_LIST", false));
            CCountedPointer<NUISystem::CList> detailListAsList;
            detailListAsList.SetStaticCast(detailList);
            entry->AddChild(detailList);
            CCountedPointer<NUISystem::CComponent> backdrop(manager->CreateComponent("PC_BOAST_BACKDROP", false));
            entry->AddChild(backdrop);

            const NGameText::CDataBank& text = CMainGameComponent::Get()->GetTextBank();
            CWideString value;
            value = text.GetTextBySymbol("TEXT_GUI_MENU_QUEST") + CWideString(L": ") + questName + CWideString(L"\n");
            value += text.GetTextBySymbol("TEXT_GUI_MENU_BOAST") + CWideString(L": ") + description + CWideString(L"\n");
            value += text.GetTextBySymbol("TEXT_GUI_MENU_WAGER") + CWideString(L": ") + GFIntToWideString((*boast).Cost)
                + CWideString(L" ") + text.GetTextBySymbol("TEXT_GUI_MENU_GOLD") + CWideString(L", ");
            value += text.GetTextBySymbol("TEXT_GUI_MENU_REWARD") + CWideString(L": ") + GFIntToWideString((*boast).Reward)
                + CWideString(L" ") + text.GetTextBySymbol("TEXT_GUI_MENU_GOLD") + CWideString(L"\n");
            CCountedPointer<NUISystem::CComponent> descriptionText(manager->CreateComponent("PC_BOAST_DESCRIPTION_TEXT", false));
            static_cast<NUISystem::CText*>(descriptionText.Data)->SetValue(value, true, true);
            entry->AddChild(descriptionText);

            CWideString takeLabel = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_TAKE_BOAST");
            CCountedPointer<NUISystem::CComponent> take(manager->CreateMenuEntryComponent(takeLabel, 0.0f, 1.0f,
                "PC_MENU_ENTRY_TEMPLATE", 0.0f, "", 15.0f, "PC_BUTTON_BASTARD"));
            NUISystem::CComponent* takeComponent = take.Data;
            CCountedPointer<NUISystem::CAction> takeAction(new NUISystem::CAction);
            takeAction->ActionType = NUISystem::UI_ACTION_TYPE_TAKE_BOAST;
            takeAction->AddParam(new NUISystem::CActionParamSLONG((*boast).ID));
            CWideString boastScriptName = (*boast).ScriptName.ToWideString();
            takeAction->AddParam(new NUISystem::CActionParamString(boastScriptName));
            take->AddAction(takeAction);
            if (boasts.size() > 1) {
                CCountedPointer<NUISystem::CAction> deleteAction(new NUISystem::CAction);
                deleteAction->ActionType = NUISystem::UI_ACTION_TYPE_DELETE_PREVIOUS;
                take->AddAction(deleteAction);
                NUISystem::CMenuEntry* menuEntry = static_cast<NUISystem::CMenuEntry*>(entry.Data);
                menuEntry->AddUIActionParam(menuEntry);
                long group = (*boast).GroupIndex;
                if (group != 0) {
#pragma inline_depth(0)   // retail calls make_pair 0x0056CB40 and the pair converter 0x0056CB60 (0x0056BA3A / 0x0056BA4A)
                BoastGroups.insert(std::make_pair(menuEntry, *boast));
#pragma inline_depth()
                    takeEntries.insert(std::make_pair(menuEntry, takeComponent));
                    std::map<long, std::vector<NUISystem::CMenuEntry*> >::iterator found = groupEntries.find(group);
                    if (found == groupEntries.end()) {
                        std::pair<long, std::vector<NUISystem::CMenuEntry*> > newGroup;
                        newGroup.first = group;
                        newGroup.second.push_back(menuEntry);
                        groupEntries.insert(newGroup);
                    } else {
                        (*found).second.push_back(menuEntry);
                    }
                }
            }
            detailListAsList->AddScrollingChild(take);
        }
    }

    boastList->Initialise();
    boastList->ChangeState(5);
    mousePointer->Initialise();
    Components.push_back(mousePointer);
    Components.push_back(boastList);
    Active = true;
    CThingPlayerCreature* hero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    if (hero) {
        CTCCombat* combat;
        hero->GetTC(&combat);
        if (combat->IsBlocking())
            combat->UnsetBlocking();
        CTCFireballSpell* fireball;
        if (hero->GetTC(&fireball))
            fireball->CancelMove();
    }
    GetPlayerManager().GetMainPlayer().AddMode(PLAYER_MODE_BOAST_UI, false);
}
