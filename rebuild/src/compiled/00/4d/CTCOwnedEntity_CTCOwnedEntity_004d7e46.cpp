// CTCOwnedEntity::CTCOwnedEntity(CThing*) -- CTCBase(owner), vtable, SwitchableNavigationTCAdded = false, an empty POwner, OwnerUID = 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D7E46, length 54 bytes (vtable 0x0123EB4C).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10.
// Retail layout (PDB): SwitchableNavigationTCAdded +0x0c, POwner (CIntelligentPointer<CThing>, vtable 0x01238C6C) at the
// unaligned +0x0d (pack(1)), OwnerUID (unsigned __int64) +0x18.
// Shape: the size-optimised constructor module (`push dword ptr [esp+0Ch]`, `and dword ptr [..],0`); POwner's inline default
// constructor = the out-of-line base constructor then its vtable store (no SetPItem), the init-list byte store scheduled
// before that call.
#pragma optimize("s", on)
class CThing;

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
};

template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer() {}
    virtual void v00();
};

#pragma pack(push, 1)
struct CTCBase_004d7e46 {
    virtual void v00();
    CThing*       ParentThing;                      // +0x04
    unsigned char _pad08[4];
    CTCBase_004d7e46(CThing* owner);                // 0x00686800
};

class CTCOwnedEntity : public CTCBase_004d7e46 { public:
    bool                        SwitchableNavigationTCAdded;   // +0x0c
    CIntelligentPointer<CThing> POwner;             // +0x0d
    unsigned char               _pad15[3];
    unsigned __int64            OwnerUID;           // +0x18
    CTCOwnedEntity(CThing* owner);
    virtual void v00();
};
#pragma pack(pop)

CTCOwnedEntity::CTCOwnedEntity(CThing* owner)
    : CTCBase_004d7e46(owner), SwitchableNavigationTCAdded(false), OwnerUID(0)
{
}