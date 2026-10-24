// CTCInventoryStats::GetXString() -- static: the localised TEXT_GUI_MENU_XSTRING text, round-tripped through a narrow string and with its underscores replaced by spaces (two passes)
// Leaf: ?GetXString@CTCInventoryStats@@SI
// Retail: Fable.exe @ 0x005CC772, 240 bytes, static __fastcall (hidden CWideString result in ecx), `ret`. Ego_r.pdb
// `?GetXString@CTCInventoryStats@@SI?AVCWideString@@XZ`. 105 callers (the inventory screens' "x" count separator).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, ~CCharString @ 0x0099EAE0,
// NGameText::CDataBank::GetTextBySymbol(const CCharString&) @ 0x009C95E0 on g_pMainGameComponent (0x013B86A0)->TextBank
// (+0x14), CWideString::ToCharString() const @ 0x0099B2C0, CCharString::ToWideString() const @ 0x0099E480,
// CWideString::operator=(const CWideString&) @ 0x0099B7D0, CWideString::CWideString(const wchar_t*) @ 0x0099B6B0,
// CWideString::Replace(const CWideString&, const CWideString&) @ 0x0099C1E0, CWideString(const CWideString&) @ 0x0099B720,
// ~CWideString @ 0x0099B510. Literals: "TEXT_GUI_MENU_XSTRING" 0x01254778, L"_" 0x0122DE7C, L" " 0x0122D820.
// Shape: frame-pointer size TU; the symbol and the Replace arguments are IMPLICIT conversions (const char* ->
// CCharString, const wchar_t* -> CWideString): VC builds every conversion temporary first and then pushes its address
// with a fresh `lea` (explicit `CWideString(L"_")` temporaries push the constructor's eax interleaved and reorder the
// frame: 225v240); the symbol's word is reused by the Replace temporaries (built right to left, destroyed in reverse);
// the two wide literals are CSE'd into esi/edi; the result is a copy of the local (no NRVO), then the narrow copy and
// the local are destroyed.
// Label: the catalog's GetXString is right (class added).
#pragma optimize("y", off)
#pragma optimize("s", on)
typedef unsigned short wchar_t;                         // VC7.1 without /Zc:wchar_t (no header in this TU)
class CWideString;
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    CWideString ToWideString() const;                   // 0x0099E480
};
class CWideString {
public:
    void* PStringData;
    CWideString(const wchar_t* text);                   // 0x0099B6B0
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    CCharString ToCharString() const;                   // 0x0099B2C0
    void Replace(const CWideString& from, const CWideString& to);   // 0x0099C1E0
};
namespace NGameText {
class CDataBank {
public:
    CWideString GetTextBySymbol(const CCharString& symbol);   // 0x009C95E0
};
}
struct CMainGameComponent {
    unsigned char         _pad00[0x14];
    NGameText::CDataBank* TextBank;                     // +0x14
};
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0
class CTCInventoryStats {
public:
    static CWideString __fastcall GetXString();
};
CWideString __fastcall CTCInventoryStats::GetXString()
{
    CWideString text = g_pMainGameComponent->TextBank->GetTextBySymbol("TEXT_GUI_MENU_XSTRING");
    CCharString narrow = text.ToCharString();
    text = narrow.ToWideString();
    text.Replace(L"_", L" ");
    text.Replace(L"_", L" ");
    return text;
}