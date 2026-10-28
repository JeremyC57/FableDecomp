// CGameScriptInterface::GetFormattedString(const CCharString& symbol, CWideStringVector_Retail& args) const -- look the symbol up in the engine text bank (0x013B86A0 -> +0x14, CDataBank::GetTextBySymbol @ 0x009C95E0), substitute the arguments when the text is not empty (ConstructString @ 0x009C7C20) and return the wide string by value
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00891340, length 87 bytes.
// Callees: CDataBank::GetTextBySymbol @ 0x009C95E0 (hidden result pointer), CWideString::operator!=(const wchar_t*) @ 0x0099B150 against L"" @ 0x0122D70C, ConstructString @ 0x009C7C20 (__fastcall: the text, the vector), CWideString::CWideString(const CWideString&) @ 0x0099B720, CWideString::~CWideString @ 0x0099B510. Shape: no frame: the temporary wide string is homed in the dead `symbol` argument slot, `this` is unused, the hidden result pointer read into esi only for the copy, `ret 0Ch`.
// Leaf: GetFormattedString
struct CDisplayEngine_Retail;
struct CPlayerManager_Retail;
struct CMainGameComponent_Retail;
struct CDefinitionManager_Retail;
class CWorld;
class CCharString;
class CScriptThing;
struct CDefPointer_Retail;
typedef unsigned short wchar_t;                // VC7.1 without /Zc:wchar_t: the type of L""
struct CCharStringData_Retail { const char* m_pData; unsigned long m_length; };   // CBasicString<char>: PData, StringLength
class CWideString { public:
    void* PStringData;
    CWideString(const CWideString& other);         // 0x0099B720
    ~CWideString();                                // 0x0099B510
    bool operator!=(const wchar_t* text) const;    // 0x0099B150
};
class CCharString { public:
    CCharStringData_Retail* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    CWideString ToWideString() const;               // 0x0099E480
    const char* GetString() const { return m_pData == 0 ? "" : m_pData->m_pData; }   // inline: "" @ 0x0129AAF4 without a data block (the null case falls through)
    unsigned long GetLength() const { return m_pData == 0 ? 0 : m_pData->m_length; }
};
struct CWideStringVector_Retail;              // std::vector<CWideString>
namespace NGameText {
class CDataBank { public:
    const CWideString GetTextBySymbol(const CCharString& symbol) const;   // 0x009C95E0 (CDataBank::GetTextBySymbol, by value through the hidden result pointer)
    void* LookupText(unsigned long hash);        // 0x009CC410 (labelled CVectorMap::LookupValue)
};
}
using NGameText::CDataBank;
struct CEngineGlobals_Retail {
    unsigned char    _pad[0x14];
    CDataBank* m_pTextBank;               // +0x14
};
extern CEngineGlobals_Retail* g_pEngineGlobals_013b86a0;   // 0x013B86A0
bool __fastcall ConstructString(CWideString* text, CWideStringVector_Retail* args);   // 0x009C7C20
unsigned long __fastcall Calc_004014a0(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0 (CRC)
class CGameScriptInterface { public:
    virtual void v000();
    CWorld*                     World;              // +0x04 (engine/CGameScriptInterface.h)
    CMainGameComponent_Retail*  Component;          // +0x08
    CDisplayEngine_Retail*      DisplayEngine;      // +0x0c
    CDefinitionManager_Retail*  DefinitionManager;  // +0x10
    CPlayerManager_Retail*      PlayerManager;      // +0x14
    long GetTextIDFromSymbol(const CCharString& symbol) const;   // 0x006E77F0 (labelled LTextSystem::GetIDFromSymbol; `this` is unused)
    long GetCurrentWorldFrameForMessages() const;   // 0x006E7510 (labelled GetMaxWorldFrameForMessages)
    long GetMaxWorldFrameForMessages() const;       // 0x006E7530
    CWideString GetFormattedString(const CCharString& symbol, CWideStringVector_Retail& args) const;
};

CWideString CGameScriptInterface::GetFormattedString(const CCharString& symbol, CWideStringVector_Retail& args) const
{
    CWideString text = g_pEngineGlobals_013b86a0->m_pTextBank->GetTextBySymbol(symbol);
    if (text != L"")
        ConstructString(&text, &args);
    return text;
}