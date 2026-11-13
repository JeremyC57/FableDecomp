// NUISystem::CFrontEndManager::ConstructFileDescription(unsigned long index, CComponent* button, CComponent* menu, CWideString& name) -- give one save-list button its description and picture: a description text component "<profile name label> : <profile>\n<date> <time>\n<region label> : <region display name>\n<time played>" shown through the menu's scrolling text viewport, and, when the save (and its ".hs" header, if present) verifies and the slot is not the empty-slot text, the region's minimap picture (else the default save picture) in the screenshot viewport; the picture, the description box and (for a real save) the text component become the button's dynamic children
// Leaf: ?ConstructFileDescription@CFrontEndManager@NUISystem@@IAEXKPAVCComponent@2@0AAVCWideString@@@Z
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x00595CC1, 1613 bytes, thiscall, `ret 10h` at 0x0059630B, /GS cookie in the biased size-TU frame
//   (`push ebp ; lea ebp,[esp-1C8h] ; sub esp,248h`; the 2 x 128 unsigned short date / time buffers). Unpadded size TU: the previous
//   function ends `ret 8` at 0x00595CBE, the next (0x0059630E) follows the `ret 10h` directly.
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?ConstructFileDescription@CFrontEndManager@NUISystem@@IAEXKPAVCComponent@2@0AAVCWideString@@@Z
//   (protected); its only caller 0x005988FE in RefreshAvailableSavedGamesForProfile 0x00598463 (chunk 2003) passes (i, button.Get(),
//   menu, save_names[i]); the same text as the landed CTCInGameMenu::ConstructSaveGameFileDescription 0x0047EEE4 (in-game twin).
// Callees: CreateComponent 0x00594F94, CCountedPointer<CComponent>(T*) 0x00429C15 / ReleaseData 0x004291DE, CCountedPointer<CText>
//   ShareData 0x00429460 (inline SetStaticCast) / ReleaseData 0x0042925C, NGameText::CDataBank::GetTextBySymbol 0x009C95E0 /
//   GetPGroupEntry 0x009C8890 / GetPTextEntry 0x009C9070, CBankFile::FindIndexBySymbolCRC 0x009CC410 / GetEntryDataType 0x009CC210,
//   NGameText::CTextEntry::GetText 0x009C7A40, CUserProfileManager::Get 0x0040D2A0 / GetPathNameFromFileName 0x00408120 /
//   GetEmptySlotName 0x00406A80 (GetProfileName inline: +8), CDateAndTime::ToFileTime 0x009E9D40, CXBConfig::FormatDateTime
//   0x00A0D980, CManager::GetInstance 0x0041E5F2 + slot 42 GetMiscThingsDef (SpaceSeparator +0x3C, MiniMapGraphics +0x21C),
//   map<CCharString, long>::_M_find 0x0042B467, CTCInventoryStats::GetTimeSpentInGame 0x005CCC91, CText slot 144 SetValue,
//   CComponent slots 3 FindChild / 60 AddChildDynamically, CViewport slot 142 AddFakeChild, CSprite slot 145 SetGraphicIndex,
//   CAFile::PathExists 0x00999230, VerifySignatureCached 0x005957D9, CWideString () 0x0099AED0 / (const unsigned short*) 0x0099B6B0 /
//   ~ 0x0099B510 / = 0x0099B7D0 / += 0x0099B8D0 and 0x0099B940 / != 0x0099B0F0 / operator+ 0x0099BF30, CCharString(const char*, long)
//   0x0099EBF0 / ~ 0x0099EAE0 / != (const char*) 0x0099E960 / GetCRC 0x00404310 / ToWideString 0x0099E480, __security_check_cookie.
// Shape (chunk 2013): shared headers with hdr/ overlay opt-ins (hdr/README.md). The size TU calls the CIVCountedPointer destructor
//   as a COMDAT unless it is forced (FABLE_RETAIL_CIVCOUNTEDPOINTER_DTOR_DIRECT_CLEAR: one level, null store kept, which retail keeps
//   for the group temporary at 0x00595F7C). The text / group arms are written text first (retail falls through into the text arm
//   and jumps to the group arm, 0x00595EFC); with the references and nested ifs below in place the group arm keeps its own copy of
//   the text-entry tail and the ToWideString arm sits behind the `valid` computation (0x005960D5) as in retail. The minimap name and the minimap map are named references
//   (retail keeps &FileRegionMiniMapNames[index] in ebx across the != call and &MiniMapGraphics in edi across _M_find, 0x0059618A /
//   0x005961A5). The valid / non-empty tests are nested ifs (an `&&` with the GetEmptySlotName temporary adds a conditional-
//   destruction flag). CArray::operator[] through begin() (FABLE_RETAIL_CARRAY) gives the group entry's first id in edi
//   (`mov edi,[edi] ; push [edi]`, 0x00595F83; `_M_start[0]` gives eax). FindChild results are tested as CComponent* and cast for
//   the AddFakeChild call (a static_cast before the test adds a null-check compare).
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CWIDESTRING_NE
#define FABLE_RETAIL_CFRONTENDMANAGER_DESTRUCTOR
#define FABLE_RETAIL_CFRONTENDMANAGER_SAVEDGAMES
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CTEXT
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CUSERPROFILEMANAGER_SAVENAMES
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#define FABLE_RETAIL_CDATEANDTIME_CTOR
#define FABLE_RETAIL_CDATEANDTIME_TOFILETIME
#define FABLE_RETAIL_CBANKFILE_FINDINDEXBYSYMBOLCRC
#define FABLE_RETAIL_NGAMETEXT_GROUPENTRY
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_DTOR_DIRECT_CLEAR
#define FABLE_RETAIL_CTCINVENTORYSTATS_TIMESPENT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#include "engine/ETCInterfaceType.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "engine/CDateAndTime.h"
#include "engine/CFrontEndManager.h"
#include "engine/NUISystem_CManager.h"
#include "engine/CDefPointer.h"
#include "engine/NUISystem_CUIMiscThingsDef.h"
#include "engine/CText.h"
#include "engine/NUISystem_CViewport.h"
#include "engine/NUISystem_CSprite.h"
#include "engine/CAFile.h"
#include "engine/CUserProfileManager.h"
#include "engine/CNewFrontendGameComponent.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/CXBConfig.h"
#include "engine/CTCInventoryStats.h"

namespace NUISystem {
void CFrontEndManager::ConstructFileDescription(unsigned long index, CComponent* button, CComponent* menu, CWideString& name)
{
    CCountedPointer<CComponent> description_box(CreateComponent("UI_TEXT_FILE_DESCRIPTION"));
    CCountedPointer<CComponent> description_component(CreateComponent("UI_TEXT_WEAPONS_DESCRIPTION_TEMPLATE_NEW"));
    CCountedPointer<CText> description_text;
    description_text.SetStaticCast(description_component);
    CWideString description = CNewFrontendGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_PROFILE_NAME") + L" : ";
    description += CUserProfileManager::Get().GetProfileName();
    description += L"\n";
    unsigned short time_text[128];
    unsigned short date_text[128];
    CXBConfig::FormatDateTime(m_vFileTimes[index].ToFileTime(), date_text, time_text, 0);
    CWideString date_and_time(date_text);
    date_and_time += CManager::GetInstance()->GetMiscThingsDef()->SpaceSeparator;
    date_and_time += CWideString(time_text);
    description += date_and_time;
    description += L"\n";
    description += CNewFrontendGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_REGION") + L" : ";
    CWideString region_name;
    const NGameText::CDataBank& text_bank = CNewFrontendGameComponent::Get()->GetTextBank();
    unsigned long entry = text_bank.FindIndexBySymbolCRC(FileRegionNames[index].GetCRC());
    if (entry > 0) {
        if (text_bank.GetEntryDataType(entry) == 0)
            region_name = text_bank.GetPTextEntry(entry, true)->GetText();
        else {
            const CArray<long>& entries = text_bank.GetPGroupEntry(entry, true)->Entries;
            region_name = text_bank.GetPTextEntry(entries[0], true)->GetText();
        }
    } else
        region_name = FileRegionNames[index].ToWideString();
    description += region_name;
    description += L"\n";
    CWideString time_played = CTCInventoryStats::GetTimeSpentInGame(FilePlayTimes[index], &CNewFrontendGameComponent::Get()->GetTextBank());
    description += time_played;
    description_text->SetValue(description, true, true);
    CComponent* text_viewport = menu->FindChild("UI_SCROLLING_VIEWPORT_TEXT_AREA");
    if (text_viewport)
        static_cast<CViewport*>(text_viewport)->AddFakeChild(description_component.Get());
    CWideString header_name = CUserProfileManager::Get().GetPathNameFromFileName(name, false) + L".hs";
    bool header_exists = false;
    if (CAFile::PathExists(header_name))
        header_exists = true;
    bool valid = VerifySignatureCached(CUserProfileManager::Get().GetPathNameFromFileName(name, false)) && (!header_exists || VerifySignatureCached(header_name));
    CComponent* picture = 0;
    if (valid) {
        if (name != CUserProfileManager::Get().GetEmptySlotName()) {
            picture = CreateComponent("UI_RING_PIC_DRAW_FROM_VIEWPORT");
            CDefPointer<CUIMiscThingsDef> misc_things = CManager::GetInstance()->GetMiscThingsDef();
            const CCharString& minimap_name = FileRegionMiniMapNames[index];
            if (minimap_name != "") {
                std::map<CCharString, long>& minimap_graphics = misc_things->MiniMapGraphics;
                std::map<CCharString, long>::iterator it = minimap_graphics.find(minimap_name);
                if (it != minimap_graphics.end() && (unsigned long)(*it).second > 0)
                    static_cast<CSprite*>(picture)->SetGraphicIndex((*it).second);
            }
        }
    }
    if (!picture)
        picture = CreateComponent("UI_RING_PIC_SAVE_VIEWPORT");
    CComponent* screenshot_viewport = menu->FindChild("UI_SCREENSHOT_VIEWPORT");
    if (screenshot_viewport)
        static_cast<CViewport*>(screenshot_viewport)->AddFakeChild(picture);
    CCountedPointer<CComponent> picture_pointer(picture);
    button->AddChildDynamically(picture_pointer);
    button->AddChildDynamically(description_box);
    if (name != CUserProfileManager::Get().GetEmptySlotName())
        button->AddChildDynamically(description_component);
}
}