// CTCInventoryStats::GetFatnessAsString(float fatness) -- the localised stats label for the hero's fatness band: TEXT_GUI_STATS_LABEL_FATNESS1 below 0.2, FATNESS2 / 3 / 4 for [0.2, 0.4) / [0.4, 0.6) / [0.6, 0.8), FATNESS5 from 0.8, and the literal L"Unbelievably Fat" when no band matches (a NaN)
// Leaf: GetFatnessAsString
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005CC862, 331 bytes (CWideString by value: hidden pointer at [ebp+8],
//   `ret 8`). Frame-pointer size TU: optimize("y",off) + ("s",on). `this` is unused.
// Callees: CCharString(const char*, long) @ 0x0099EBF0 and ~CCharString @ 0x0099EAE0, operator+(const CCharString&,
//   const CCharString&) @ 0x0099F570 (`__fastcall`), NGameText::CDataBank::GetTextBySymbol(const CCharString&)
//   @ 0x009C95E0 on g_pMainGameComponent(0x013B86A0)->TextBank (+0x14), CWideString(const wchar_t*) @ 0x0099B6B0.
// Literals: "FATNESS1".."FATNESS5" 0x01254814 / 0x012547F0 / 0x012547D8 / 0x012547C0 / 0x012547B4,
//   "TEXT_GUI_STATS_LABEL_" 0x012547FC, L"Unbelievably Fat" 0x01254790; doubles 0.2 0x01254820, 0.4 0x01248028,
//   0.6 0x012547E8, 0.8 0x012547D0.
// Shape: the landed sibling GetHeroSexualityAsString 0x005CCBBD -- one `return` per band, the first band keeps its own
//   body and the others tail-merge after their `push <literal>`; the bands are `lo <= f && f < hi` on the float
//   promoted to double (the value stays on the x87 stack and each arm pops it).
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <stddef.h>
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
struct CWideString {
    void* PStringData;
    CWideString(const wchar_t* text);                   // 0x0099B6B0
};
struct CDataBank {                                      // NGameText::CDataBank
    CWideString GetTextBySymbol(const CCharString& symbol);   // 0x009C95E0
};
struct CMainGameComponent {
    unsigned char _pad00[0x14];
    CDataBank*    TextBank;                             // +0x14
};
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0

namespace NInventory {
struct CTCInventoryStats {
    CWideString GetFatnessAsString(float fatness);
};
CWideString CTCInventoryStats::GetFatnessAsString(float fatness)
{
    CDataBank* bank = g_pMainGameComponent->TextBank;
    if (fatness < 0.2)
        return bank->GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("FATNESS1"));
    if (fatness >= 0.2 && fatness < 0.4)
        return bank->GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("FATNESS2"));
    if (fatness >= 0.4 && fatness < 0.6)
        return bank->GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("FATNESS3"));
    if (fatness >= 0.6 && fatness < 0.8)
        return bank->GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("FATNESS4"));
    if (fatness >= 0.8)
        return bank->GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("FATNESS5"));
    return CWideString(L"Unbelievably Fat");
}
}