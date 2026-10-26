// CGameScriptThing::IsEqualTo(const CScriptThing& other) const -- whether the other script thing's intelligent pointer (+0xc) and this one's resolve to the same thing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D46A0, length 48 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 twice (the other's first, kept in ebx). Shape: the equality materialised with `sub ; neg ; sbb ; inc` into bl, `ret 4`.
// Virtual const (the mangled name).
// Leaf: ?IsEqualTo@CGameScriptThing@@UBE_NABVCScriptThing@@@Z
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
    long GetCurrentStateGroupType();               // 0x006D5AE0 (the label "_Cons_val<...>" is propagated)
};
struct CIntelligentPointer_Retail {                // 8 bytes: vptr + the pointee link
    virtual void v00();
    void* m_pItem;
    CThing_Retail* GetPItem();                     // 0x00A01B50 (CBaseIntelligentPointer::GetPItem)
};
class CScriptThing { public:
    virtual void v00();
    unsigned char             _pad04[8];
    CIntelligentPointer_Retail m_thing;            // +0x0c
};

class CGameScriptThing : public CScriptThing { public:
    virtual bool IsEqualTo(const CScriptThing& other) const;
};

bool CGameScriptThing::IsEqualTo(const CScriptThing& other) const
{
    return const_cast<CScriptThing&>(other).m_thing.GetPItem() == const_cast<CGameScriptThing*>(this)->m_thing.GetPItem();
}