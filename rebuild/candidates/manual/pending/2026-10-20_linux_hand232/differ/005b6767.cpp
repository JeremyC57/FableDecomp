// Grade: DIFFER(282v282) -- scheduling only: retail `mov eax,[g_pMainGameComponent]` before `mov esi,[ebp-28h]` (the armour difference); ours loads the difference first. With the bank local declared first (probe 005b6767_v3) the TextBank load moves above the `sub` instead.
// CTCInventoryClothing::GetItemDescriptionArmourRating(CWideString* text, long itemDefIndex) -- appends "<Armour rating> N (+D)" for a clothing item: its armour value and the signed difference to what is worn now
// Retail 0x005B6767, 282 bytes, `ret 8`. Frame-pointer size TU: optimize("y", off) + ("s", on).
// Callees: ArgPasser::ArgPasser() 0x005B336A, CalculateArmorValues(ArgPasser*, long) 0x005B4EBA, the main game
// component's (0x013B86A0) text bank (+0x14) CDataBank::GetTextBySymbol(const CCharString&) 0x009C95E0,
// CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CWideString(const wchar_t*) 0x0099B6B0,
// CWideString::operator+= 0x0099B8D0, ~CWideString 0x0099B510, IntToWideString(long) 0x00405F00 (__fastcall, hidden
// result in ecx; manifest "CGuiControlNumber::SetSLongValue" is propagated).
// Shape: the symbol converts implicitly (re-addressed with `lea`); every other piece is an explicit CWideString
// temporary appended and destroyed in turn; all string temporaries live in the dead `itemDefIndex` slot
// ([ebp+0Ch]) except GetTextBySymbol's result ([ebp-4]); the difference and the text bank (a named local) are taken before the text is built.
// Layout: Ego_r.pdb ArgPasser (48 bytes): total_item_armor_value_final +0x0C, total_cur_armor_value_final +0x10.
#pragma optimize("y", off)
#pragma optimize("s", on)

typedef unsigned short wchar_t;                    // VC7.1 without /Zc:wchar_t
struct CCharString {
    void* m_pStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CWideString {
    void* m_pStringData;
    CWideString(const wchar_t* text);                  // 0x0099B6B0
    CWideString(const CWideString& other);
    ~CWideString();                                    // 0x0099B510
    const CWideString& operator+=(const CWideString& other);   // 0x0099B8D0
};
CWideString __fastcall IntToWideString(long value);    // 0x00405F00
struct CDataBank {
    CWideString GetTextBySymbol(const CCharString& symbol);    // 0x009C95E0
};
struct CMainGameComponent {
    unsigned char _pad_0x00[0x14];
    CDataBank* TextBank;                               // +0x14
};
extern CMainGameComponent* g_pMainGameComponent;       // 0x013B86A0

struct ArgPasser {                                     // 48 bytes (Ego_r.pdb)
    bool  is_suit;                                     // +0x00
    long  num_suit_parts;                              // +0x04
    long  max_suit_parts;                              // +0x08
    long  total_item_armor_value_final;                // +0x0c
    long  total_cur_armor_value_final;                 // +0x10
    float total_item_attractiveness_value_final;       // +0x14
    float total_cur_attractiveness_value_final;        // +0x18
    float total_item_scariness_value_final;            // +0x1c
    float total_cur_scariness_value_final;             // +0x20
    float total_item_goodstrength_value_final;         // +0x24
    float total_cur_goodstrength_value_final;          // +0x28
    long  total_price;                                 // +0x2c
    ArgPasser();                                       // 0x005B336A
};

struct CTCInventoryClothing {
    void CalculateArmorValues(ArgPasser* values, long itemDefIndex);   // 0x005B4EBA
    void GetItemDescriptionArmourRating(CWideString* text, long itemDefIndex);
};

void CTCInventoryClothing::GetItemDescriptionArmourRating(CWideString* text, long itemDefIndex)
{
    ArgPasser values;
    CalculateArmorValues(&values, itemDefIndex);
    long difference = values.total_item_armor_value_final - values.total_cur_armor_value_final;
    CDataBank* textBank = g_pMainGameComponent->TextBank;
    *text += textBank->GetTextBySymbol("TEXT_GUI_MENU_ARMOUR_RATING");
    *text += CWideString(L" ");
    *text += IntToWideString(values.total_item_armor_value_final);
    *text += CWideString(L" (");
    if (difference > 0)
        *text += CWideString(L"+");
    *text += IntToWideString(difference);
    *text += CWideString(L")");
}
