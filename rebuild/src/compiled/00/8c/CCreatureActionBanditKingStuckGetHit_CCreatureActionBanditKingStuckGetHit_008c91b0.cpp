// CCreatureAction_BanditKingStuckGetHit::CCreatureAction_BanditKingStuckGetHit() -- base CCombatActionBase(250), own vtable (0x01281EE4), default-constructed CIntelligentPointer member at +0x114
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C91B0, length 48 bytes.
// Callees: CCombatActionBase::CCombatActionBase(long) @ 0x00857E40, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10
// (the inline CIntelligentPointer default constructor: base call then its vtable 0x01238C6C, rtti-vtables.tsv)
// Shape: base, this class's vtable, then the init-list member: the member's inline constructor is a call so the
// derived vtable store stays ahead of it (only plain stores sink it). The member's address is kept in edi for its
// vtable store. The PDB build has this pointer at +0x12c (its CCombatActionBase is 0x18 bytes wider).
class CBaseObject;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                   // +0x04
    CBaseIntelligentPointer();              // 0x00A01B10
    CBaseObject* GetPItem() const;          // 0x00A01B50
    void SetPItem(CBaseObject* item);       // 0x00A01B90
};

struct CIntelligentPointer : CBaseIntelligentPointer {         // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    CIntelligentPointer(CBaseObject* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

class CCombatActionBase { public:                  // retail sizeof 0x114 (the PDB build is 0x12c: three containers are 4 bytes wider there)
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(long actionCode);     // 0x00857E40
};

class CCreatureAction_BanditKingStuckGetHit : public CCombatActionBase { public:             // vtable 0x01281EE4
    CIntelligentPointer PTarget;            // +0x114 CIntelligentPointer<CThing>
    virtual void v00();
    CCreatureAction_BanditKingStuckGetHit();
};

CCreatureAction_BanditKingStuckGetHit::CCreatureAction_BanditKingStuckGetHit()
    : CCombatActionBase(250)
    , PTarget()
{
}