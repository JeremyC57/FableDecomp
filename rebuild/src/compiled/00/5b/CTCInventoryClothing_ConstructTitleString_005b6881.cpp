// CTCInventoryClothing::ConstructTitleString(CWideString& title) -- appends "<localised TOTAL ARMOUR> <armour value>" to the title
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005B6881, length 138 bytes. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0 on
// "TEXT_GUI_MENU_CLOTHING_TOTAL_ARMOUR" @ 0x01253A48, CDataBank::GetTextBySymbol @ 0x009C95E0 (engine globals
// @ 0x013B86A0 +0x14), CWideString::operator+= @ 0x0099B8D0, CWideString(const wchar_t*) @ 0x0099B6B0 on L" " @ 0x0122D820,
// ~CWideString @ 0x0099B510, GetCurrentTotalArmorValue() @ 0x005B4A95, GFIntToWideString(long) @ 0x00405F00 (__fastcall,
// hidden result in ecx, value in edx; manifest "CGuiControlNumber::SetSLongValue" is propagated).
// Shape: three `title += <temporary>` statements; the symbol is a named CCharString in its own block and the text bank a
// named local read AFTER it (retail loads the globals between the string constructor and the argument pushes); the looked-up
// text dies before the symbol; the two later temporaries share the [ebp-8] frame word.
#pragma optimize("y", off)
#pragma optimize("s", on)
typedef unsigned short wchar_t;                    // VC7.1 without /Zc:wchar_t

class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);              // 0x0099EBF0
    ~CCharString();                                               // 0x0099EAE0
};
class CWideString { public:
    void* m_pStringData;
    CWideString(const wchar_t* text);                             // 0x0099B6B0
    ~CWideString();                                               // 0x0099B510
    const CWideString& operator+=(const CWideString& other);      // 0x0099B8D0
};
CWideString __fastcall GFIntToWideString(long value);               // 0x00405F00
namespace NGameText {
class CDataBank { public:
    const CWideString GetTextBySymbol(const CCharString& symbol) const;       // 0x009C95E0
};
}
using NGameText::CDataBank;
struct CEngineGlobals {
    unsigned char _pad[0x14];
    CDataBank*    m_pTextBank;                                    // +0x14
};
extern CEngineGlobals* g_pEngineGlobals;                          // 0x013B86A0

struct CTCInventoryClothing_Retail {
    void* __vftable;
    long GetCurrentTotalArmorValue();                             // 0x005B4A95
    void ConstructTitleString(CWideString& title);
};
void CTCInventoryClothing_Retail::ConstructTitleString(CWideString& title)
{
    {
        CCharString symbol("TEXT_GUI_MENU_CLOTHING_TOTAL_ARMOUR");
        CDataBank* textBank = g_pEngineGlobals->m_pTextBank;
        title += textBank->GetTextBySymbol(symbol);
    }
    title += CWideString(L" ");
    title += GFIntToWideString(GetCurrentTotalArmorValue());
}