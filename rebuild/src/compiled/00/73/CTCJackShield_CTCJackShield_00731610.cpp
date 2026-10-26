// CTCJackShield::CTCJackShield(CThing* owner) -- the shield component: CTCBase over the owner, its emitter intelligent pointer cleared
// Leaf: ??0CTCJackShield
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00731610, length 103 bytes (+ int3 padding), `ret 4`. Vtable 0x01266EFC.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10 (member and
// temporary), CBaseIntelligentPointer::SetPItem @ 0x00A01B90, GetPItem @ 0x00A01B50, ~CBaseIntelligentPointer @ 0x00A01C10.
// Shape: PEmitter is default-constructed (base ctor + the CIntelligentPointer<CThing> vtable 0x01238C6C) and then assigned a
// null CIntelligentPointer<CThing>(0) temporary in the body (`operator=` is SetPItem(other.GetPItem())); the temporary's class
// has no user dtor, so only the base dtor runs (no vtable re-store).
// Layout: Ego_r.pdb CTCJackShield (0x14 bytes): CTCBase (vptr, ParentThing, flags) then PEmitter +0x0C.
class CThing;
class CBaseObject;
struct CBaseIntelligentPointer {            // 8 bytes: vptr + the pointed-to item
    virtual void v00();
    CBaseObject* m_pItem;                   // +4
    CBaseIntelligentPointer();              // 0x00A01B10
    ~CBaseIntelligentPointer();             // 0x00A01C10
    void SetPItem(CBaseObject* item);       // 0x00A01B90
    CBaseObject* GetPItem() const;          // 0x00A01B50
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C for T = CThing
    virtual void v00();
    CIntelligentPointer() {}
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

class CTCBase { public:
    virtual void v00();
    CThing*       ParentThing;              // +0x04
    bool          ToBeRemoved;              // +0x08
    bool          IsInGlobalUpdateTCs;      // +0x09
    CTCBase(CThing* owner);                 // 0x00686800
};
class CTCJackShield : public CTCBase { public:            // vtable 0x01266EFC
    CIntelligentPointer<CThing> PEmitter;   // +0x0c
    virtual void v00();
    CTCJackShield(CThing* owner);
};

CTCJackShield::CTCJackShield(CThing* owner)
    : CTCBase(owner)
{
    PEmitter = CIntelligentPointer<CThing>(0);
}