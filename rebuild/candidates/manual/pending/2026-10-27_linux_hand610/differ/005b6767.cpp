// CTCInventoryClothing::GetItemDescriptionArmourRating(CWideString& text, long itemDefIndex) const -- appends "<Armour rating> N (+D)" for a clothing item: its armour value and the signed difference to what is worn now
// Grade: DIFFER(282v282) -- wave 25: names PDB-exact (const method, CWideString& text, nested ArgPasser, private const CalculateArmorValues(ArgPasser&), NGameText::CDataBank::GetTextBySymbol const, GFIntToWideString, PTextBank as CCountedPointer); residue unchanged: retail loads g_pMainGameComponent before `mov esi,[ebp-28h]` (ours after). Tried this wave: split difference (`d = item; bank = ...; d -= cur`, moves the bank load between), game-component local between the split (same as parked), PTextBank-> inline in the call (bank load moves after the CCharString ctor, 280v282). Lane gap: two-load scheduling.
// Leaf: ?GetItemDescriptionArmourRating@CTCInventoryClothing@@QBEXAAVCWideString@@J@Z
// Flags: /Zc:wchar_t
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B6767, 282 bytes, `ret 8`. Frame-pointer size TU: optimize("y", off) + ("s", on).
// Identity: Ego_r.pdb ?GetItemDescriptionArmourRating@CTCInventoryClothing@@QBEXAAVCWideString@@J@Z (const, text by reference).
// Callees: CTCInventoryClothing::ArgPasser::ArgPasser() 0x005B336A, CTCInventoryClothing::CalculateArmorValues(ArgPasser&,
//   long) const (private) 0x005B4EBA, NGameText::CDataBank::GetTextBySymbol(const CCharString&) const 0x009C95E0 on the main
//   game component's (0x013B86A0) PTextBank (CCountedPointer, +0x14), CCharString(const char*, long) 0x0099EBF0 /
//   ~CCharString 0x0099EAE0, CWideString(const wchar_t*) 0x0099B6B0, CWideString::operator+=(const CWideString&) 0x0099B8D0,
//   ~CWideString 0x0099B510, GFIntToWideString(long) 0x00405F00 (__fastcall, hidden result).
// Shape: the symbol converts implicitly; every other piece is an explicit CWideString temporary appended and destroyed in
//   turn; the difference and the text bank are taken before the text is built.
// Layout: engine/CTCInventoryClothing.h for `this` (no member read); Ego_r.pdb CTCInventoryClothing::ArgPasser (48 bytes).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define CTCInventoryClothing CTCInventoryClothingLayout
#include "engine/CTCInventoryClothing.h"
#undef CTCInventoryClothing
#include "engine/CCountedPointer.h"

class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
class CWideString {
public:
    void* PStringData;
    CWideString(const wchar_t* text);                  // 0x0099B6B0
    CWideString(const CWideString& other);
    ~CWideString();                                    // 0x0099B510
    const CWideString& operator+=(const CWideString& other);   // 0x0099B8D0
};
CWideString __fastcall GFIntToWideString(long value);  // 0x00405F00
namespace NGameText {
class CDataBank {
public:
    const CWideString GetTextBySymbol(const CCharString& symbol) const;   // 0x009C95E0
};
}
class CMainGameComponent {
public:
    unsigned char _pad00[0x14];
    CCountedPointer<NGameText::CDataBank> PTextBank;  // +0x14
};
extern CMainGameComponent* g_pMainGameComponent;      // 0x013B86A0

class CTCInventoryClothing : public CTCInventoryClothingLayout {
public:
    struct ArgPasser {                                 // Ego_r.pdb, 48 bytes
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
private:
    void CalculateArmorValues(ArgPasser& values, long itemDefIndex) const;   // 0x005B4EBA
public:
    void GetItemDescriptionArmourRating(CWideString& text, long itemDefIndex) const;
};

void CTCInventoryClothing::GetItemDescriptionArmourRating(CWideString& text, long itemDefIndex) const
{
    ArgPasser values;
    CalculateArmorValues(values, itemDefIndex);
    long difference = values.total_item_armor_value_final - values.total_cur_armor_value_final;
    NGameText::CDataBank* textBank = g_pMainGameComponent->PTextBank.Get();
    text += textBank->GetTextBySymbol("TEXT_GUI_MENU_ARMOUR_RATING");
    text += CWideString(L" ");
    text += GFIntToWideString(values.total_item_armor_value_final);
    text += CWideString(L" (");
    if (difference > 0)
        text += CWideString(L"+");
    text += GFIntToWideString(difference);
    text += CWideString(L")");
}
