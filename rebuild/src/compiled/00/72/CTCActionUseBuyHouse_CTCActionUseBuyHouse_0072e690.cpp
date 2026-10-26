// CTCActionUseBuyHouse::CTCActionUseBuyHouse(CThing*) -- CTCActionUseBase(owner) inline, own vtable, null def, no pending confirmation, join the global update list
// Leaf: ??0CTCActionUseBuyHouse
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072E690, length 47 bytes (+ int3 padding).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800 (manifest "CEngineSubPrimitive2DClockSprite" is a
// propagated name), CTCBase::AddToGlobalTCs() @ 0x00686ED0 (tests IsInGlobalUpdateTCs at +9 and
// links `this` into the global update list -- the counterpart of the landed RemoveFromGlobalTCs
// 0x00686DF0; the manifest's "std::list::push_back" label is a propagated name).
// Own vtable 0x01266A7C = CTCActionUseBuyHouse (rtti-vtables.tsv).
//
// Layout = Ego_r.pdb CTCActionUseBuyHouse (0x20 bytes): CTCActionUseBase 0x14 {CTCBase 0xc,
// PActionUseDef +0x0c, Usable +0x10}, CDefPointer<CBuyHouseDef> PDef +0x14, WaitingForConfirmation +0x18,
// HouseQuestion +0x1c (left uninitialised).
// Shape: this speed-optimised module inlines the CTCActionUseBase constructor (its own out-of-line copy
// is the /Os 0x004DA939): the base's two members are stored BEFORE the derived vtable (the base's
// vtable store is dead and dropped), the derived members after it from one zero register, then the
// body's call. engine/CTCBase.h holds the layout only, so the root is modelled as `CTCBase`.
class CThing;
class CActionUseDef;
class CBuyHouseDef;

class CTCBase { public:                                      // engine/CTCBase.h layout (0xc bytes)
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CTCBase(CThing* thing);                           // 0x00686800
public:
    void AddToGlobalTCs();   // 0x00686ED0
};

template<class T>
class CDefPointer { public:
    const T* m_pDef;
    CDefPointer() : m_pDef(0) { }
};

class CTCActionUseBase : public CTCBase { public:                   // vtable 0x0123F09C
    CDefPointer<CActionUseDef> PActionUseDef;               // +0x0c
    bool                       Usable;                      // +0x10
    virtual void v00();
    CTCActionUseBase(CThing* owner) : CTCBase(owner), Usable(true) { }
};

struct CTCActionUseBuyHouse : CTCActionUseBase {            // vtable 0x01266A7C
    CDefPointer<CBuyHouseDef> PDef;                         // +0x14
    bool                      WaitingForConfirmation;       // +0x18
    long                      HouseQuestion;                // +0x1c
    virtual void v00();
    CTCActionUseBuyHouse(CThing* owner);
};

CTCActionUseBuyHouse::CTCActionUseBuyHouse(CThing* owner)
    : CTCActionUseBase(owner)
    , WaitingForConfirmation(false)
{
    AddToGlobalTCs();
}