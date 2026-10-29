// CCreatureAction_WaspQueenPrimaryAttack::FinishAction() -- base FinishAction, then remove the hit-on-collision component (interface type 200 = CTCHitOnCollision) from the creature
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C4020, length 32 bytes (byte-identical to the LashOutAttack body @ 0x008C40D0 up to the relocations). FinishAction is vtable slot 4 (protected `MAE`); the base is called directly here.
// Callees: CCombatActionBase::FinishAction @ 0x00859A20 (direct base call), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8),
// CThing::RemoveTC(long interfaceType) @ 0x004C9840 (named from its body: it looks the type up in the thing's interface map at +0x44 and
// retires the component; the manifest "CBaseObject::OnPropertyChanged" label is propagated). The DiveAttack Initialise @ 0x008C4250 adds
// the same component ("CTCHitOnCollision", bit 8 of the bitset dword at +0x38 = type 200).
// Shape: the argument is pushed before the object expression is evaluated.
enum ETCInterfaceType { };
class CThing { public:
    void RemoveTC(ETCInterfaceType interfaceType);      // 0x004C9840
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

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    protected: virtual void FinishAction(); public:            // slot 4: 0x00859A20
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];     // +0x10
};

class CCreatureAction_WaspQueenPrimaryAttack : public CCombatActionBase { public:
    virtual void FinishAction();
};

void CCreatureAction_WaspQueenPrimaryAttack::FinishAction()
{
    CCombatActionBase::FinishAction();
    PCreature.GetPItem()->RemoveTC((ETCInterfaceType)(200));
}