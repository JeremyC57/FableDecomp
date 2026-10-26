// CCreatureAction_ShopkeeperPutAway::PerformAction() -- if the item being put away is alive (pointer set, dead bit clear), kill it silently
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008446E0, length 64 bytes. Vtable slot 3 (PerformAction).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (the pointer member at +0xa8, three times: the landed IS_THING_ALIVE idiom re-fetches,
// then the call target), CThing::Kill(bool) @ 0x004C9B80.
// Shape: the member pointer rides in esi (`lea esi,[ecx+0A8h]`); the shape of the landed 0x008445A0 without the range test. Retail
// CCreatureActionBase is 0xa8 bytes (PDB 0xb4), so the pointer (PDB +0xb4) is at +0xa8 and `this` is modelled locally.
class CThing { public:                             // only the dead bit and the call are modelled
    unsigned char _pad[0x91];
    unsigned char m_flags91;                // +0x91 bit 0: dead
    void Kill(bool silent);                 // 0x004C9B80
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

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined, re-fetching GetPItem).
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

class CCreatureAction_ShopkeeperPutAway { public:
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void PerformAction();           // slot 3
    unsigned char _pad04[0xa8 - 4];         // +0x04 (retail CCreatureActionBase body)
    CIntelligentPointer<CThing> PItem;          // +0xa8 CIntelligentPointer<CThing> (PDB +0xb4)
};

void CCreatureAction_ShopkeeperPutAway::PerformAction()
{
    if (IS_THING_ALIVE(PItem))
        PItem.GetPItem()->Kill(true);
}