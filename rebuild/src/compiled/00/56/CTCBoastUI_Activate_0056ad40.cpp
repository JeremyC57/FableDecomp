// CTCBoastUI::Activate() -- open the boast menu: close the quick-access menu, rebuild one branch per active quest that offers boasts (title, description with the quest, boast, wager and reward, and a "take boast" entry), push the mouse pointer and the list as UI components, drop the hero's block / fireball charge and put the player into boast mode
// Leaf: ?Activate@CTCBoastUI@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0056AD40, 4,284 bytes (early `ret` 0x0056AE99 when no active quest has boasts; final `ret`
//   0x0056BDFB). Frame `push ebp ; mov ebp,esp ; and esp,-8 ; sub esp,208h`.
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
// Inline split by plain C++ (no inline device, no flag): c2 expands an inline callee at a site iff c1 * (1/f + 6) / 7 < 74, f the
//   site's static frequency (c2.dll 0x1078D1AA). The first quest loop is a `while` (its body 9x, the code after it back at 1x); the
//   second quest loop and the boast loop are `for` (body 5x, the code after it stays at the body's frequency). Two condition spellings
//   set the remaining branch splits; both fold away and change no instruction (see the comments at the two tests).
// Registers and frame: the boast loop reads the record through a `const CBoast&` (edi = node + 8 at 0x0056B190, the iterator lives
//   in memory) and through the FableWin.pdb inline accessors GetTitle / GetText / GetCost / GetReward / GetID / GetScriptName /
//   GetGroupIndex: an inline accessor's value is evaluated where the call is, so Cost / Reward are loaded into ebx before the
//   CWideString(L", ") / CWideString(L"\n") arguments (0x0056B4C0 / 0x0056B637), which sets retail's colouring and its 0x208 frame.
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
    // A `while` loop: c2's static frequency after it drops back to the loop entry's (after a `for` it would stay at the body's
    // 5x and every later site would expand; retail calls them, e.g. the ~CCountedPointer COMDATs 0x00429B72 / 0x00429C53).
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
        // `&& true` folds away (same `test eax,eax ; jne` + own ~CCharString copy as retail, 0x0056AF60..0x0056AF6D) but keeps
        // the static split of an `&&`: c2 sends 0.25 down this continue and 0.75 past it, the extra fall-through weight that,
        // with the test below, puts the later sites at retail's call / expand split (work/s6/fmodel.py: one 0.75 fall-through
        // upstream is needed; `if (Active && true)`, the HasScript or the questsWithBoasts test give identical bytes).
        if (questCardThing == 0 && true)
            continue;
        CTCQuestCard* questCard;
        questCardThing->GetTC(&questCard);
        CWideString questName = questCard->GetQuestName();
        std::list<CQuestManager::CBoast> boasts;
        CQuestManager::Get().GetBoastsForScript(scriptName, boasts);
        // The leading `false ||` folds away (retail 0x0056AFF2 `cmp [eax],eax ; jne`, the continue path laid out in line) but
        // keeps the static split of an `||`: 0.75 down this continue, so its ~list<CBoast> expands (clear 0x004B8C00 + the
        // sentinel free, 0x0056AFF6..0x0056B00D) while list::size and the two map constructors after the test stay calls
        // (0x0056B157 / 0x0056B16C / 0x0056B178). A plain test splits 0.5 / 0.5 (252 calls v 238); a trailing `|| false` gives
        // the same split but c2 then moves this continue block next to ++quest and the card-check continue jumps into it.
        if (false || boasts.empty())
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
        for (std::list<CQuestManager::CBoast>::const_iterator it = boasts.begin(); it != boasts.end(); ++it) {
            const CQuestManager::CBoast& boast = *it;   // edi = &*it (0x0056B190 `mov edi,[esp+1Ch] ; add edi,8`)
            CWideString title = boast.GetTitle();
            CWideString description = boast.GetText();
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
            value += text.GetTextBySymbol("TEXT_GUI_MENU_WAGER") + CWideString(L": ") + GFIntToWideString(boast.GetCost())
                + CWideString(L" ") + text.GetTextBySymbol("TEXT_GUI_MENU_GOLD") + CWideString(L", ");
            value += text.GetTextBySymbol("TEXT_GUI_MENU_REWARD") + CWideString(L": ") + GFIntToWideString(boast.GetReward())
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
            takeAction->AddParam(new NUISystem::CActionParamSLONG(boast.GetID()));
            CWideString boastScriptName = boast.GetScriptName().ToWideString();
            takeAction->AddParam(new NUISystem::CActionParamString(boastScriptName));
            take->AddAction(takeAction);
            if (boasts.size() > 1) {
                CCountedPointer<NUISystem::CAction> deleteAction(new NUISystem::CAction);
                deleteAction->ActionType = NUISystem::UI_ACTION_TYPE_DELETE_PREVIOUS;
                take->AddAction(deleteAction);
                NUISystem::CMenuEntry* menuEntry = static_cast<NUISystem::CMenuEntry*>(entry.Data);
                menuEntry->AddUIActionParam(menuEntry);
                long group = boast.GetGroupIndex();
                if (group != 0) {
                    BoastGroups.insert(std::make_pair(menuEntry, boast));
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