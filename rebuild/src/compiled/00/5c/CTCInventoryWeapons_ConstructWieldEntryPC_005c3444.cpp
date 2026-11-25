// CTCInventoryWeapons::ConstructWieldEntryPC(bool melee, NUISystem::CViewport* viewport, NUISystem::CComponent* list, long defIndex) -- PC twin of ConstructWieldEntry: the guide text is the melee/ranged PC wield guide plus two separators and the item def's description, and both texts are also registered with the optional list
// Grade: RELOCATION_MATCH -- the shared engine/CCountedPointer.h (members Data / Info) in place of the parked local CCountedPointer stand-in (m_pObject): the entry component now loads into esi after the viewport call as retail does (parked residue: one scheduling difference)
// Leaf: ?ConstructWieldEntryPC@CTCInventoryWeapons@@QAE?AV?$CCountedPointer@VCComponent@NUISystem@@@@_NPAVCViewport@NUISystem@@PAVCComponent@4@J@Z
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x005C3444, 638 bytes, `ret 14h`. Frame-pointer size TU: optimize("s", on).
// FableWin.pdb: ?ConstructWieldEntryPC@CTCInventoryWeapons@@QAE?AV?$CCountedPointer@VCComponent@NUISystem@@@@_NPAVCViewport@NUISystem@@PAVCComponent@4@J@Z
// Callees as 0x005C323A, plus CWideString copy @ 0x0099B720 (from the empty string 0x013BCA24) / operator= @ 0x0099B7D0 /
// operator+= @ 0x0099B8D0 (twice with the static string 0x013B8BA8), the item-def description @ 0x005D9630 (`__fastcall`, hidden
// result in ecx), and the list's slot 124 (+0x1F0).
// Shape: see 0x005C323A (implicit const char* conversions, the guide bank / description component read early, a named entry
// component pointer); tried a reference local, a split declaration and a same-TU counted-pointer ctor body for the esi load.
// Name fix (agentNF 2026-11-10): 0x004291DE is CCountedPointer<NUISystem::CComponent>::ReleaseData (the destructor is the 5-byte
//   `jmp` COMDAT 0x00429C53; Ego_r.pdb ??1 is 5 bytes): FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (engine/CCountedPointer.h).
// Name fix (agentNF26, 2026-11-12, FAM09 follow-up): 0x013BCA24 is CWideString::NullString (?NullString@CWideString@@2V1@A, public static;
//   Ego_r.pdb + FableWin.pdb `dynamic initializer for 'CWideString::NullString'', initializer 0x01223DA0 landed by agentFAM09); was g_EmptyWideString.
// NF85 (q11059): moved off the row-local CWideString / CTCInventoryItem / CCharString / NGameText::CDataBank / CMainGameComponent /
//   NUISystem CAction / CComponent (slot padding) / CViewport / CManager / CTCInventoryWeapons models onto the shared headers, as its
//   console twin 0x005C323A (NF85) with the sibling 0x005C574E's opt-ins: CreateMenuEntryComponent 0x00428494 now binds the Ego_r.pdb
//   spelling ..MV5@MV5@@Z (the local model's non-const 6th parameter gave the stand-in ..MV5@M2@Z); slots by their header names (59
//   AddChild, 66 AddAction, 124 AddPositionChild, CViewport 142 AddFakeChild, CText 144 SetValue); FABLE_RETAIL_CCOUNTEDPOINTER_
//   VISIBLE_CALLED_CTOR keeps the entry pointer tracked, so its load into esi stays after the viewport call as in retail.
// NF419 (2026-11-25): no `#pragma optimize("y", off)`: the leaf is byte- and relocation-identical without it (size mode keeps
//   its frame); it only framed end-of-TU template and compiler-generated copies (each copy checked in the T3 nf416 audit).
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CWIDESTRING_NULLSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYWEAPONS_METHODS
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_KINDS
#define FABLE_RETAIL_CTCINVENTORYITEM_DETAILS
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryWeapons.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CMainGameComponent.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CText.h"
#include "engine/NUISystem_CViewport.h"
#include "engine/NUISystem_CAction.h"

namespace { extern const CWideString NEWLINE; }                  // 0x013B8BA8 (`anonymous namespace'::NEWLINE of this TU, L"\n"; ESTABLISHED_SPELLINGS)

CCountedPointer<NUISystem::CComponent> CTCInventoryWeapons::ConstructWieldEntryPC(bool melee, NUISystem::CViewport* viewport, NUISystem::CComponent* list, long defIndex)
{
    const NGameText::CDataBank& bank = CMainGameComponent::Get()->GetTextBank();
    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    CWideString text = bank.GetTextBySymbol("TEXT_GUI_MENU_WIELD");
    CCountedPointer<NUISystem::CComponent> entry(manager->CreateMenuEntryComponent(text, 0.0f, 1.0f,
        "PC_MENU_ENTRY_TEMPLATE", 0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
    CCountedPointer<NUISystem::CComponent> description(manager->CreateComponent("PC_DESCRIPTION", false));
    const NGameText::CDataBank& guideBank = CMainGameComponent::Get()->GetTextBank();
    NUISystem::CComponent* descriptionComponent = description.Data;
    CCharString guide(melee ? "TEXT_GUI_MENUGUIDE_WEAPONS_MELEE_WIELD_PC" : "TEXT_GUI_MENUGUIDE_WEAPONS_RANGED_WIELD_PC");
    CWideString guideText(CWideString::NullString);
    guideText = guideBank.GetTextBySymbol(guide);
    guideText += NEWLINE;
    guideText += NEWLINE;
    guideText += CTCInventoryItem::GetDetails(defIndex);
    static_cast<NUISystem::CText*>(descriptionComponent)->SetValue(guideText, true, true);
    NUISystem::CComponent* entryComponent = entry.Data;
    viewport->AddFakeChild(description.Data);
    entryComponent->AddChild(description);
    if (list)
        list->AddPositionChild(description.Data);
    CCountedPointer<NUISystem::CComponent> name(manager->CreateComponent("PC_SUBTITLE", false));
    static_cast<NUISystem::CText*>(name.Data)->SetValue(text, true, true);
    entryComponent->AddChild(name);
    if (list)
        list->AddPositionChild(name.Data);
    CCountedPointer<NUISystem::CAction> action(new NUISystem::CAction);
    action->ActionType = NUISystem::UI_ACTION_TYPE_WIELD;
    entryComponent->AddAction(action);
    return entry;
}
