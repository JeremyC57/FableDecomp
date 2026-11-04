// CTCInventoryClothing::GetItemDescriptionArmourRating(CWideString& text, long itemDefIndex) const -- appends "<Armour rating> N (+D)" for a clothing item: its armour value and the signed difference to what is worn now
// Grade: DIFFER(282v282) -- scheduling: retail loads CMainGameComponent::PMainGameComponent, the item value and the subtraction before the text-bank dereference; ours interleaves them (bank first) or reorders them (difference first)
// Leaf: ?GetItemDescriptionArmourRating@CTCInventoryClothing@@QBEXAAVCWideString@@J@Z
// Flags: none
// Retail 0x005B6767, 282 bytes, `ret 8`. Frame-pointer size TU: optimize("y", off) + ("s", on) (as the landed siblings
// GetItemDescriptionNoArmourRating 0x005B62FE / GetItemDescription 0x005B5CF8).
// Identity: Ego_r.pdb / FableWin.pdb ?GetItemDescriptionArmourRating@CTCInventoryClothing@@QBEXAAVCWideString@@J@Z (public const).
// Callees: CTCInventoryClothing::ArgPasser::ArgPasser() 0x005B336A, CTCInventoryClothing::CalculateArmorValues(ArgPasser&, long)
// const 0x005B4EBA (private; Ego_r/FableWin ?CalculateArmorValues@CTCInventoryClothing@@ABEXAAUArgPasser@1@J@Z), the main game
// component's text bank (CMainGameComponent::GetTextBank, inline, PTextBank +0x14) NGameText::CDataBank::GetTextBySymbol
// 0x009C95E0, CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CWideString(const wchar_t*) 0x0099B6B0,
// CWideString::operator+= 0x0099B8D0, ~CWideString 0x0099B510, GFIntToWideString(long) 0x00405F00 (__fastcall).
// Layout: Ego_r.pdb CTCInventoryClothing::ArgPasser (48 bytes): total_item_armor_value_final +0x0C, total_cur_armor_value_final +0x10.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDATABANK
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CCOUNTEDPOINTER
#include "fable_game_text_bank.h"
#include "fable_game.h"
CWideString __fastcall GFIntToWideString(long value);    // 0x00405F00

class CTCInventoryClothing {
public:
    struct ArgPasser {                                 // 48 bytes (Ego_r.pdb)
        bool  is_suit;                                 // +0x00
        long  num_suit_parts;                          // +0x04
        long  max_suit_parts;                          // +0x08
        long  total_item_armor_value_final;            // +0x0c
        long  total_cur_armor_value_final;             // +0x10
        float total_item_attractiveness_value_final;   // +0x14
        float total_cur_attractiveness_value_final;    // +0x18
        float total_item_scariness_value_final;        // +0x1c
        float total_cur_scariness_value_final;         // +0x20
        float total_item_goodstrength_value_final;     // +0x24
        float total_cur_goodstrength_value_final;      // +0x28
        long  total_price;                             // +0x2c
        ArgPasser();                                   // 0x005B336A
    };
    void GetItemDescriptionArmourRating(CWideString& text, long itemDefIndex) const;
private:
    void CalculateArmorValues(ArgPasser& values, long itemDefIndex) const;   // 0x005B4EBA
};

void CTCInventoryClothing::GetItemDescriptionArmourRating(CWideString& text, long itemDefIndex) const
{
    ArgPasser values;
    CalculateArmorValues(values, itemDefIndex);
    const NGameText::CDataBank& textBank = CMainGameComponent::Get()->GetTextBank();
    long difference = values.total_item_armor_value_final - values.total_cur_armor_value_final;
    text += textBank.GetTextBySymbol("TEXT_GUI_MENU_ARMOUR_RATING");
    text += CWideString(L" ");
    text += GFIntToWideString(values.total_item_armor_value_final);
    text += CWideString(L" (");
    if (difference > 0)
        text += CWideString(L"+");
    text += GFIntToWideString(difference);
    text += CWideString(L")");
}
