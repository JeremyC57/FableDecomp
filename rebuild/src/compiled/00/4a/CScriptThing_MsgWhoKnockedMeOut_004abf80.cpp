// CScriptThing::MsgWhoKnockedMeOut() -- return who knocked the thing out: a copy of the (null) handle itself when it has no implementation, else the implementation's answer
// Retail: Fable.exe (TLC, VC7.1) @ 0x004ABF80, 60 bytes (vtable 0x01238C8C slot 33, `?MsgWhoKnockedMeOut@CScriptThing@@UBE?AV1@XZ`).
// Callees: the implementation's own MsgWhoKnockedMeOut (virtual slot 33 = +0x84); the copy
// constructor 0x004ABE90 is inlined here (vtable store, two word copies, `inc` of the count block).
//
// Shape: a CScriptThing returned by value through the hidden pointer. The pimpl handle forwards every
// virtual to the SAME virtual of the CScriptThing held in PImp.Data; with no implementation it returns
// `*this` (a null handle). Twins at 0x004ABEC0/0x004ABF00/0x004ABF40 differ only in the slot; the
// first of the family (MsgWhoKilledMe 0x004ABE60) calls the copy constructor out of line because the
// inline definition follows it in the retail TU.
// Model: CScriptThing (PDB: 0xc bytes) = CBase (vptr; virtual destructor 0x0099A2E0) +
// CCountedPointer<CScriptThing> PImp {Data +4, Info +8}; slots 1..32 hold their place (names in the
// comments from rtti-vtables.tsv / the landed CScriptThing bodies). The engine header flattens the vptr
// and cannot carry the virtual table, so the class is modelled locally with the header's member names.
// The implementation pointer is read into a local: retail keeps it in eax for the vtable fetch and
// `mov ecx,eax` (re-reading the member reloads `[ecx+4]` into ecx).

struct CCPPointerInfo {
    long m_references;                       // +0 reference count
};

struct CBase {
    virtual ~CBase();                        // 0x0099A2E0
};

struct CScriptThing;

struct CCountedPointer_CScriptThing {
    CScriptThing* Data;                      // +0
    CCPPointerInfo* Info;                    // +4
    CCountedPointer_CScriptThing(const CCountedPointer_CScriptThing& other)
        : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->m_references;
    }
};

struct CScriptThing : CBase {
    CCountedPointer_CScriptThing PImp;       // +0x04
    CScriptThing(const CScriptThing& other) : PImp(other.PImp) {}   // 0x004ABE90 (inlined)
    virtual ~CScriptThing();                 // slot 0
    virtual void v01();  // GetName
    virtual void v02();
    virtual void v03();
    virtual void v04();  // SetDataString
    virtual void v05();
    virtual void v06();  // GetPos
    virtual void v07();
    virtual void v08();  // GetCurrentMapName
    virtual void v09();  // GetHomeMapName
    virtual void v10();  // GetAngleXY
    virtual void v11();  // GetPThing
    virtual void v12();  // GetPThingUniqueID
    virtual void v13();  // GetCurrentStateGroupType
    virtual void v14();  // GetCurrentScriptPriority
    virtual void v15();  // IsSneaking
    virtual void v16();  // IsAwareOfHero
    virtual void v17();  // MsgIsTriggeredBy
    virtual void v18();  // MsgIsKilledBy
    virtual void v19();  // MsgWhoKilledMe
    virtual void v20();  // MsgIsUsedBy
    virtual void v21();  // MsgIsHitBy
    virtual void v22();  // MsgIsHitByWithFlourish
    virtual void v23();  // MsgIsHitByWithDecapitate
    virtual void v24();  // MsgIsHitByWithWeapon
    virtual void v25();  // MsgIsHitByWithProjectileWeapon
    virtual void v26();  // MsgWhoHitMe
    virtual void v27();  // MsgIsTalkedToBy
    virtual void v28();  // MsgWhoTalkedToMe
    virtual void v29();  // MsgExpressionPerformedTo
    virtual void v30();  // MsgWhoExpressedToMe
    virtual void v31();  // MsgHowLongWasExpressionPerformed
    virtual void v32();  // MsgIsKnockedOutBy
    virtual CScriptThing MsgWhoKnockedMeOut() const;   // slot 33 (+0x84)
};

CScriptThing CScriptThing::MsgWhoKnockedMeOut() const
{
    CScriptThing* implementation = PImp.Data;
    if (implementation == 0)
        return *this;
    return implementation->MsgWhoKnockedMeOut();
}