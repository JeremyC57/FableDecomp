// CGameScriptThing::SetToKillOnLevelUnload_0066add0(long kill) -- while the global game state (0x013B8394) exists, when the intelligent pointer at +0xc holds a live thing (not dead: +0x91 bit 0), forward the flag to it (KillOnLevelUnload_008368d0 @ 0x008368D0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0066ADD0, length 64 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 three times (on +0xc; the pointer re-fetched for every access), KillOnLevelUnload_008368d0 @ 0x008368D0 (on the thing, the flag).
// Shape: the intelligent pointer's address kept in esi, `ret 4`. The chunk label "CGameScriptThing::SetToKillOnLevelUnload" is propagated: 0x0066ADD0 is a twin in another module.
extern void* g_pGameState_013b8394;               // 0x013B8394
struct CTCGraphicAppearance_Retail;
struct CCharString {
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
    void KillOnLevelUnload_008368d0(long kill);   // 0x008368D0
    long GetCurrentStateGroupType();               // 0x006D5AE0 (the label "_Cons_val<...>" is propagated)
};
struct CIntelligentPointer_Retail {                // 8 bytes: vptr + the pointee link
    virtual void v00();
    void* m_pItem;
    CThing_Retail* GetPItem();                     // 0x00A01B50 (CBaseIntelligentPointer::GetPItem)
};

struct CGameScriptThing {
    unsigned char             _pad[0xc];
    CIntelligentPointer_Retail m_thing;            // +0x0c
    void SetToKillOnLevelUnload_0066add0(long kill);
};

void CGameScriptThing::SetToKillOnLevelUnload_0066add0(long kill)
{
    if (!g_pGameState_013b8394)
        return;
    if (m_thing.GetPItem()) {
        if (!m_thing.GetPItem()->m_dead)
            m_thing.GetPItem()->KillOnLevelUnload_008368d0(kill);
    }
}
