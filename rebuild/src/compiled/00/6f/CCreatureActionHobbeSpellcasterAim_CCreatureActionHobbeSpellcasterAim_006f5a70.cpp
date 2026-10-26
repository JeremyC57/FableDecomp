// CCreatureAction_HobbeSpellcasterAim::CCreatureAction_HobbeSpellcasterAim() -- default constructor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006F5A70, length 39 bytes (+ int3 padding).
// Callees: the aim-action base constructor @ 0x008BFB00 (itself CCreatureActionBase(0x32) +
// vtable 0x0127F614, CCreatureAction_GenericAimProjectileWeapon in rtti-vtables.tsv), then
// CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10 for the member at +0x114
// followed by its inline CIntelligentPointer vtable store (0x01238C6C). Own vtable 0x01263CF4
// (CCreatureAction_HobbeSpellcasterAim). The manifest label (CAIStateGroupBehaviourBase ctor)
// is propagated garbage; the class is read off the vtable.
class CCreatureAction_GenericAimProjectileWeapon { public:
    virtual void v00();
    CCreatureAction_GenericAimProjectileWeapon();      // 0x008BFB00
    unsigned char _body[0x110];             // +0x4..+0x113
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

class CThing;
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {     // vtable 0x01238C6C
public:
    CIntelligentPointer() { }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CCreatureAction_HobbeSpellcasterAim : public CCreatureAction_GenericAimProjectileWeapon { public:   // vtable 0x01263CF4
    CIntelligentPointer<CThing> m_pTarget;          // +0x114
    CCreatureAction_HobbeSpellcasterAim();
};

CCreatureAction_HobbeSpellcasterAim::CCreatureAction_HobbeSpellcasterAim()
    : m_pTarget()
{
}