// CTCInventoryClothing::GetItemDescriptionNoArmourRating(CWideString& out, long defIndex) -- append a clothing item's stat block to `out`: the suit-parts count when the item is part of a suit, then attractiveness, scariness and alignment, each as "<label> [+]<item value> ([+]<change>)" with the change against what the hero wears now, and finally the item's description text
// Leaf: GetItemDescriptionNoArmourRating@CTCInventoryClothing
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B62FE, 1129 bytes, `ret 8`. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: ArgPasser::ArgPasser 0x005B336A, CTCInventoryClothing::CalculateArmorValues(ArgPasser&, long) 0x005B4EBA,
//   CDataBank::GetTextBySymbol 0x009C95E0 (text bank at g_pMainGameComponent 0x013B86A0 +0x14), IntToWideString 0x00405F00,
//   GetItemDefDescription 0x005D9630 (both `__fastcall`, CWideString by hidden pointer), CWideString(const wchar*) 0x0099B6B0 /
//   += 0x0099B8D0 / ~ 0x0099B510, CCharString 0x0099EBF0 / ~ 0x0099EAE0, __ftol2 0x00BFEA70 (the float-to-long casts).
// Shape: each piece is `out += <temporary>` (the temporary built into the dead `out` / defIndex argument slots, `out`
//   itself kept in esi); the change is ONE local re-assigned per stat, computed before each label (its first value
//   lives in ebx until VC re-uses ebx for the L")\n" literal, the later ones in the dead `out` slot); `x > 0.0f` is `test ah,41h ; jne`.
// Layout: ArgPasser (Ego_r.pdb, 0x30 bytes): is_suit, num/max_suit_parts, item/cur armour, attractiveness, scariness,
//   goodstrength pairs, total_price. engine/CTCInventoryClothing.h is the PDB layout and no member is read here, so `this`
//   is a method-only local model.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CWideString { public:
    void* PStringData;
    CWideString(const unsigned short* text);            // 0x0099B6B0
    ~CWideString();                                     // 0x0099B510
    const CWideString& operator+=(const CWideString& other);   // 0x0099B8D0
};
CWideString __fastcall IntToWideString(long value);     // 0x00405F00
CWideString __fastcall GetItemDefDescription(long defIndex);   // 0x005D9630
namespace NGameText {
class CDataBank { public:
    const CWideString GetTextBySymbol(const CCharString& symbol) const;   // 0x009C95E0
};
}
using NGameText::CDataBank;
class CMainGameComponent { public:
    unsigned char _pad00[0x14];
    CDataBank*    TextBank;                             // +0x14
};
extern CMainGameComponent* g_pMainGameComponent;      // 0x013B86A0
struct ArgPasser {                                      // Ego_r.pdb, 0x30 bytes
    bool  is_suit;                                      // +0x00
    long  num_suit_parts;                               // +0x04
    long  max_suit_parts;                               // +0x08
    long  total_item_armor_value_final;                 // +0x0c
    long  total_cur_armor_value_final;                  // +0x10
    float total_item_attractiveness_value_final;        // +0x14
    float total_cur_attractiveness_value_final;         // +0x18
    float total_item_scariness_value_final;             // +0x1c
    float total_cur_scariness_value_final;              // +0x20
    float total_item_goodstrength_value_final;          // +0x24
    float total_cur_goodstrength_value_final;           // +0x28
    long  total_price;                                  // +0x2c
    ArgPasser();                                        // 0x005B336A
};
class CTCInventoryClothing { public:
    void CalculateArmorValues(ArgPasser& values, long defIndex);   // 0x005B4EBA
    void GetItemDescriptionNoArmourRating(CWideString& out, long defIndex);
};
void CTCInventoryClothing::GetItemDescriptionNoArmourRating(CWideString& out, long defIndex)
{
    ArgPasser values;
    CalculateArmorValues(values, defIndex);
    CDataBank* bank = g_pMainGameComponent->TextBank;
    if (values.is_suit) {
        out += bank->GetTextBySymbol("TEXT_GUI_MENU_PARTS_ACQUIRED");
        out += IntToWideString(values.num_suit_parts);
        out += CWideString(L"/");
        out += IntToWideString(values.max_suit_parts);
        out += CWideString(L"\n");
    }
    long change = (long)(values.total_item_attractiveness_value_final - values.total_cur_attractiveness_value_final);
    out += bank->GetTextBySymbol("TEXT_GUI_MENU_ATTRACTIVENESS");
    out += CWideString(L" ");
    if (values.total_item_attractiveness_value_final > 0.0f)
        out += CWideString(L"+");
    out += IntToWideString((long)values.total_item_attractiveness_value_final);
    out += CWideString(L" (");
    if (change > 0)
        out += CWideString(L"+");
    out += IntToWideString(change);
    out += CWideString(L")\n");
    change = (long)(values.total_item_scariness_value_final - values.total_cur_scariness_value_final);
    out += bank->GetTextBySymbol("TEXT_GUI_MENU_SCARINESS");
    out += CWideString(L" ");
    if (values.total_item_scariness_value_final > 0.0f)
        out += CWideString(L"+");
    out += IntToWideString((long)values.total_item_scariness_value_final);
    out += CWideString(L" (");
    if (change > 0)
        out += CWideString(L"+");
    out += IntToWideString(change);
    out += CWideString(L")\n");
    change = (long)(values.total_item_goodstrength_value_final - values.total_cur_goodstrength_value_final);
    out += bank->GetTextBySymbol("TEXT_GUI_MENU_ALIGNMENT_MODIFIER");
    out += CWideString(L" ");
    if (values.total_item_goodstrength_value_final > 0.0f)
        out += CWideString(L"+");
    out += IntToWideString((long)values.total_item_goodstrength_value_final);
    out += CWideString(L" (");
    if (change > 0)
        out += CWideString(L"+");
    out += IntToWideString(change);
    out += CWideString(L")\n");
    out += GetItemDefDescription(defIndex);
}