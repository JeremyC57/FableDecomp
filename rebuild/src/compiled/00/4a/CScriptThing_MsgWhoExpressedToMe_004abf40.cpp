// CScriptThing::MsgWhoExpressedToMe() -- return who last performed an expression to the thing: a copy of the (null) handle itself when it has no implementation, else the implementation's answer
// Leaf: ?MsgWhoExpressedToMe@CScriptThing@@UBE?AV1@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004ABF40, 57 bytes, thiscall, `ret 4` (the result by hidden pointer).
// Identity (proven): slot 30 of vtable 0x01238C8C (RTTI .?AVCScriptThing@@) and the slot it forwards to (`call
//   [edx+0x78]`); Ego_r.pdb ?MsgWhoExpressedToMe@CScriptThing@@UBE?AV1@XZ (the vtable order around it matches the landed slots: MsgExpressionPerformedTo 29, MsgWhoKnockedMeOut 33).
// Callees: the implementation's own MsgWhoExpressedToMe (virtual slot 30); the copy constructor 0x004ABE90 is inlined (vtable store,
//   two word copies, `inc` of the count block).
// Shape (family M075, representative 0x004ABEC0): the landed twin MsgWhoKnockedMeOut 0x004ABF80 (slot 33) verbatim -- the
//   implementation pointer read into a local (eax for the vtable fetch and `mov ecx,eax`).
// Header debt: engine/CScriptThing.h flattens the vptr into a `__vftable` word and cannot carry the virtual table or the
//   copy constructor, so the class is modelled locally with the header's member names (as the landed slot-33 twin).
class CCPPointerInfo {
public:
    long m_references;                                    // +0 reference count
};
struct CBase {
    virtual ~CBase();                                     // 0x0099A2E0
};
class CScriptThing;
struct CCountedPointer_CScriptThing {
    CScriptThing* Data;                                   // +0
    CCPPointerInfo* Info;                                 // +4
    CCountedPointer_CScriptThing(const CCountedPointer_CScriptThing& other)
        : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->m_references;
    }
};
class CScriptThing : public CBase {
public:
    CCountedPointer_CScriptThing PImp;                    // +0x04
    CScriptThing(const CScriptThing& other) : PImp(other.PImp) { }   // 0x004ABE90 (inlined)
    virtual ~CScriptThing();                              // slot 0
    virtual void VirtualSlot01();                          // GetName
    virtual void VirtualSlot02();                          // GetDefName
    virtual void VirtualSlot03();                          // GetDataString
    virtual void VirtualSlot04();                          // SetDataString
    virtual void VirtualSlot05();                          // GetFocalPos
    virtual void VirtualSlot06();                          // GetPos
    virtual void VirtualSlot07();                          // GetHomePos
    virtual void VirtualSlot08();                          // GetCurrentMapName
    virtual void VirtualSlot09();                          // GetHomeMapName
    virtual void VirtualSlot10();                          // GetAngleXY
    virtual void VirtualSlot11();                          // GetPThing
    virtual void VirtualSlot12();                          // GetPThingUniqueID
    virtual void VirtualSlot13();                          // GetCurrentStateGroupType
    virtual void VirtualSlot14();                          // GetCurrentScriptPriority
    virtual void VirtualSlot15();                          // IsSneaking
    virtual void VirtualSlot16();                          // IsAwareOfHero
    virtual void VirtualSlot17();                          // MsgIsTriggeredBy
    virtual void VirtualSlot18();                          // MsgIsKilledBy
    virtual void VirtualSlot19();                          // MsgWhoKilledMe
    virtual void VirtualSlot20();                          // MsgIsUsedBy
    virtual void VirtualSlot21();                          // MsgIsHitBy
    virtual void VirtualSlot22();                          // MsgIsHitByWithFlourish
    virtual void VirtualSlot23();                          // MsgIsHitByWithDecapitate
    virtual void VirtualSlot24();                          // MsgIsHitByWithWeapon
    virtual void VirtualSlot25();                          // MsgIsHitByWithProjectileWeapon
    virtual void VirtualSlot26();                          // MsgWhoHitMe
    virtual void VirtualSlot27();                          // MsgIsTalkedToBy
    virtual void VirtualSlot28();                          // MsgWhoTalkedToMe
    virtual void VirtualSlot29();                          // MsgExpressionPerformedTo
    virtual CScriptThing MsgWhoExpressedToMe() const;       // slot 30 (+0x78)
};
CScriptThing CScriptThing::MsgWhoExpressedToMe() const
{
    CScriptThing* implementation = PImp.Data;
    if (implementation == 0)
        return *this;
    return implementation->MsgWhoExpressedToMe();
}