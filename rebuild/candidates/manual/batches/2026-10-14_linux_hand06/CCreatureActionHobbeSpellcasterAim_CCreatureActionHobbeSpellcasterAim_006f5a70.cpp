// CCreatureAction_HobbeSpellcasterAim::CCreatureAction_HobbeSpellcasterAim() -- default constructor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006F5A70, length 39 bytes (+ int3 padding).
// Callees: the aim-action base constructor @ 0x008BFB00 (itself CCreatureActionBase(0x32) +
// vtable 0x0127F614, CCreatureAction_GenericAimProjectileWeapon in rtti-vtables.tsv), then
// CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10 for the member at +0x114
// followed by its inline CIntelligentPointer vtable store (0x01238C6C). Own vtable 0x01263CF4
// (CCreatureAction_HobbeSpellcasterAim). The manifest label (CAIStateGroupBehaviourBase ctor)
// is propagated garbage; the class is read off the vtable.
struct CCreatureAction_GenericAimProjectileWeapon {
    virtual void v00();
    CCreatureAction_GenericAimProjectileWeapon();      // 0x008BFB00
    unsigned char _body[0x110];             // +0x4..+0x113
};

struct CBaseIntelligentPointer {         // 8 bytes: vptr + the pointee link
    virtual void v00();
    void* m_pItem;                          // +4
    CBaseIntelligentPointer();              // 0x00A01B10
};

struct CIntelligentPointer : CBaseIntelligentPointer {     // vtable 0x01238C6C
    CIntelligentPointer() { }
};

struct CCreatureAction_HobbeSpellcasterAim : CCreatureAction_GenericAimProjectileWeapon {   // vtable 0x01263CF4
    CIntelligentPointer m_pTarget;          // +0x114
    CCreatureAction_HobbeSpellcasterAim();
};

CCreatureAction_HobbeSpellcasterAim::CCreatureAction_HobbeSpellcasterAim()
    : m_pTarget()
{
}
