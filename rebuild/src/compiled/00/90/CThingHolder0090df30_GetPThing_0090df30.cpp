// CThingHolder_0090df30::GetPThing() -- the thing held by the intelligent pointer at +0x48 when it exists and all five low bits of its flags word (+0x6c) are set; else null
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090DF30, length 48 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 three times (on +0x48; the last a tail jump). Shape: the null result kept in edi, the flag test `not eax ; test al,1Fh`.
// The chunk label "CGameScriptThing::GetPThing" is propagated (the pointer sits at +0x48 here; the +0x50 twin is 0x006AF520); named by address.
class CThing_Retail;
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
class CThing_Retail { public:
    unsigned char _pad[0x6c];
    unsigned long m_flags;                         // +0x6c
};

struct CThingHolder_0090df30 {
    unsigned char             _pad[0x48];
    CIntelligentPointer<CThing_Retail> m_thing;            // +0x48
    CThing_Retail* GetPThing();
};

CThing_Retail* CThingHolder_0090df30::GetPThing()
{
    CThing_Retail* result = 0;
    if (m_thing.GetPItem()) {
        if (!(~m_thing.GetPItem()->m_flags & 0x1f))
            result = m_thing.GetPItem();
    }
    return result;
}