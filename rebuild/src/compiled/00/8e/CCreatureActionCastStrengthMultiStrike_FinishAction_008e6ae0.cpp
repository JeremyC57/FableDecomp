// CCreatureAction_CastStrengthMultiStrike::FinishAction() -- base FinishAction; while strikes remain and owner and target are alive, chain a fresh multi-strike action with one strike fewer as the owner's current action, else remove the owner's interface-0x8c component
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E6AE0, length 202 bytes (+ int3 padding), plain `ret` (virtual, no arguments).
// Callees: CCombatActionBase::FinishAction 0x00859A20 (direct), CBaseIntelligentPointer::GetPItem 0x00A01B50 (owner +8, target +0xa8),
// CCreatureAction_CastStrengthMultiStrike::CCreatureAction_CastStrengthMultiStrike(CThingCreatureBase*, CThing*, long) 0x008E6A70,
// CThingCreatureBase::SetCurrentAction(const CCreatureActionBase&) 0x006644F0, the temporary's destructor 0x0062E900 (a shared
// byte-identical body; manifest label "~CTCLightningOrb" is propagated), CThing::RemoveTC(long) 0x004C9840 (manifest label
// "CBaseObject::OnPropertyChanged" is propagated; named as in the landed 0x008C4020).
// Shape: the 0x118-byte action temporary fills the whole frame; the constant 1 lives in ebx for both the `> 1` compare and the
// dead-flag tests; target, owner and the call's owner are three named locals fetched in that order before the temporary is built
// (an inline `Owner.GetPItem()->SetCurrentAction(T(Owner.GetPItem(), ...))` fetches `this` after the temporary and drops the ebx 1).
// Layout: NumberOfStrikes at retail +0x114 (PDB +0x12c); thing byte +0x91 bit 0 = dead.
enum ETCInterfaceType { };
class CThing { public:
    unsigned char _pad00[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    void RemoveTC(ETCInterfaceType interfaceType);          // 0x004C9840
};
class CCreatureActionBase;
class CThingCreatureBase : public CThing { public:
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
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

class CCreatureActionBase { public:
    virtual void v00();
};

class CCombatActionBase : public CCreatureActionBase { public:
    long _pad04;
    CIntelligentPointer<CThingCreatureBase> Owner;              // +0x08 (the owning creature)
    unsigned char _pad10[0xa8 - 0x10];
    CIntelligentPointer<CThingCreatureBase> Target;             // +0xa8 (the action's target)
    unsigned char _padb0[0x114 - 0xb0];
    protected: virtual void FinishAction(); public:                // 0x00859A20
};

class CCreatureAction_CastStrengthMultiStrike : public CCombatActionBase { public:   // retail sizeof 0x118
    long NumberOfStrikes;                       // +0x114 (PDB +0x12c)
    CCreatureAction_CastStrengthMultiStrike(CThingCreatureBase* owner, CThing* target, long numberOfStrikes);   // 0x008E6A70
    ~CCreatureAction_CastStrengthMultiStrike(); // 0x0062E900
    virtual void FinishAction();
};

void CCreatureAction_CastStrengthMultiStrike::FinishAction()
{
    CCombatActionBase::FinishAction();
    if (NumberOfStrikes > 1 && Owner.GetPItem() && !(Owner.GetPItem()->m_flags91 & 1)
        && Target.GetPItem() && !(Target.GetPItem()->m_flags91 & 1)) {
        CThing* target = Target.GetPItem();
        CThingCreatureBase* owner = Owner.GetPItem();
        CThingCreatureBase* creature = Owner.GetPItem();
        creature->SetCurrentAction(CCreatureAction_CastStrengthMultiStrike(owner, target, NumberOfStrikes - 1));
    } else {
        Owner.GetPItem()->RemoveTC((ETCInterfaceType)(0x8c));
    }
}