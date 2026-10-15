// CTCActionUseBuyOrSteal::CTCActionUseBuyOrSteal(CThing& owner) -- the inlined CTCActionUseBase constructor (CTCBase(owner), no action-use def, Usable), this class's vtable, an empty PPerformer intelligent pointer, not waiting for confirmation, then the component registers itself in the global update list
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072A630, length 74 bytes.
// Leaf: ??0CTCActionUseBuyOrSteal
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10,
// CBaseIntelligentPointer::SetPItem @ 0x00A01B90, CTCBase::AddToGlobalTCs @ 0x00686ED0 (Ego_r.pdb name; the manifest's
// "std::list::push_back" is the list insert it performs). Vtables (rtti-vtables.tsv): 0x0126679C CTCActionUseBuyOrSteal,
// 0x01238C6C CIntelligentPointer. Layout from Ego_r.pdb: CTCActionUseBase = CTCBase (0xc) + CDefPointer<CActionUseDef const>
// PActionUseDef +0x0c + bool Usable +0x10 (sizeof 0x14); CTCActionUseBuyOrSteal adds CIntelligentPointer<CThing> PPerformer
// +0x14 and bool WaitingForConfirmation +0x1c (sizeof 0x20, matching the landed Create factory @ 0x004D2C12).
// Shape: CTCActionUseBase's own out-of-line constructor (0x004DA939) stores its vtable first; inlined here, that store is
// dead under the derived one and only its two member stores remain, ahead of the derived vtable store. The intelligent
// pointer is built by its inline converting constructor (base ctor, its vtable, SetPItem(0)); the bool follows in member
// order and AddToGlobalTCs is the constructor body. engine/CTCBase.h is a flattened POD (no virtuals), so the polymorphic
// CTCBase is modelled locally as the landed CTCBase-derived constructors do.
struct CThing;
struct CBaseObject;

struct CTCBase {                               // engine/CTCBase.h layout, modelled polymorphic
    virtual void v00();
    CThing* ParentThing;                       // +0x04
    bool    ToBeRemoved;                       // +0x08
    bool    IsInGlobalUpdateTCs;               // +0x09
    CTCBase(CThing& owner);                    // 0x00686800
    void AddToGlobalTCs();                     // 0x00686ED0
};

struct CDefPointee {                           // counted def: count at +4, slot 1 destroys
    virtual void v00();
    virtual void Destroy();                    // slot 1 (+4)
    long m_refCount;                           // +0x04
};

struct CDefPointer {                           // CDefPointer<T const>
    CDefPointee* m_pDef;
    CDefPointer() : m_pDef(0) { }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                      // +0x04
    CBaseIntelligentPointer();                 // 0x00A01B10
    void SetPItem(CBaseObject* item);          // 0x00A01B90
};

struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(CBaseObject* item) : CBaseIntelligentPointer() { SetPItem(item); }
};

struct CTCActionUseBase : CTCBase {            // vtable 0x0123F09C (own constructor 0x004DA939)
    CDefPointer PActionUseDef;                 // +0x0c
    bool        Usable;                        // +0x10
    CTCActionUseBase(CThing& owner) : CTCBase(owner), Usable(true) { }
};

struct CTCActionUseBuyOrSteal : CTCActionUseBase {   // vtable 0x0126679C
    CIntelligentPointer PPerformer;            // +0x14
    bool WaitingForConfirmation;               // +0x1c
    virtual void v00();
    CTCActionUseBuyOrSteal(CThing& owner);
};

CTCActionUseBuyOrSteal::CTCActionUseBuyOrSteal(CThing& owner)
    : CTCActionUseBase(owner), PPerformer(0), WaitingForConfirmation(false)
{
    AddToGlobalTCs();
}