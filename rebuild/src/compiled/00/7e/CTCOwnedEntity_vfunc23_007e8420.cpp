// CTCOwnedEntity::vfunc_23() const -- true while the owner is set and alive (its dead bit, +0x91 bit 0, clear)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E8420, length 27 bytes (byte-identical twin: vfunc_19 @ 0x007E8440).
// Callees: CBaseIntelligentPointer::GetPItem() const @ 0x00A01B50 (once, on POwner at +0x0d: `add ecx,0Dh`).
// Retail CTCOwnedEntity (0x20 bytes, PDB): SwitchableNavigationTCAdded +0x0c, POwner (CIntelligentPointer<CThing>,
// 8 bytes: vptr + item) at the unaligned +0x0d, OwnerUID (unsigned __int64) +0x18 -- modelled under pack(1).
// Shape: the pointer fetched once into a local, `if (p && !dead) return true; return false;` gives `mov al,1` and the
// shared `xor al,al` tail.
class CThing { public:
    unsigned char _pad00[0x91];
    unsigned char m_flags91;                        // +0x91 bit 0: dead
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

#pragma pack(push, 1)
struct CTCBase_007e8420 {
    virtual void v00();
    CThing*       ParentThing;                      // +0x04
    unsigned char _pad08[4];
};

class CTCOwnedEntity : public CTCBase_007e8420 { public:
    bool                    SwitchableNavigationTCAdded;   // +0x0c
    CIntelligentPointer<CThing> POwner;                 // +0x0d CIntelligentPointer<CThing>
    unsigned char           _pad15[3];
    unsigned __int64        OwnerUID;               // +0x18
    virtual bool vfunc_23() const;
};
#pragma pack(pop)

bool CTCOwnedEntity::vfunc_23() const
{
    CThing* owner = POwner.GetPItem();
    if (owner && !(owner->m_flags91 & 1))
        return true;
    return false;
}