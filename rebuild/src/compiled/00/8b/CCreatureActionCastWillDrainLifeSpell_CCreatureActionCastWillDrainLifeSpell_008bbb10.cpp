// CCreatureAction_CastWillDrainLifeSpell::CCreatureAction_CastWillDrainLifeSpell() -- CCreatureActionBase(150) through the inlined spell-action base, Level and DrainDefIndex zero, then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008BBB10, length 48 bytes.
// Callees: 0x00857E40 CCreatureActionBase::CCreatureActionBase(long)
// The manifest label (C3DMeshFileXAnimatedPrimitiveChunk) is propagated; the vtable stored is
// CCreatureAction_CastWillDrainLifeSpell's (0x012645EC). The two zero stores at +0x114/+0x118 (PDB: Level,
// DrainDefIndex) precede the vtable store, so they belong to an inlined intermediate base constructor whose
// own vtable store is dropped (`__declspec(novtable)`: the retail spell-action base is abstract).
//

class CCreatureActionBase { public:
    virtual void v0();
    CCreatureActionBase(long actionType);          // 0x00857E40
    unsigned char _body[0x110];                    // +0x04 .. +0x113 (PDB sizeof 0x114)
};
struct __declspec(novtable) CCreatureAction_CastSpellBase : CCreatureActionBase {
    long Level;                                    // +0x114
    long DrainDefIndex;                            // +0x118   sizeof == 0x11c
    CCreatureAction_CastSpellBase(long actionType) : CCreatureActionBase(actionType), Level(0), DrainDefIndex(0) {}
};
class CCreatureAction_CastWillDrainLifeSpell : public CCreatureAction_CastSpellBase { public:
    virtual void v0();
    CCreatureAction_CastWillDrainLifeSpell();
};
CCreatureAction_CastWillDrainLifeSpell::CCreatureAction_CastWillDrainLifeSpell()
    : CCreatureAction_CastSpellBase(150)
{
}