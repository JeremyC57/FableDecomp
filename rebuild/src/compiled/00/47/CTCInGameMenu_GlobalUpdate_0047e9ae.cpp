// CTCInGameMenu::GlobalUpdate(bool) -- per-frame menu work: queues a pending load-game event, counts down a deferred save, and drives the "video options changed" dialog's 15-second revert timeout
// Leaf: ?GlobalUpdate@CTCInGameMenu@@UAEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0047E9AE, 860 bytes; frame-pointer size TU (`optimize("y", off)` + `("s", on)`), /GS cookie.
// Shape: the X centre term is written `(GetBackBufferWidth() * 0.5f) - scale.X * K`; the redundant parentheses add a no-code PAREN
//   tuple that moves the float-argument slot `push ecx` below the X `fsubp`, as retail (the unparenthesised form pushes it first).
//   IS_THING_ALIVE<CThingPlayerCreature*> 0x0043DBD0 is the shared engine/ThingAlive.h body, visible but out of line
//   (FABLE_RETAIL_IS_THING_ALIVE_NOINLINE) as in the retail TU, so VC7.1 keeps the hero in edi across the call (0x0047E9E6).
// Callees: CTCBase::GetPlayerManager 0x00686D40, CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetPControlledCreature
//   0x00487DC0, CGameEvent::AddData 0x009F1760 (AddSLONG / SetPlayer / the ctor expanded), CMainGameComponent::AddGameEvent
//   0x0041649C, CTCInGameMenu::SaveGame 0x0047E196, GFGetTime 0x009E1BC0, CThing::GetTC (bitset test 0x00410DE0 + CVectorMap
//   LowerBound 0x0040F020), CTCInventory::RevertVideoChanges 0x00582754, CUserProfileManager::Get 0x0040D2A0 / Apply 0x0040BC80 /
//   Save 0x0040A740, NGameText::CDataBank::GetTextBySymbol 0x009C95E0, CWideString operator+= 0x0099B940 / 0x0099B8D0 / ~ 0x0099B510,
//   operator+(const wchar_t*, const CWideString&) 0x0099BFF0, GFIntToWideString 0x00405F00, CManager::GetUIScale 0x0041CF47,
//   CUIScreenUnitConverter::GetBackBufferWidth / Height; virtuals CComponent FindChild (3) / ChangePosition (31) /
//   GetTargetStateIndex (87), CText::SetValue (145 CCharString / 144 CWideString).
// Flags: /GS
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORY
#define FABLE_RETAIL_CTCINVENTORY_REVERTVIDEOCHANGES
#define FABLE_RETAIL_CTCINGAMEMENU
#define FABLE_RETAIL_CTCINGAMEMENU_SAVELOAD
#define FABLE_RETAIL_CTCINGAMEMENU_VIDEODIALOGS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CGAMEEVENT_INLINE
#define FABLE_RETAIL_CGAMEEVENT_INLINE_ADDMORE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_ADDGAMEEVENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CUSERPROFILEMANAGER_SETTERS
#define FABLE_RETAIL_CUSERPROFILEMANAGER_DISPLAYDIALOG
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CYESNO
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#include "stlport/fable_containers.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/ThingAlive.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/CGameEvent.h"
#include "engine/CMainGameComponent.h"
#include "engine/CUserProfileManager.h"
#include "engine/CTCInventory.h"
#include "engine/CTCInGameMenu.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CText.h"
#include "engine/NUISystem_CYesNo.h"
#include "engine/NUISystem_CUIScreenUnitConverter.h"
#include "engine/GFGetTime.h"


CWideString __fastcall GFIntToWideString(long value);                // 0x00405F00 ?GFIntToWideString@@YI?AVCWideString@@J@Z

using namespace NUISystem;

void CTCInGameMenu::GlobalUpdate(bool)
{
    if (!InGameMenuOpen)
        return;
    if (LoadGameIndex != -1) {
        CThingPlayerCreature* hero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        if (IS_THING_ALIVE(hero)) {
            CGameEvent event(60);                                    // the load-game event (EGameEventType 60; no enumerator dump)
            event.AddSLONG(LoadGameIndex);
            event.SetPlayer(hero->GetPlayerNumber());
            CMainGameComponent::Get()->AddGameEvent(event);
        }
        LoadGameIndex = -1;
    }
    if (SaveGameIndex != -1) {
        if (FramesToWaitToSave > 0) {
            --FramesToWaitToSave;
        } else {
            SaveGame(SaveGameIndex);
            SaveGameIndex = -1;
            FramesToWaitToSave = -1;
        }
    }
    if (PVideoOptionsChangedDialog && PVideoOptionsChangedDialog->GetTargetStateIndex() == 1 && VideoChangesTimer != -1.0) {
        if (GFGetTime() - VideoChangesTimer > 15.0) {
            CThing* parent = ParentThing;
            CTCInventory* inventory;
            if (parent->GetTC(TCI_INVENTORY, &inventory) && inventory)
                inventory->RevertVideoChanges();
            CUserProfileManager::Get().SetDisplayDialogOnOptionsChanged(false);
            CUserProfileManager::Get().Apply();
            CUserProfileManager::Get().Save();
            CUserProfileManager::Get().SetDisplayDialogOnOptionsChanged(true);
            CText* text = static_cast<CText*>(PVideoOptionsChangedDialog->FindChild("PC_GAME_OPTIONS_CHANGED_TEXT"));
            if (text)
                text->SetValue("TEXT_GUI_OPTIONS_RESET_TIMEOUT", true, true);
            C2DVector pos((CUIScreenUnitConverter::GetBackBufferWidth() * 0.5f) - CManager::GetInstance()->GetUIScale().X * 204.0f,
                          CUIScreenUnitConverter::GetBackBufferHeight() * 0.5f - CManager::GetInstance()->GetUIScale().Y * 52.0f);
            PVideoOptionsChangedDialog->ChangePosition(pos, 0.0f, false);
            VideoChangesTimer = -1.0;
        } else {
            CText* text = static_cast<CText*>(PVideoOptionsChangedDialog->FindChild("PC_GAME_OPTIONS_CHANGED_TEXT"));
            if (text) {
                CWideString message = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_OPTIONS_CHANGED_SUCCESSFULLY");
                message += L"\n";
                message += CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_OPTIONS_REVERTING_IN");
                message += L" " + GFIntToWideString((long)(15.0 - (GFGetTime() - VideoChangesTimer)));
                text->SetValue(message, true, true);
            }
        }
    }
}