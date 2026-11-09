// CTCBoastUI::Activate() -- open the boast menu: close the quick-access menu, rebuild one branch per active quest that offers boasts (title, description with the quest, boast, wager and reward, and a "take boast" entry), push the mouse pointer and the list as UI components, drop the hero's block / fireball charge and put the player into boast mode
// Inline-budget: #pragma inline_depth(0) ... #pragma inline_depth() at 8 sites (first list::size, the two map ctors, SetStaticCast, the two make_pair inserts, the new-group insert, the three scope exits of the CCountedPointer locals; each site names its retail call) -- retail calls these COMDATs where VC7.1 at the default budget expands them; the per-row flags /d2inlT45 (bitset test leaves too, map ctors stay) and /d2inlS5 (no change) do not reproduce the split, and per-site scores (c2 trace work/inl/r2_v2.json) admit no single threshold.
// Grade: DIFFER(4299v4284) (probe r2_s4h = this file's bytes) -- strict 729/1110 = 65.7% / loose 93.3% (fit); calls aligned 226 / 238 (was 213). Left: the find / push_back / ~CBoast-list / map-dtor sites of the group branch and the outer exit, frame and register drift.
// Next: the remaining call-site splits (retail 0x0056BB0E / 0x0056BB75 push_back 0x0056DCE0 called while ours expands it under depth 0 only with the find; 0x0056BC36 map dtor 0x0056D7B0; 0x0056BC6F list::clear 0x004B8C00 at the boasts exit), then the register web; NAME_FIX 0x0042A2DB (reloc_bindings binds the CList SetStaticCast spelling to 0x0042A2CA).
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
    for (std::list<CCountedPointer<CQuestManager::CActiveQuest> >::const_iterator quest = activeQuests.begin();
         quest != activeQuests.end(); ++quest) {
        if ((*quest)->HasScript()) {
            CCharString scriptName = (*quest)->GetScriptInfo().GetName();
            std::list<CQuestManager::CBoast> boasts;
            CQuestManager::Get().GetBoastsForScript(scriptName, boasts);
            if (!boasts.empty())
                ++questsWithBoasts;
        }
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
#pragma inline_depth(0)   // first boasts.size(): retail calls list::size 0x004B87D0 (0x0056B157)
        boastSubList->InitialiseOffsets(boasts.size());
#pragma inline_depth()
#pragma inline_depth(0)   // the two local maps: retail calls their no-argument ctors 0x0056E4A0 / 0x0056D770 (0x0056B16C / 0x0056B178)
        std::map<long, std::vector<NUISystem::CMenuEntry*> > groupEntries;
        std::map<NUISystem::CMenuEntry*, NUISystem::CComponent*> takeEntries;
#pragma inline_depth()
        for (std::list<CQuestManager::CBoast>::iterator boast = boasts.begin(); boast != boasts.end(); ++boast) {
            CWideString title = (*boast).Title;
            CWideString description = (*boast).Text;
            CCountedPointer<NUISystem::CComponent> entry(manager->CreateMenuEntryComponent(title, 0.0f, 1.0f,
                "PC_MENU_ENTRY_BRANCH_TEMPLATE", 0.0f, "", 15.0f, "PC_BUTTON_BASTARD"));
            boastSubList->AddScrollingChildAndSetOffsets(entry);
            CCountedPointer<NUISystem::CComponent> detailList(manager->CreateComponent("PC_BOAST_SUB_LIST", false));
            CCountedPointer<NUISystem::CList> detailListAsList;
#pragma inline_depth(0)   // retail calls CCountedPointer<CList>::SetStaticCast<CComponent> 0x0042A2DB (0x0056B281)
            detailListAsList.SetStaticCast(detailList);
#pragma inline_depth()
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
#pragma inline_depth(0)   // retail calls make_pair 0x0056CB40, the pair converter 0x0056CB60 and insert_unique (0x0056BA3A..0x0056BAAF)
                    BoastGroups.insert(std::make_pair(menuEntry, *boast));
                    takeEntries.insert(std::make_pair(menuEntry, takeComponent));
#pragma inline_depth()
                    std::map<long, std::vector<NUISystem::CMenuEntry*> >::iterator found = groupEntries.find(group);
                    if (found == groupEntries.end()) {
                        std::pair<long, std::vector<NUISystem::CMenuEntry*> > newGroup;
                        newGroup.first = group;
                        newGroup.second.push_back(menuEntry);
#pragma inline_depth(0)   // retail calls the pair<const long, vector> converter 0x0056DAB0 and insert_unique 0x0056E020 (0x0056BB22 / 0x0056BB3E)
                        groupEntries.insert(newGroup);
#pragma inline_depth()
                    } else {
                        (*found).second.push_back(menuEntry);
                    }
                }
#pragma inline_depth(0)   // deleteAction's scope exit: retail calls ~CCountedPointer<CAction> through ILT 0x00429B72 (0x0056BB81)
            }
#pragma inline_depth()
            detailListAsList->AddScrollingChild(take);
#pragma inline_depth(0)   // boast loop body exit: retail calls every ~CCountedPointer (ILT 0x00429B72 / 0x00429C53 / 0x00429D8E, 0x0056BBAD..0x0056BC04)
        }
#pragma inline_depth()
#pragma inline_depth(0)   // quest loop body exit: retail calls the map dtors (0x0056D7B0 at 0x0056BC36) and ~CCountedPointer (0x0056BC4E..0x0056BC66)
    }
#pragma inline_depth()

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
