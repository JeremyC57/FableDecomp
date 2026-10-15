// CCreatureAction_BanditKingStuck::CCreatureAction_BanditKingStuck(CThingCreatureBase*, CThing*, long, CThing*, CThing*) -- base CCombatActionBase(owner, target, 150, zeroed CSoundPair), own vtable (0x01281C54), TimesToLoop from the count, PStuckEffect / PObstruction set to the two things
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C94D0, length 132 bytes (+ int3 padding).
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, CThing*, long, const CSoundPair&) @ 0x00857F30,
// CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 (twice), CBaseIntelligentPointer::SetPItem @ 0x00A01B90 (twice).
// Shape: the owner is loaded into edx before the frame registers are pushed; the sound pair is zeroed through eax and
// passed by reference; base arguments right to left (the pair, 0x96, the target, the owner). Then the init-list members in
// declaration order: the count word (sinks below the vtable store), the CIntelligentPointer<CThing> at +0x118 set from the
// LAST argument, and the one at +0x120 set from the fourth (base ctor @ 0x00A01B10, vtable 0x01238C6C, SetPItem each).
// `ret 14h`: five arguments.
// Layout: engine/CCreatureAction_BanditKingStuck.h has TimesToLoop +0x12c, PStuckEffect +0x130, PObstruction +0x138 (the PDB
// CCombatActionBase is 0x12c bytes; retail 0x114), so `this` is modelled locally at +0x114 / +0x118 / +0x120.
struct CThingCreatureBase;
struct CThing;
struct CBaseObject;

struct CSoundPair {                         // two sound ids, zeroed by the inline constructor (register-zeroed frame stores)
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

struct CBaseIntelligentPointer {            // 8 bytes: vptr + the pointed-to item
    virtual void v00();
    CBaseObject* m_pItem;                   // +4
    CBaseIntelligentPointer();              // 0x00A01B10
    void SetPItem(CBaseObject* item);       // 0x00A01B90
};

struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};

struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound);   // 0x00857F30
};

struct CCreatureAction_BanditKingStuck : CCombatActionBase {   // vtable 0x01281C54
    long                TimesToLoop;        // +0x114 (PDB +0x12c)
    CIntelligentPointer PStuckEffect;       // +0x118 (PDB +0x130)
    CIntelligentPointer PObstruction;       // +0x120 (PDB +0x138)
    virtual void v00();
    CCreatureAction_BanditKingStuck(CThingCreatureBase* owner, CThing* target, long timesToLoop, CThing* obstruction, CThing* stuckEffect);
};

CCreatureAction_BanditKingStuck::CCreatureAction_BanditKingStuck(CThingCreatureBase* owner, CThing* target, long timesToLoop, CThing* obstruction, CThing* stuckEffect)
    : CCombatActionBase(owner, target, 150, CSoundPair())
    , TimesToLoop(timesToLoop)
    , PStuckEffect(stuckEffect)
    , PObstruction(obstruction)
{
}
