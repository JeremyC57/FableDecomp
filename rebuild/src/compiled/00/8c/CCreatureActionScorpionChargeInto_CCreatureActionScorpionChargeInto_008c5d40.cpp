// CCreatureAction_ScorpionChargeInto::CCreatureAction_ScorpionChargeInto(const CCreatureAction_ScorpionChargeInto&) -- copy constructor: base CCombatActionBase(other), own vtable (0x01280ED4), ChargeDistance copied, PTarget copy-constructed via GetPItem/SetPItem
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C5D40, length 80 bytes.
// Callees: CCombatActionBase::CCombatActionBase(const CCombatActionBase&) @ 0x0062F3B0, CBaseIntelligentPointer::CBaseIntelligentPointer
// @ 0x00A01B10, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CBaseIntelligentPointer::SetPItem @ 0x00A01B90 (the inline
// CIntelligentPointer copy constructor, as in the landed 0x008C3D10 shape; its vtable 0x01238C6C)
// Shape: the implicit memberwise copy: base copy out of line, vtable, the ChargeDistance dword (a float copied with
// a plain `mov`), then the pointer member's inline copy constructor (its address in edi). ChargeDistance is the
// engine-header name (PDB +0x12c -> retail +0x114). The manifest label "CCreatureAction_BanditKingStuck" is
// rejected: the vtable stored is CCreatureAction_ScorpionChargeInto's (rtti-vtables.tsv).
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {         // vtable 0x01238C6C
public:
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    CIntelligentPointer(CBaseObject* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(const CCombatActionBase& other);   // 0x0062F3B0
};

class CCreatureAction_ScorpionChargeInto : public CCombatActionBase { public:   // vtable 0x01280ED4; retail sizeof 0x120
    float               ChargeDistance;     // +0x114
    CIntelligentPointer<CThing> PTarget;            // +0x118 CIntelligentPointer<CThing>
    virtual void v00();
    CCreatureAction_ScorpionChargeInto(const CCreatureAction_ScorpionChargeInto& other);
};

CCreatureAction_ScorpionChargeInto::CCreatureAction_ScorpionChargeInto(const CCreatureAction_ScorpionChargeInto& other)
    : CCombatActionBase(other)
    , ChargeDistance(other.ChargeDistance)
    , PTarget(other.PTarget)
{
}