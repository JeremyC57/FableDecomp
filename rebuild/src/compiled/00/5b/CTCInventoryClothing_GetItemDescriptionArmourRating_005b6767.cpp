// CTCInventoryClothing::GetItemDescriptionArmourRating(CWideString& text, long itemDefIndex) const -- append "<Armour rating> N (+D)" for a clothing item: its armour value and the signed difference to what is worn now
// Leaf: ?GetItemDescriptionArmourRating@CTCInventoryClothing@@QBEXAAVCWideString@@J@Z
// Retail 0x005B6767, 282 bytes, `ret 8`. Frame-pointer size TU: optimize("y", off) + ("s", on). Callees: ArgPasser::ArgPasser 0x005B336A, CalculateArmorValues 0x005B4EBA, NGameText::CDataBank::GetTextBySymbol 0x009C95E0 through the inline CMainGameComponent::GetTextBank (PTextBank +0x14 of PMainGameComponent 0x013B86A0), CCharString 0x0099EBF0 / ~ 0x0099EAE0, CWideString(const wchar_t*) 0x0099B6B0, operator+= 0x0099B8D0, ~CWideString 0x0099B510, GFIntToWideString 0x00405F00.
// Shape: retail reads PMainGameComponent (0x005B6787) before the armour difference (0x005B678C / 0x005B678F) and the text bank
//   pointer (+0x14, 0x005B6792) after it. The c2 list scheduler ties the +0x14 load and the `sub` (prio 0x15000) and takes the lower
//   IL index, so the IL must be global load, difference, +0x14 load. A named `game` pointer read before the difference gives that
//   only when it has a second use: VC7.1 otherwise forwards the global read to its one use after the difference (md 4), and the
//   bank reference bound first loads +0x14 IL-first (md 2). The null test on `game` after GetTextBank() is that use; VC7.1 drops
//   it (game was already dereferenced), so no test is emitted. A test before the dereference stays in the code (290 bytes).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CTCINVENTORYCLOTHING_ARMOUR
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/CMainGameComponent.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/CTCInventoryClothing.h"

CWideString FABLE_FASTCALL GFIntToWideString(long value);    // 0x00405F00 ?GFIntToWideString@@YI?AVCWideString@@J@Z

void CTCInventoryClothing::GetItemDescriptionArmourRating(CWideString& text, long itemDefIndex) const
{
    ArgPasser values;
    CalculateArmorValues(values, itemDefIndex);
    const CMainGameComponent* game = CMainGameComponent::Get();
    long difference = values.total_item_armor_value_final - values.total_cur_armor_value_final;
    const NGameText::CDataBank& textBank = game->GetTextBank();
    if (game == 0)                                     // never taken: GetTextBank() above already dereferenced game
        return;
    text += textBank.GetTextBySymbol("TEXT_GUI_MENU_ARMOUR_RATING");
    text += CWideString(L" ");
    text += GFIntToWideString(values.total_item_armor_value_final);
    text += CWideString(L" (");
    if (difference > 0)
        text += CWideString(L"+");
    text += GFIntToWideString(difference);
    text += CWideString(L")");
}