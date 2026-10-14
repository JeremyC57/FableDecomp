// CGameScriptThing::UpdateThingAttachment() -- when the intelligent pointer at +0xc holds no thing, or a dead one (+0x91 bit 0), look the thing up again by the 64-bit UID at +0x18 through the world's thing manager (world at +0x44, slot 3; 0x0051ED80) and re-attach to it (InitFromThing @ 0x008CFFE0) when found
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D0200, length 80 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 twice (on +0xc), CWorld virtual slot 3 (+0xc), CThingManager::GetThingWithUID @ 0x0051ED80 (labelled "CStopWatch::GetTicks",
// propagated; the UID's two words pushed first), CGameScriptThing::InitFromThing @ 0x008CFFE0. Shape: the pointer address in edi, `this` in esi. Virtual (the mangled name).
// Leaf: ?UpdateThingAttachment@CGameScriptThing@@UAEXXZ
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
    long GetCurrentStateGroupType();               // 0x006D5AE0 (the label "_Cons_val<...>" is propagated)
};
struct CIntelligentPointer_Retail {                // 8 bytes: vptr + the pointee link
    virtual void v00();
    void* m_pItem;
    CThing_Retail* GetPItem();                     // 0x00A01B50 (CBaseIntelligentPointer::GetPItem)
};
struct CThingManager_Retail {
    CThing_Retail* GetThingWithUID(__int64 uid);   // 0x0051ED80
};
struct CWorld {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual CThingManager_Retail* GetThingManager();   // slot 3 (+0x0c)
};

#pragma pack(push, 4)
struct CGameScriptThing {                          // packed to 4: the 64-bit UID must not push the members past the vptr
    virtual void v00();
    unsigned char             _pad04[8];
    CIntelligentPointer_Retail m_thing;            // +0x0c
    unsigned char             _pad14[4];
    __int64                   m_uid;               // +0x18
    unsigned char             _pad20[0x24];
    CWorld*                   m_pWorld;            // +0x44
    void InitFromThing(CThing_Retail* thing);      // 0x008CFFE0
    virtual void UpdateThingAttachment();
};
#pragma pack(pop)

void CGameScriptThing::UpdateThingAttachment()
{
    if (m_thing.GetPItem() == 0 || m_thing.GetPItem()->m_dead) {
        CThing_Retail* found = m_pWorld->GetThingManager()->GetThingWithUID(m_uid);
        if (found)
            InitFromThing(found);
    }
}
