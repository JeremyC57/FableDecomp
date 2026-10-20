// CTCInventoryStats::GetHeroSexualityAsString(long sexuality) -- the localised stats label for the hero's sexuality: TEXT_GUI_STATS_LABEL_SEX_UNKNOWN for 0, else the label for SEX_STRAIGHT (1), SEX_BI (2), SEX_GAY (3) or NONE
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005CCBBD, length 212 bytes (CWideString by value: hidden pointer at [ebp+8], `ret 8`).
// Frame-pointer size TU: optimize("y",off) + ("s",on). `this` is unused.
// Callees: CCharString(const char*, long) @ 0x0099EBF0 and ~CCharString @ 0x0099EAE0, operator+(const CCharString&, const CCharString&)
// @ 0x0099F570 (`__fastcall`, the result through the hidden pointer in ecx, the left operand in edx), NGameText::CDataBank::GetTextBySymbol
// (const CCharString&) @ 0x009C95E0 (CWideString by value, straight into this function's result) on g_pMainGameComponent(0x013B86A0)->
// TextBank (+0x14). Literals: "SEX_UNKNOWN" 0x01254994, "TEXT_GUI_STATS_LABEL_" 0x012547FC, "SEX_BI" 0x0125498C, "SEX_GAY" 0x01254984,
// "SEX_STRAIGHT" 0x01254974, "NONE" 0x0125496C.
// Shape: one `return` per case (0, 2, 3, 1, else), each building the same sum of two temporaries. The compiler head-merges the
// first constructor's `push -1` above the whole chain and tail-merges everything after the per-arm `push <literal>`, so only the
// unknown case keeps its own body (its temporaries in [ebp-8]/[ebp-4]; the merged arms use the slots the other way round) and
// the concatenation temporary lives in the dead argument slot ([ebp+0Ch]). An if-chain assigning a `const char*` first
// materialises the literal in eax (one byte longer); a switch becomes a jump table.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
struct CWideString {
    void* PStringData;
};
struct CDataBank {                                      // NGameText::CDataBank
    CWideString GetTextBySymbol(const CCharString& symbol);   // 0x009C95E0
};
struct CMainGameComponent {
    unsigned char _pad00[0x14];
    CDataBank*    TextBank;                             // +0x14
};
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0
enum EHeroSexuality { SEX_UNKNOWN_VALUE = 0, SEX_STRAIGHT_VALUE = 1, SEX_BI_VALUE = 2, SEX_GAY_VALUE = 3 };

namespace NInventory {
struct CTCInventoryStats {
    CWideString GetHeroSexualityAsString(long sexuality);
};
CWideString CTCInventoryStats::GetHeroSexualityAsString(long sexuality)
{
    CDataBank* bank = g_pMainGameComponent->TextBank;
    if (sexuality == SEX_UNKNOWN_VALUE)
        return bank->GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("SEX_UNKNOWN"));
    if (sexuality == SEX_BI_VALUE)
        return bank->GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("SEX_BI"));
    if (sexuality == SEX_GAY_VALUE)
        return bank->GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("SEX_GAY"));
    if (sexuality == SEX_STRAIGHT_VALUE)
        return bank->GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("SEX_STRAIGHT"));
    return bank->GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("NONE"));
}
}