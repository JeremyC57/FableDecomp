// CCreatureAction_BanditKingStuck::CCreatureAction_BanditKingStuck(const CCreatureAction_BanditKingStuck&) -- copy constructor: base CCombatActionBase(other), own vtable (0x01281C54), TimesToLoop copied, PStuckEffect and PObstruction copied through GetPItem/SetPItem
// Leaf: ??0CCreatureAction_BanditKingStuck@@QAE@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C9860, length 117 bytes (+ int3 padding).
// Callees: CCombatActionBase::CCombatActionBase(const CCombatActionBase&) @ 0x0062F3B0, CBaseIntelligentPointer::CBaseIntelligentPointer
// @ 0x00A01B10 (twice), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (twice), CBaseIntelligentPointer::SetPItem @ 0x00A01B90 (twice).
// Shape: `other` rides in edi and is pushed for the out-of-line base copy constructor; the vtable store comes first, the
// count word is copied through eax (sinks below the first member's `lea`), then each CIntelligentPointer<CThing> member is
// its inline copy constructor (base ctor, vtable 0x01238C6C, SetPItem(other.GetPItem())), the member address kept in ebx. `ret 4`.
// Layout: engine/CCreatureAction_BanditKingStuck.h has TimesToLoop +0x12c, PStuckEffect +0x130, PObstruction +0x138 (the PDB
// CCombatActionBase is 0x12c bytes; retail 0x114), so `this` is modelled locally at +0x114 / +0x118 / +0x120.
struct CBaseObject;

struct CBaseIntelligentPointer {            // 8 bytes: vptr + the pointed-to item
    virtual void v00();
    CBaseObject* m_pItem;                   // +4
    CBaseIntelligentPointer();              // 0x00A01B10
    CBaseObject* GetPItem() const;          // 0x00A01B50
    void SetPItem(CBaseObject* item);       // 0x00A01B90
};

struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(const CCombatActionBase& other);   // 0x0062F3B0
};

struct CCreatureAction_BanditKingStuck : CCombatActionBase {   // vtable 0x01281C54
    long                TimesToLoop;        // +0x114 (PDB +0x12c)
    CIntelligentPointer PStuckEffect;       // +0x118 (PDB +0x130)
    CIntelligentPointer PObstruction;       // +0x120 (PDB +0x138)
    virtual void v00();
    CCreatureAction_BanditKingStuck(const CCreatureAction_BanditKingStuck& other);
};

CCreatureAction_BanditKingStuck::CCreatureAction_BanditKingStuck(const CCreatureAction_BanditKingStuck& other)
    : CCombatActionBase(other)
    , TimesToLoop(other.TimesToLoop)
    , PStuckEffect(other.PStuckEffect)
    , PObstruction(other.PObstruction)
{
}
