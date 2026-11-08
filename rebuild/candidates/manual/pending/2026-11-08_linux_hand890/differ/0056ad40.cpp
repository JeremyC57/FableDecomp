// CTCBoastUI::Activate() -- open the boast menu: close the quick-access menu, rebuild one branch per active quest that offers boasts (title, description with the quest, boast, wager and reward, and a "take boast" entry), push the mouse pointer and the list as UI components, drop the hero's block / fireball charge and put the player into boast mode
// Grade: NOT GRADED -- decoded draft only, 0 grades (session budget spent on row 0x00551EA0); not compiled. Residue unknown.
// Next: build the overlay opt-ins listed below, compile, then follow the hand880 UI-row method (calls.py --counts first: retail calls make_pair 0x0056CB40, the pair converters 0x0056CB60 / 0x0056DAB0, list size 0x004B87D0 and three tree insert_unique out of line; the list clear 0x004B8C00 out of line, the boast-count loop of the `> 1` test inline).
// Leaf: ?Activate@CTCBoastUI@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0056AD40, 4,284 bytes: an early `ret` at 0x0056AE99 inside the body (no boasts: clears
//   ActivationRequested and leaves), the final `ret` at 0x0056BDFB, int3 to 0x0056BDFF, next function 0x0056BE00 (CTCBoastUI::GlobalUpdate,
//   which calls this row as ?Activate@CTCBoastUI@@QAEXXZ). Frame: `push ebp ; mov ebp,esp ; and esp,-8 ; sub esp,208h` (8-aligned:
//   the row has a double-aligned local -- probably a pair<long, vector> or the CBoast copy).
// Name: the catalog's "CQuestManager::UpdateUI_QuestMenu" is a forge-autoname label. FableWin.pdb 0x0239BB40 / Ego_r.pdb 0x005E17E0
//   ?Activate@CTCBoastUI@@QAEXXZ (thiscall, no arguments, plain `ret`), and the landed caller 0x0056BE00 binds it so (reloc_bindings.tsv).
// Layout (Ego_r.pdb CTCBoastUI, retail STLport sizes, as the landed 0x0056AC10 / 0x0056A880 / 0x0056A980): CTCBase (vptr +0,
//   ParentThing +4, flags +8), BoastGroups map<NUISystem::CMenuEntry*, CQuestManager::CBoast> +0x0C (header +0x0C, count +0x10),
//   Active +0x18, Components vector<NUISystem::CComponent*> +0x1C, Quitting +0x28, AllowedToOpen +0x29, ActivationRequested +0x2A.
// Header work needed (none of it exists yet; each an additive FABLE_RETAIL_* opt-in in hdr/):
//   - engine/CTCBoastUI.h: a retail branch (class CTCBoastUI : public CTCBase with the members above and Activate()).
//   - engine/CQuestManager.h: ActiveQuests (list<CCountedPointer<CActiveQuest> > +0x38) typed, CActiveQuest's PScriptInfo (+4; the
//     script name CCharString at +8 of it) and PScript (+8, CCountedPointer<CScriptBase>, Data tested), GetBoastsForScript 0x004B19F0
//     (const CCharString&, list<CBoast>& out) and GetActiveQuestCardFromScriptName 0x004B0C80 (returns CThing*) -- PDB spellings to
//     verify (the reloc binding of 0x004B0C80 is a QuestManagerMethods stand-in).
//   - the UI list classes behind "PC_BOAST_LIST" / "PC_BOAST_SUB_LIST": slots 151 (0x25C), 152 (0x260), 156 (0x270) and the
//     CCountedPointer conversion-assignment 0x0042A2DB target with slot 144 (0x240) are not in any NUISystem header yet
//     (CComponent: Initialise 43, ChangeState(unsigned long) 48, AddChild 59, AddAction 66, AddActionOnBack 68; CText::SetValue 144).
//   - CTCQuickAccessMenu (TC 246): RequestClose 0x00625920 and virtual slot 26 (0x68) called with true; CTCQuestCard::GetQuestName
//     0x00702310 (TC 108); CTCCombat (TC 48) 0x007331E0 (bool byte +0x63 getter) / 0x007338A0; TC 157 0x007C8FD0 (labelled
//     CTCFireballSpell::CancelMove -- check the enum: ETCInterfaceType.h has 157 = TCI_FIREBALL_SPELL); CPlayer 0x006345C0 (AddMode, 0x20, 0).
// Retail shape notes (for the first draft):
//   - GetTC: TC 246 uses a null default (`xor esi,esi` before the bit test); TC 108 and TC 48 use the out-pointer form whose slot
//     is read even when the bit is clear ([esp+100h] / [esp+20h]); TC 157 is read unchecked after HasTC.
//   - The quest list is walked twice (count pass, then build pass), both `for (it = begin; it != end; ++it)` with the CActiveQuest
//     PScript test first; the build pass skips quests without a quest card or without boasts.
//   - Every CreateMenuEntryComponent call: (name, 0.0f, 1.0f, CCharString(template), 0.0f, CCharString(""), 15.0f,
//     CCharString("PC_BUTTON_BASTARD")); the by-value CCharStrings are built in the argument slots (`push ecx ; mov ecx,esp`).
//   - The CAction allocations are `new CAction` with the null test (`test eax,eax ; je`), the action type stored through the
//     counted pointer's Data after construction (0xBBA, 4, 0x26); the parameters are pushed with the inline list push_back
//     (malloc(0xC) nodes) of `new CActionParamSLONG(boast.ID)` (vtable 0x012366F4 stored inline, Type 3) and
//     `new CActionParamString(scriptName)` 0x0054DAE0.
//   - The description text is one CWideString built with operator= then three operator+= of operator+ chains (fastcall 0x0099BE70,
//     result temporaries destroyed in reverse after each statement).
#include "engine/CTCBoastUI.h"                 // needs FABLE_RETAIL_CTCBOASTUI (not written yet)
#include "engine/CQuestManager.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CAction.h"
#include "engine/NUISystem_CActionParamSLONG.h"
#include "engine/NUISystem_CText.h"
#include "engine/CPlayerGui.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/NGameText_CDataBank.h"

void CTCBoastUI::Activate()
{
    CPlayerGui* gui = CPlayerGui::PGlobalGui;                         // [0x013B8790]: bytes +0xFD / +0xF6 (busy flags) end the call
    if (gui->IsBusyFlagFD() || gui->IsBusyFlagF6())                   // descriptive: find the PDB names of the two bool members
        return;
    CTCQuickAccessMenu* quickAccess = 0;
    ParentThing->GetTC(&quickAccess);                                  // TC 246, `xor esi,esi` default
    quickAccess->RequestClose();                                       // 0x00625920 (called even when null)
    quickAccess->Slot26(true);                                         // [edx+68h] with 1
    BoastGroups.clear();                                               // inline: _M_erase(root) 0x0056D440 + header reset
    if (Active)
        return;

    CQuestManager* questManager = CQuestManager::Get();                // [0x013B89FC]
    long questsWithBoasts = 0;
    for (std::list<CCountedPointer<CQuestManager::CActiveQuest> >::iterator it = questManager->ActiveQuests.begin();
         it != questManager->ActiveQuests.end(); ++it) {
        if ((*it)->PScript.Data == 0)                                  // [quest+8]
            continue;
        CCharString scriptName((*it)->PScriptInfo->Name);              // [quest+4] +8, copy 0x0099EC30
        std::list<CQuestManager::CBoast> boasts;                       // malloc(0x28) header node
        questManager->GetBoastsForScript(scriptName, boasts);          // 0x004B19F0
        if (!boasts.empty())
            ++questsWithBoasts;
    }                                                                  // list clear 0x004B8C00 + free, ~CCharString
    if (questsWithBoasts == 0) {
        ActivationRequested = false;
        return;
    }

    gui->Close(true);                                                  // 0x00438400
    gui->PrepareForLoad();                                             // 0x004382C0
    NUISystem::CManager* manager = NUISystem::CManager::GetInstance(); // 0x0041E5F2
    NUISystem::CComponent* mousePointer = manager->CreateComponent(CCharString("UI_MOUSE_POINTER"), false);
    NUISystem::CComponent* boastList = manager->CreateComponent(CCharString("PC_BOAST_LIST"), false);
    boastList->Slot156(questsWithBoasts);                              // [eax+270h]

    for (std::list<CCountedPointer<CQuestManager::CActiveQuest> >::iterator it = questManager->ActiveQuests.begin();
         it != questManager->ActiveQuests.end(); ++it) {
        if ((*it)->PScript.Data == 0)
            continue;
        CCharString scriptName((*it)->PScriptInfo->Name);
        CThing* questCardThing = questManager->GetActiveQuestCardFromScriptName(scriptName);   // 0x004B0C80
        if (questCardThing == 0)
            continue;
        CTCQuestCard* questCard;
        questCardThing->GetTC(&questCard);                             // TC 108, out-pointer form
        CWideString questName = questCard->GetQuestName();             // 0x00702310
        std::list<CQuestManager::CBoast> boasts;
        questManager->GetBoastsForScript(scriptName, boasts);
        if (boasts.empty())
            continue;

        CCountedPointer<NUISystem::CComponent> branch(manager->CreateMenuEntryComponent(questName, 0.0f, 1.0f,
            CCharString("PC_MENU_ENTRY_BRANCH_TEMPLATE"), 0.0f, CCharString(""), 15.0f, CCharString("PC_BUTTON_BASTARD")));
        boastList->Slot152(branch);                                    // [edx+260h]
        CCountedPointer<NUISystem::CAction> openAction(new NUISystem::CAction);
        openAction->ActionType = (NUISystem::EActionType)0xBBA;
        branch->AddActionOnBack(openAction);                           // slot 68
        CCountedPointer<NUISystem::CComponent> subList(manager->CreateComponent(CCharString("PC_BOAST_SUB_LIST"), false));
        NUISystem::CComponent* subListComponent = subList.Data;        // [esp+118h]
        branch->AddChild(subList);                                     // slot 59
        subListComponent->Slot156(boasts.size());                      // list size out of line 0x004B87D0
        std::map<long, std::vector<NUISystem::CComponent*> > groupEntries;          // [esp+188h] (0x0056E4A0 / 0x0056E4E0)
        std::map<NUISystem::CComponent*, NUISystem::CComponent*> takeEntries;       // [esp+1B4h] (0x0056D770 / 0x0056D7B0)
        for (std::list<CQuestManager::CBoast>::iterator boast = boasts.begin(); boast != boasts.end(); ++boast) {
            CWideString title((*boast).Title);                         // copy 0x0099B720
            CWideString description((*boast).Text);
            CCountedPointer<NUISystem::CComponent> entry(manager->CreateMenuEntryComponent(title, 0.0f, 1.0f,
                CCharString("PC_MENU_ENTRY_BRANCH_TEMPLATE"), 0.0f, CCharString(""), 15.0f, CCharString("PC_BUTTON_BASTARD")));
            subListComponent->Slot152(entry);
            CCountedPointer<NUISystem::CComponent> detailList(manager->CreateComponent(CCharString("PC_BOAST_SUB_LIST"), false));
            CCountedPointer<NUISystem::CListType144> detailListAsList;  // {0,0} then conversion-assign 0x0042A2DB
            detailListAsList = detailList;
            entry->AddChild(detailList);
            CCountedPointer<NUISystem::CComponent> backdrop(manager->CreateComponent(CCharString("PC_BOAST_BACKDROP"), false));
            entry->AddChild(backdrop);
            const NGameText::CDataBank* text = CMainGameComponent::Get()->PTextBank;   // [0x013B86A0]+0x14
            CWideString value;
            value = text->GetTextBySymbol(CCharString("TEXT_GUI_MENU_QUEST")) + L": " + questName + L"\n";
            value += text->GetTextBySymbol(CCharString("TEXT_GUI_MENU_BOAST")) + L": " + description + L"\n";   // [esp+54h] = Text copy
            value += text->GetTextBySymbol(CCharString("TEXT_GUI_MENU_WAGER")) + L": " + GFIntToWideString((*boast).Cost) + L" "
                   + text->GetTextBySymbol(CCharString("TEXT_GUI_MENU_GOLD")) + L", ";
            value += text->GetTextBySymbol(CCharString("TEXT_GUI_MENU_REWARD")) + L": " + GFIntToWideString((*boast).Reward) + L" "
                   + text->GetTextBySymbol(CCharString("TEXT_GUI_MENU_GOLD")) + L"\n";
            CCountedPointer<NUISystem::CComponent> descriptionText(manager->CreateComponent(CCharString("PC_BOAST_DESCRIPTION_TEXT"), false));
            static_cast<NUISystem::CText*>(descriptionText.Data)->SetValue(value, true, true);   // slot 144
            entry->AddChild(descriptionText);
            CWideString takeLabel = text->GetTextBySymbol(CCharString("TEXT_GUI_MENU_TAKE_BOAST"));
            CCountedPointer<NUISystem::CComponent> take(manager->CreateMenuEntryComponent(takeLabel, 0.0f, 1.0f,
                CCharString("PC_MENU_ENTRY_TEMPLATE"), 0.0f, CCharString(""), 15.0f, CCharString("PC_BUTTON_BASTARD")));
            NUISystem::CComponent* takeComponent = take.Data;          // [esp+194h]
            CCountedPointer<NUISystem::CAction> takeAction(new NUISystem::CAction);
            takeAction->ActionType = (NUISystem::EActionType)4;
            takeAction->UIActionParams.push_back(new NUISystem::CActionParamSLONG((*boast).ID));
            CWideString script = (*boast).ScriptName.ToWideString(); // 0x0099E480
            takeAction->UIActionParams.push_back(new NUISystem::CActionParamString(script));
            take->AddAction(takeAction);                               // slot 66
            if (boasts.size() > 1) {                                   // inline node count
                CCountedPointer<NUISystem::CAction> backAction(new NUISystem::CAction);
                backAction->ActionType = (NUISystem::EActionType)0x26;
                take->AddAction(backAction);
                NUISystem::CComponent* entryComponent = entry.Data;    // [esp+34h]
                entryComponent->Slot151(entryComponent);               // passes the +0x18 base subobject (null-checked)
                long group = (*boast).GroupIndex;                      // [esp+108h]
                if (group != 0) {
                    BoastGroups.insert(std::make_pair(entryComponent, *boast));        // make_pair 0x0056CB40, convert 0x0056CB60, insert 0x0056D930
                    takeEntries.insert(std::make_pair(takeComponent, entryComponent)); // insert_unique 0x0056D9F0
                    std::map<long, std::vector<NUISystem::CComponent*> >::iterator g = groupEntries.find(group);   // 0x0056D130
                    if (g == groupEntries.end()) {
                        std::pair<long, std::vector<NUISystem::CComponent*> > newGroup(group, std::vector<NUISystem::CComponent*>());
                        newGroup.second.push_back(entryComponent);    // 0x0056DCE0
                        groupEntries.insert(newGroup);                 // convert 0x0056DAB0, insert_unique 0x0056E020
                    } else {
                        (*g).second.push_back(entryComponent);
                    }
                }
            }
            detailListAsList->Slot144(take);                           // [edx+240h]
        }
    }

    boastList->Initialise();                                           // slot 43
    boastList->ChangeState(5);                                         // slot 48
    mousePointer->Initialise();
    Components.push_back(mousePointer);                                // inline, _M_insert_overflow 0x00485DBC
    Components.push_back(boastList);
    Active = true;
    CThingPlayerCreature* hero = GetPlayerManager().GetMainPlayer()->GetPControlledCreature();   // 0x00686D40 / 0x00449970 / 0x00487DC0
    if (hero) {
        CTCCombat* combat;
        hero->GetTC(&combat);                                          // TC 48, out-pointer form
        if (combat->IsBlocking())                                      // 0x007331E0 (byte +0x63)
            combat->UnsetBlocking();                                   // 0x007338A0
        if (hero->HasTC(TCI_FIREBALL_SPELL))                           // TC 157 (bit +0x30 0x20000000)
            hero->GetTCUnchecked157()->CancelMove();                   // 0x007C8FD0
    }
    GetPlayerManager().GetMainPlayer()->AddMode(0x20, 0);              // 0x006345C0
}
