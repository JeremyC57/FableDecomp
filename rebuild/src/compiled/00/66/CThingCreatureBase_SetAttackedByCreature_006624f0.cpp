// CThingCreatureBase::SetAttackedByCreature(CThingCreatureBase* attacker) -- record the attacker and the world frame of the attack
// Retail: Fable.exe (TLC, VC7.1) @ 0x006624F0, 85 bytes (+ int3 padding).
// Callees: static CWorld::GetWorldFrame @ 0x0049D870, CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10,
//          SetPItem @ 0x00A01B90 (on the temporary, then on PLastAttackedByCreature), GetPItem @ 0x00A01B50,
//          ~CBaseIntelligentPointer @ 0x00A01C10. Vtable 0x0123E7D8: CIntelligentPointer<CThingCreatureBase> (the
//          inlined derived constructor's store).
// Layout: PLastAttackedByCreature at retail +0xf4, WFLastAttackedByCreature at +0xfc (PDB +0x110/+0x118).
// Shape: the landed SetPTarget family (0x007A7150): an 8-byte stack CIntelligentPointer built by the inline
//   default constructor, pointed at the argument, read back into the member pointer and destroyed. The frame
//   store comes first (no GetWorld call here: the static getter is called directly).
class CBaseObject;
class CThingCreatureBase;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;
    CBaseIntelligentPointer();                      // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem();                        // 0x00A01B50
    void SetPItem(CBaseObject* item);               // 0x00A01B90
};

struct CIntelligentPointer : CBaseIntelligentPointer {      // CIntelligentPointer<CThingCreatureBase>, vtable 0x0123E7D8
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};

class CWorld { public:
    static long GetWorldFrame();                    // 0x0049D870
};

class CThingCreatureBase { public:
    unsigned char _pad000[0xf4];
    CBaseIntelligentPointer PLastAttackedByCreature;    // +0xf4
    unsigned long WFLastAttackedByCreature;             // +0xfc
    void SetAttackedByCreature(CThingCreatureBase* attacker);
};

void CThingCreatureBase::SetAttackedByCreature(CThingCreatureBase* attacker)
{
    WFLastAttackedByCreature = CWorld::GetWorldFrame();
    CIntelligentPointer pointer;
    pointer.SetPItem((CBaseObject*)attacker);
    PLastAttackedByCreature.SetPItem(pointer.GetPItem());
}