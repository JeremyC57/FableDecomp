// CGameScriptThing::SetDataString(const CCharString& text) -- assign the text to the string at +0x24 (CCharString::operator= @ 0x0099EFB0) and, when the intelligent pointer at +0xc holds a live thing, to the thing's data string at +0x78
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D0350, length 80 bytes.
// Callees: CCharString::operator= @ 0x0099EFB0 twice, CBaseIntelligentPointer::GetPItem @ 0x00A01B50 three times (on +0xc; re-fetched for every access). Shape: the text in edi,
// the pointer address formed with `add esi,0Ch`, `ret 4`.
struct CTCGraphicAppearance_Retail;
class CCharString { public:
    void* m_pData;
    CCharString(const CCharString& other);         // 0x0099EC30
    CCharString(const char* text, long length);    // 0x0099EBF0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
struct CThing_Retail {
    unsigned char  _pad00[0x10];
    unsigned char  m_type;                         // +0x10 (1 = the player creature)
    unsigned char  _pad11[0x53];
    CTCGraphicAppearance_Retail* m_pAppearance;    // +0x64
    unsigned char  _pad68[4];
    unsigned char  m_flags;                        // +0x6c
    unsigned char  _pad6d[0xb];
    CCharString    m_dataString;                   // +0x78
    unsigned char  _pad7c[0x15];
    unsigned char  m_dead : 1;                     // +0x91 bit 0
    unsigned char  m_91_1 : 7;
    void SetToKillOnLevelUnload(long kill);        // 0x004C9C40
    void SetDataString(const CCharString& text) { m_dataString = text; }
    long GetCurrentStateGroupType();               // 0x006D5AE0 (the label "_Cons_val<...>" is propagated)
};
struct CIntelligentPointer_Retail {                // 8 bytes: vptr + the pointee link
    virtual void v00();
    void* m_pItem;
    CThing_Retail* GetPItem();                     // 0x00A01B50 (CBaseIntelligentPointer::GetPItem)
};

class CGameScriptThing { public:
    unsigned char             _pad[0xc];
    CIntelligentPointer_Retail m_thing;            // +0x0c
    unsigned char             _pad14[0x10];
    CCharString               m_dataString;        // +0x24
    void SetDataString(const CCharString& text);
};

void CGameScriptThing::SetDataString(const CCharString& text)
{
    m_dataString = text;
    if (m_thing.GetPItem()) {
        if (!m_thing.GetPItem()->m_dead)
            m_thing.GetPItem()->SetDataString(text);
    }
}