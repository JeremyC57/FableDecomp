// CTCInventoryExperience::AddMagicCategoryToList(CList* list, CWideString title, CCharString definition, CList* subList, CWideString description, CCharString icon) -- adds a magic-category branch entry (title, sub-list, category description and subtitle) to the experience-spending list
// Leaf: ?AddMagicCategoryToList@CTCInventoryExperience@@AAEXPAVCList@NUISystem@@VCWideString@@VCCharString@@012@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0056F69F, 517 bytes, `ret 18h`; frame-pointer size TU
// (`optimize("y", off)` + `("s", on)`, as the landed AddSoundAction 0x0056F615 of the same TU).
// Callees: CManager::GetInstance 0x0041E5F2 (__fastcall), CManager::CreateMenuEntryComponent 0x00428494
// (manifest signature: const CWideString&, float, float, const CCharString&, float, CCharString, float, CCharString),
// CManager::CreateComponent(const CCharString&, bool) 0x0041DB1D, CCountedPointer<CComponent>(CComponent*) 0x00429C15
// and its dtor 0x004291DE, AddSoundAction 0x0056F615, CCharString(const char*, long) 0x0099EBF0 / ~CCharString
// 0x0099EAE0, CCharString::ToWideString 0x0099E480, ~CWideString 0x0099B510; component slots 59 (+0xEC, AddChild),
// 144 (+0x240, SetText(const CWideString&, bool, bool)), list slot 152 (+0x260, AddItem), viewport slot 142 (+0x238).
// Layout: retail CTCInventoryExperience pTextViewport +0x1DC (landed ctor 0x0057832D).
// Name fix (agentNF 2026-11-10): 0x004291DE is CCountedPointer<NUISystem::CComponent>::ReleaseData (the destructor is the 5-byte
//   `jmp` COMDAT 0x00429C53; Ego_r.pdb ??1 is 5 bytes): FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (engine/CCountedPointer.h).
// NF82 (q11002): moved off the row-local CWideString / CCharString / NUISystem::CComponent (slot padding) / CList / CViewport / CManager /
//   CTCInventoryExperience models onto the shared headers (the 0x00573792 AddStatToListPC recipe of the same class): fable_string.h,
//   engine/CTCInventoryExperience.h (FABLE_RETAIL_CTCINVENTORYEXPERIENCE + _PRIVATE: Ego_r.pdb private AddSoundAction /
//   AddMagicCategoryToList, pTextViewport +0x1DC), engine/NUISystem_CManager.h (Ego_r.pdb CreateMenuEntryComponent(const CWideString&,
//   float, float, const CCharString&, float, const CCharString, float, CCharString): the call now binds the PDB spelling
//   ..MV5@MV5@@Z, was the stand-in ..MV5@M2@Z), NUISystem_CList.h (slot 152 AddScrollingChildAndSetOffsets), NUISystem_CViewport.h
//   (slot 142 AddFakeChild) and NUISystem_CText.h (slot 144 SetValue(const CWideString&, bool, bool) on the created components,
//   cast as the sibling rows do).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE_PRIVATE
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CTEXT
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryExperience.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CViewport.h"
#include "engine/NUISystem_CText.h"

void CTCInventoryExperience::AddMagicCategoryToList(NUISystem::CList* list, CWideString title, CCharString definition,
                                                    NUISystem::CList* subList, CWideString description, CCharString icon)
{
    NUISystem::CManager* frontEnd = NUISystem::CManager::GetInstance();
    CCountedPointer<NUISystem::CComponent> entry(frontEnd->CreateMenuEntryComponent(title, 0.0f, 0.0f, "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW",
                                                                                    0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
    CCountedPointer<NUISystem::CComponent> subPointer(subList);
    AddSoundAction(entry.operator->(), "SND_TEXT_QST_TUTORIAL_GUI_EXPSPEND_SUB_ALL");
    CCountedPointer<NUISystem::CComponent> definitionPointer(NUISystem::CManager::GetInstance()->CreateComponent(definition, false));
    entry->AddChild(subPointer);
    entry->AddChild(definitionPointer);
    CCountedPointer<NUISystem::CComponent> descriptionPointer(frontEnd->CreateComponent("UI_EXPSPEND_CAT_DESC", false));
    static_cast<NUISystem::CText*>(descriptionPointer.operator->())->SetValue(icon.ToWideString(), true, true);
    entry->AddChild(descriptionPointer);
    if (pTextViewport)
        pTextViewport->AddFakeChild(descriptionPointer.Data);
    CCountedPointer<NUISystem::CComponent> subtitlePointer(frontEnd->CreateComponent("UI_ONE_OFF_SUBTITLE", false));
    static_cast<NUISystem::CText*>(subtitlePointer.operator->())->SetValue(description, true, true);
    entry->AddChild(subtitlePointer);
    list->AddScrollingChildAndSetOffsets(entry);
}
