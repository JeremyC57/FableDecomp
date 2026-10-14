// CGameScriptInterface::ShowOnScreenMessage(const CCharString& text, float time) const -- set the subtitle renderer's text (embedded at +0x18 of the game UI singleton @ 0x013B8790, 0x00492ED1) with the main component's font bank (+0x08 -> +0x1614c), the text, colour 0, the time, false and true
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00892780, length 43 bytes.
// Callees: CSubtitleRenderer::SetText(CFontBank*, const CCharString&, unsigned long, float, bool, bool) @ 0x00492ED1. Shape: no frame, the font bank loaded between the pushes, `add ecx,18h` for the embedded renderer, `ret 8`.
// Leaf: ShowOnScreenMessage
struct CDisplayEngine_Retail;
struct CPlayerManager_Retail;
struct CMainGameComponent_Retail;
struct CDefinitionManager_Retail;
struct CWorld;
struct CCharString;
struct CScriptThing;
struct CDefPointer_Retail;
typedef unsigned short wchar_t;                // VC7.1 without /Zc:wchar_t: the type of L""
struct CCharStringData_Retail { const char* m_pData; unsigned long m_length; };   // CBasicString<char>: PData, StringLength
struct CWideString {
    void* PStringData;
    CWideString(const CWideString& other);         // 0x0099B720
    ~CWideString();                                // 0x0099B510
    bool operator!=(const wchar_t* text) const;    // 0x0099B150
};
struct CCharString {
    CCharStringData_Retail* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    CWideString ToWideString() const;               // 0x0099E480
    const char* GetString() const { return m_pData == 0 ? "" : m_pData->m_pData; }   // inline: "" @ 0x0129AAF4 without a data block (the null case falls through)
    unsigned long GetLength() const { return m_pData == 0 ? 0 : m_pData->m_length; }
};
struct CThing_Retail;
struct CFontBank_Retail;
struct CRGBColour;
struct CSubtitleRenderer_Retail {               // embedded at CGameUI+0x18 (`add ecx,18h`)
    void SetText(CFontBank_Retail* font, const CCharString& text, unsigned long colour, float time, bool a, bool b);   // 0x00492ED1
    void SetText(CFontBank_Retail* font, const CCharString& text, const CCharString& name, const CCharString& a, const CCharString& b, unsigned long colour, float time, bool c, bool d);   // 0x00492FB4
};
struct CMiniMapDisplay_Retail {
    void AddMarker(CThing_Retail* thing, long id);         // 0x0082A890 (CMiniMapDisplay::AddMarker)
    void SetMarkerGraphic(CThing_Retail* thing, long id);  // 0x0082A340 (CMiniMapDisplay::SetMarkerGraphic)
    void RemoveMarker(CThing_Retail* thing);               // 0x0082A380 (the label is propagated)
};
struct CDrawQuestInfo_Retail {
    long AddBarHealth(CThing_Retail* thing, const CRGBColour& colour, const CCharString& name, float value);   // 0x0064F885 (CDrawQuestInfo::AddBarHealth)
};
struct CGameUI_Retail {                          // the game UI singleton behind the pointer at 0x013B8790
    unsigned char            _pad00[0x18];
    CSubtitleRenderer_Retail m_subtitles;        // +0x18
    unsigned char            _pad19[0x1af];
    CDrawQuestInfo_Retail*   m_pQuestInfo;       // +0x1c8
    CDrawQuestInfo_Retail* GetQuestInfo() { return m_pQuestInfo; }
    CMiniMapDisplay_Retail* GetMiniMapDisplay(); // 0x00437CE0 (+0x160 -> +0x28; the label "CVertexBufferWin32::DoSizeof" is propagated)
    void ShowGuildMasterMessage(const CCharString& text, long id, bool flag);   // 0x00437800 (the label is propagated)
    void ShowOnScreenMessage(const CWideString& text, long a, long b, long c);   // 0x0043A2A0 (labelled ShowOnScreenMessage_FormatDisplay)
};
extern CGameUI_Retail* g_pGameUI_013b8790;      // 0x013B8790
struct CMainGameComponent_Retail {
    unsigned char    _pad[0x1614c];
    CFontBank_Retail* m_pFontBank;               // +0x1614c
};
struct CGameScriptInterface {
    virtual void v000();
    CWorld*                     World;              // +0x04 (engine/CGameScriptInterface.h)
    CMainGameComponent_Retail*  Component;          // +0x08
    CDisplayEngine_Retail*      DisplayEngine;      // +0x0c
    CDefinitionManager_Retail*  DefinitionManager;  // +0x10
    CPlayerManager_Retail*      PlayerManager;      // +0x14
    long GetTextIDFromSymbol(const CCharString& symbol) const;   // 0x006E77F0 (labelled LTextSystem::GetIDFromSymbol; `this` is unused)
    long GetCurrentWorldFrameForMessages() const;   // 0x006E7510 (labelled GetMaxWorldFrameForMessages)
    long GetMaxWorldFrameForMessages() const;       // 0x006E7530
    void ShowOnScreenMessage(const CCharString& text, float time) const;
};

void CGameScriptInterface::ShowOnScreenMessage(const CCharString& text, float time) const
{
    g_pGameUI_013b8790->m_subtitles.SetText(Component->m_pFontBank, text, 0, time, false, true);
}
