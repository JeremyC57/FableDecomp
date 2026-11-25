// CTCInventoryWeapons::ConstructWieldEntry(bool melee, NUISystem::CViewport* viewport) -- builds the "wield" menu entry: a menu entry component labelled TEXT_GUI_MENU_WIELD, a one-off description text (melee / ranged wield guide) shown in the viewport, a name text, and a type-1 select action; returns the entry by value
// Grade: RELOCATION_MATCH -- the shared engine/CCountedPointer.h (members Data / Info) in place of the parked local CCountedPointer stand-in (m_pObject): the entry component now loads into esi after the viewport call as retail does (parked residue: one scheduling difference)
// Leaf: ?ConstructWieldEntry@CTCInventoryWeapons@@QAE?AV?$CCountedPointer@VCComponent@NUISystem@@@@_NPAVCViewport@NUISystem@@@Z
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x005C323A, 522 bytes, `ret 0Ch`. Frame-pointer size TU: optimize("s", on).
// FableWin.pdb: ?ConstructWieldEntry@CTCInventoryWeapons@@QAE?AV?$CCountedPointer@VCComponent@NUISystem@@@@_NPAVCViewport@NUISystem@@@Z
// Callees: NGameText::CDataBank::GetTextBySymbol @ 0x009C95E0 (CMainGameComponent::Get() 0x013B86A0 +0x14), CManager::GetInstance
// @ 0x0041E5F2, CManager::CreateMenuEntryComponent(const CWideString&, float, float, const CCharString&, float, CCharString, float,
// CCharString) @ 0x00428494, CManager::CreateComponent @ 0x0041DB1D, CCountedPointer<CComponent>(T*) @ 0x00429C15 / copy @ 0x00428928 /
// dtor @ 0x004291DE, CAction @ 0x0042BE50 + CCountedPointer<CAction> @ 0x0042AA29 / dtor @ 0x00429160, operator new @ 0x00BFEA1A,
// component slots 59 AddChild, 66 (+0x108), 142 (+0x238), 144 SetText; CCharString @ 0x0099EBF0 / ~ @ 0x0099EAE0, ~CWideString @ 0x0099B510.
// Shape: the CCharString definition / symbol arguments are `const char*` converted implicitly (the temporary is built before the
// pushes and passed by `lea`); the by-value CCharString parameters are built in their argument slots. The guide's bank and the
// description component are read into locals before the guide string is built; the entry component is a named pointer local
// (it moves the entry to the far frame slot, -0x30, as in retail).
// Name fix (agentNF 2026-11-10): 0x004291DE is CCountedPointer<NUISystem::CComponent>::ReleaseData (the destructor is the 5-byte
//   `jmp` COMDAT 0x00429C53; Ego_r.pdb ??1 is 5 bytes): FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (engine/CCountedPointer.h).
// NF85 (q11059): moved off the row-local CWideString / CCharString / NGameText::CDataBank / CMainGameComponent / NUISystem CAction /
//   CComponent (slot padding) / CViewport / CManager / CTCInventoryWeapons models onto the shared headers, with the include and opt-in set
//   of the landed same-TU sibling ConstructAugEntry 0x005C574E: CreateMenuEntryComponent 0x00428494 now binds the Ego_r.pdb spelling
//   ..MV5@MV5@@Z (the local model's non-const 6th parameter gave the stand-in ..MV5@M2@Z); slots by their header names (59 AddChild,
//   66 AddAction, CViewport 142 AddFakeChild, CText 144 SetValue); FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR (as 0x005C574E) keeps
//   the entry pointer tracked, so its load into esi stays after the viewport call as in retail.
// NF419 (2026-11-25): no `#pragma optimize("y", off)`: the leaf is byte- and relocation-identical without it (size mode keeps
//   its frame); it only framed end-of-TU template and compiler-generated copies (each copy checked in the T3 nf416 audit).
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
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
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryWeapons.h"
#include "engine/CMainGameComponent.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CText.h"
#include "engine/NUISystem_CViewport.h"
#include "engine/NUISystem_CAction.h"

CCountedPointer<NUISystem::CComponent> CTCInventoryWeapons::ConstructWieldEntry(bool melee, NUISystem::CViewport* viewport)
{
    const NGameText::CDataBank& bank = CMainGameComponent::Get()->GetTextBank();
    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    CWideString text = bank.GetTextBySymbol("TEXT_GUI_MENU_WIELD");
    CCountedPointer<NUISystem::CComponent> entry(manager->CreateMenuEntryComponent(text, 0.0f, 1.0f,
        "UI_WEAPON_MENU_ENTRY_ACTION_TEMPLATE_NEW", 0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
    CCountedPointer<NUISystem::CComponent> description(manager->CreateComponent("UI_TEXT_WEAPONS_DESCRIPTION_TEMPLATE_NEW_ONE_OFF", false));
    const NGameText::CDataBank& guideBank = CMainGameComponent::Get()->GetTextBank();
    NUISystem::CComponent* descriptionComponent = description.Data;
    CCharString guide(melee ? "TEXT_GUI_MENUGUIDE_WEAPONS_MELEE_WIELD" : "TEXT_GUI_MENUGUIDE_WEAPONS_RANGED_WIELD");
    static_cast<NUISystem::CText*>(descriptionComponent)->SetValue(guideBank.GetTextBySymbol(guide), true, true);
    NUISystem::CComponent* entryComponent = entry.Data;
    viewport->AddFakeChild(description.Data);
    entryComponent->AddChild(description);
    CCountedPointer<NUISystem::CComponent> name(manager->CreateComponent("UI_TEXT_TRADE_NAME_TEMPLATE_NEW", false));
    static_cast<NUISystem::CText*>(name.Data)->SetValue(text, true, true);
    entryComponent->AddChild(name);
    CCountedPointer<NUISystem::CAction> action(new NUISystem::CAction);
    action->ActionType = NUISystem::UI_ACTION_TYPE_WIELD;
    entryComponent->AddAction(action);
    return entry;
}
