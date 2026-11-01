// CTCHeroStats::GetDrinkResistance() const -- the GUI text for the hero's drink resistance: level 1..5 from the drink multiplier, looked up as the text symbol "<prefix><level>"
// Leaf: GetDrinkResistance
// Naming wave 18: the global stand-in g_pEngine (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057A7B9, length 111 bytes, `ret 4` (hidden CWideString result). Size-optimised
// TU (`#pragma optimize("s", on)`: `push 5 ; pop eax`, `xor eax,eax ; inc eax`, frame pointer), as the landed Drink 0x0057A82A.
// Callees: __ftol2 @ 0x00BFEA70, the `__fastcall` GFIntToCharString(long) @ 0x0099F830 and operator+(const char*, const
// CCharString&) @ 0x0099F690 (hidden results in ecx; the prefix literal 0x0124F314 "TEXT_GUI_STATS_LABEL_DRINKERCLASS"), ~CCharString @ 0x0099EAE0,
// NGameText::CDataBank::GetTextBySymbol(const CCharString&) const @ 0x009C95E0 (CWideString by hidden pointer).
// Globals: the engine singleton 0x013B86A0 (+0x14 = the game-text data bank).
// Shape: `(long)((1.0f - DrinkAffectMultiplier) * 5.0f + 1.0f)` clamped to [1, 5]; the number string is a temporary destroyed
// right after the concatenation, the symbol a named local destroyed after the lookup.
// Layout: retail DrinkAffectMultiplier +0x1a4 (PDB +0x1bc; see 0x0057A82A).
#pragma optimize("s", on)
class CCharString { public:
    ~CCharString();                             // 0x0099EAE0
    void* m_pStringData;
};
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690
CCharString __fastcall GFIntToCharString(long value);                          // 0x0099F830
class CWideString { public: void* PStringData; };
namespace NGameText {
class CDataBank {
public:
    const CWideString GetTextBySymbol(const CCharString& symbol) const;   // 0x009C95E0
};
}
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
    unsigned char _pad00[0x14];
    NGameText::CDataBank* PGameTextDataBank;    // +0x14
};
class CThing;
class CTCHeroStats {
public:
    virtual ~CTCHeroStats();
    CThing*       ParentThing;                  // +0x04
    unsigned char _pad08[0x1a4 - 0x08];
    float         DrinkAffectMultiplier;        // +0x1a4
    CWideString GetDrinkResistance() const;
};

CWideString CTCHeroStats::GetDrinkResistance() const
{
    long level = (long)((1.0f - DrinkAffectMultiplier) * 5.0f + 1.0f);
    if (level < 1)
        level = 1;
    if (level > 5)
        level = 5;
    CCharString symbol = "TEXT_GUI_STATS_LABEL_DRINKERCLASS" + GFIntToCharString(level);
    return CMainGameComponent::Get()->PGameTextDataBank->GetTextBySymbol(symbol);
}