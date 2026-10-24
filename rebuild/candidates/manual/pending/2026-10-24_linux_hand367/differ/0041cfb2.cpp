// JustGetTheFrikkingText2(const CCharString& symbol) -- localised text for a symbol: the game's text bank first, then the new front end's, else the symbol itself widened
// Grade: DIFFER(139v138) -- lane gap: LTCG custom calling convention. Retail receives the hidden CWideString result
// pointer in EDI (callers do `lea edi,[ebp+8] ; push <symbol> ; call`, 0x0041DC33) and the symbol on the stack (`ret 4`);
// VC7.1 without /GL can only emit the standard `__fastcall` (result in ecx, symbol in edx). ltcg_grade.py cannot grade it
// either, because it always exports the target, which forces the standard convention.
// Leaf: ?JustGetTheFrikkingText2@@YI?AVCWideString@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0041CFB2, length 138 bytes, `ret 4` after `leave`. Name: Ego_r.pdb / FableWin.pdb
// `?JustGetTheFrikkingText2@@YI?AVCWideString@@ABVCCharString@@@Z` (a global `__fastcall` function in the NUISystem::CManager
// TU; the catalog's "GetTextBySymbol" names its callee). Single caller 0x0041DC36 (a CManager member).
// Body: with g_pMainGameComponent (0x013B86A0), its PTextBank (+0x14)->GetTextBySymbol(symbol) (0x009C95E0, hidden result in
// the [ebp-4] temporary); a text other than the empty string (0x013BCA24, CWideString::operator!= 0x0099B0F0) is copied
// into the result (0x0099B720); else the same through g_pNewFrontendGameComponent (0x013B871C) m_pTextBank (+0x60); when
// neither bank knows the symbol the result is CWideString::CreateFromCharString(symbol) (0x0099B6A0, `__fastcall`).
// Shape: frame-pointer size TU (`optimize("s", on)` + `("y", off)`); the empty-string address is hoisted into esi; the two
// lookups share one temporary slot and the copy-out block (the second `!=` jumps into the first's copy).
#pragma optimize("s", on)
#pragma optimize("y", off)
class CCharString {
public:
    void* PStringData;
};
class CWideString {
public:
    CWideString(const CWideString& other);               // 0x0099B720
    ~CWideString();                                      // 0x0099B510
    bool operator!=(const CWideString& other) const;     // 0x0099B0F0
    static CWideString __fastcall CreateFromCharString(const CCharString& text);   // 0x0099B6A0
private:
    void* PStringData;
};
extern CWideString g_emptyWideString;                    // 0x013BCA24
namespace NGameText {
class CDataBank {
public:
    const CWideString GetTextBySymbol(const CCharString& symbol) const;   // 0x009C95E0
};
}
class CMainGameComponent {
public:
    unsigned char         _pad00[0x14];
    NGameText::CDataBank* PTextBank;                     // +0x14 CCountedPointer<NGameText::CDataBank>::Data
};
extern CMainGameComponent* g_pMainGameComponent;         // 0x013B86A0
class CNewFrontendGameComponent {
public:
    unsigned char         _pad00[0x60];
    NGameText::CDataBank* m_pTextBank;                   // +0x60 CCountedPointer<NGameText::CDataBank>::Data
};
extern CNewFrontendGameComponent* g_pNewFrontendGameComponent;   // 0x013B871C

CWideString __fastcall JustGetTheFrikkingText2(const CCharString& symbol)
{
    if (g_pMainGameComponent)
    {
        CWideString text = g_pMainGameComponent->PTextBank->GetTextBySymbol(symbol);
        if (text != g_emptyWideString)
            return text;
    }
    if (g_pNewFrontendGameComponent)
    {
        CWideString text = g_pNewFrontendGameComponent->m_pTextBank->GetTextBySymbol(symbol);
        if (text != g_emptyWideString)
            return text;
    }
    return CWideString::CreateFromCharString(symbol);
}
