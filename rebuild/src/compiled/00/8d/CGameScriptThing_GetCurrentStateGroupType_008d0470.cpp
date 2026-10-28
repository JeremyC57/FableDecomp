// CGameScriptThing::GetCurrentStateGroupType() -- the current state-group type (0x006D5AE0) of the live player creature (+0x10 == 1) held by the intelligent pointer at +0xc; 0 otherwise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D0470, length 64 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 four times (on +0xc), the state-group getter @ 0x006D5AE0 (a tail jump; labelled "_Cons_val<...>", propagated).
// Shape: the pointer address in esi, `xor eax,eax` on the fall-through.
struct CTCGraphicAppearance_Retail;
class CCharString { public:
    void* m_pData;
    CCharString(const CCharString& other);         // 0x0099EC30
    CCharString(const char* text, long length);    // 0x0099EBF0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
class CThing_Retail { public:
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
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CGameScriptThing { public:
    unsigned char             _pad[0xc];
    CIntelligentPointer<CThing_Retail> m_thing;            // +0x0c
    long GetCurrentStateGroupType();
};

long CGameScriptThing::GetCurrentStateGroupType()
{
    if (m_thing.GetPItem()) {
        if (!m_thing.GetPItem()->m_dead) {
            if (m_thing.GetPItem()->m_type == 1)
                return m_thing.GetPItem()->GetCurrentStateGroupType();
        }
    }
    return 0;
}