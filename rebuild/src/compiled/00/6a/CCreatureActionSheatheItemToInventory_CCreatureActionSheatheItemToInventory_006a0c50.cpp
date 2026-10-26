// CCreatureAction_SheatheItemToInventory::CCreatureAction_SheatheItemToInventory(const CCreatureAction_SheatheItemToInventory& other) -- copy constructor: CCreatureActionBase copy, own vtable, add-ref copy of PFollowUpAction
// Leaf: ??0CCreatureAction_SheatheItemToInventory
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A0C50, length 57 bytes.
// Callees: CCreatureActionBase::CCreatureActionBase(const CCreatureActionBase&) @ 0x0062EBC0.
// Vtable: 0x0125C83C (CCreatureAction_SheatheItemToInventory, rtti-vtables.tsv). Called from the
// landed Clone @ 0x006A0C20 (`new T(*this)`, sizeof 0xb0).
// Shape: `other` in edi pushed for the base copy constructor; the derived vtable store; then the one
// derived member, PFollowUpAction (CCountedPointer<CCreatureActionBase>, PDB name), is copied by its
// inline copy constructor with the count block add-ref'd in place (`inc dword ptr [eax]`).
// Layout: retail places PFollowUpAction at +0xa8 (the PDB's +0xb4 sits 12 bytes high, the retail
// CCreatureActionBase being 0xa8 bytes, not the PDB's 0xb4); no engine header exists for this class.
// engine/CCreatureActionBase.h is that 0xb4-byte PDB layout, so the base is a local opaque 0xa8-byte model
// (polymorphic, with its out-of-line copy constructor).
// The manifest label "CScriptGameResourceObjectScriptedThingBase" is propagated (the vtable names the class).
class CCPPointerInfo { public:
    long m_count;                                     // +0x00 shared reference count
};

class CCreatureActionBase;

struct CCountedPointer_CCreatureActionBase {
    CCreatureActionBase* Data;
    CCPPointerInfo*      Info;
    CCountedPointer_CCreatureActionBase(const CCountedPointer_CCreatureActionBase& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->m_count;
    }
};

class CCreatureActionBase { public:
    virtual void v00();
    unsigned char _body[0xa8 - 4];                    // retail sizeof(CCreatureActionBase) = 0xa8
    CCreatureActionBase(const CCreatureActionBase& other);     // 0x0062EBC0
};

class CCreatureAction_SheatheItemToInventory : public CCreatureActionBase { public:
    CCountedPointer_CCreatureActionBase PFollowUpAction;      // +0xa8
    virtual void v00();
    CCreatureAction_SheatheItemToInventory(const CCreatureAction_SheatheItemToInventory& other);
};

CCreatureAction_SheatheItemToInventory::CCreatureAction_SheatheItemToInventory(const CCreatureAction_SheatheItemToInventory& other)
    : CCreatureActionBase(other), PFollowUpAction(other.PFollowUpAction)
{
}