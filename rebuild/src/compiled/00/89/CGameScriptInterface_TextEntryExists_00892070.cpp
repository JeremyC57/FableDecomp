// CGameScriptInterface::TextEntryExists(const CCharString& symbol) const -- true when the engine text bank (0x013B86A0 -> +0x14) has an entry for the CRC of the symbol (Calc @ 0x004014A0 over the string's data and length, or "" and 0 without a data block; the lookup @ 0x009CC410)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00892070, length 65 bytes.
// Callees: Calc @ 0x004014A0 (__fastcall: seed 0, the data, the length pushed), the bank lookup @ 0x009CC410 (labelled CVectorMap::LookupValue). Shape: the bank kept in esi from the start, the length and the data each through their own inline accessor (two separate null tests on the data block, "" @ 0x0129AAF4), `neg ; sbb ; neg` for the non-null result, `ret 4`.
// Leaf: TextEntryExists
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
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    CWideString ToWideString() const;               // 0x0099E480
    const char* GetString() const { return m_pData == 0 ? "" : m_pData->m_pData; }   // inline: "" @ 0x0129AAF4 without a data block (the null case falls through)
    unsigned long GetLength() const { return m_pData == 0 ? 0 : m_pData->m_length; }
};
struct CWideStringVector_Retail;              // std::vector<CWideString>
struct CDataBank_Retail {
    CWideString GetTextBySymbol(const CCharString& symbol);   // 0x009C95E0 (CDataBank::GetTextBySymbol, by value through the hidden result pointer)
    void* LookupText(unsigned long hash);        // 0x009CC410 (labelled CVectorMap::LookupValue)
};
struct CEngineGlobals_Retail {
    unsigned char    _pad[0x14];
    CDataBank_Retail* m_pTextBank;               // +0x14
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
    bool TextEntryExists(const CCharString& symbol) const;
};

bool CGameScriptInterface::TextEntryExists(const CCharString& symbol) const
{
    CDataBank_Retail* bank = g_pEngineGlobals_013b86a0->m_pTextBank;
    return bank->LookupText(Calc_004014a0(0, symbol.GetString(), symbol.GetLength())) != 0;
}