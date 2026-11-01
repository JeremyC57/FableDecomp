// CCreatureAction_KrakenLowAttackLoop::Clone() const -- new CCreatureAction_KrakenLowAttackLoop(*this): the implicit copy constructor inline (CCombatActionBase copy, vtable, Interrupt, LoopNum)
// Leaf: ?Clone@CCreatureAction_KrakenLowAttackLoop@?A0x
// Retail: Fable.exe (TLC, VC7.1) @ 0x00925D90, 71 bytes, thiscall, plain `ret`.
// Identity (proven): vtable 0x01290C64 slot 11 (the Clone slot; 0x01290C90 is this function's only reference), RTTI
//   .?AVCCreatureAction_KrakenLowAttackLoop@?A0x29922355@@ (rtti-vtables.tsv; anonymous namespace of the kraken TU), and the
//   body stores that same vtable into the copy. Ego_r.pdb / FableWin.pdb
//   ?Clone@CCreatureAction_KrakenLowAttackLoop@?A0xe2035276@@UBEPAVCCreatureActionBase@@XZ (donor hash); retail spelling
//   ?Clone@CCreatureAction_KrakenLowAttackLoop@?A0x29922355@@UBEPAVCCreatureActionBase@@XZ. RTTI base: CCombatActionBase
//   (: CCreatureActionBase), so the base copy is ??0CCombatActionBase@@QAE@ABV0@@Z 0x0062F3B0 (bound so). It allocates
//   0x11C bytes = Ego_r 0x134 less the CCombatActionBase shrink (0x12C -> 0x114). The manifest label "vfunc_11" is the
//   slot number only.
// Callees: operator new 0x00BFEA1A, CCombatActionBase::CCombatActionBase(const CCombatActionBase&) 0x0062F3B0.
// Layout: Ego_r.pdb members Interrupt (bool, retail +0x114: `mov al` byte copy) and LoopNum (long, retail +0x118); the
//   landed constructor 0x009255E0 (`(CThingCreatureBase&, CThing&, long, bool)`, Ego_r ??0..@@QAE@AAVCThingCreatureBase@@AAVCThing@@J_N@Z)
//   stores the long argument at +0x118 and the bool at +0x114, i.e. LoopNum and Interrupt. No engine header exists for the
//   anonymous-namespace class, and engine/CCreatureActionBase.h flattens the vptr into a data member, so the base chain
//   is the landed CCreatureAction_TentacleStrikeLoop::Clone model (0x008CFB70) over the same offsets.
// Shape (family K037): `return new X(*this);` with X's implicit copy constructor inlined; null-tested allocation. The class
//   is defined in `namespace { }` (the vftable store references Clone, so the virtual body is emitted; leaf by prefix).
class CCreatureActionBase {                                  // retail sizeof 0xA8 (Ego_r.pdb 0xB4)
public:
    virtual ~CCreatureActionBase();
    virtual CCreatureActionBase* Clone() const = 0;          // slot 11
};
class CCombatActionBase : public CCreatureActionBase {       // retail sizeof 0x114 (Ego_r.pdb 0x12C)
public:
    CCombatActionBase(const CCombatActionBase& other);      // 0x0062F3B0
private:
    unsigned char _body[0x114 - 4];
};

namespace {

class CCreatureAction_KrakenLowAttackLoop : public CCombatActionBase {   // vtable 0x01290C64
public:
    virtual CCreatureActionBase* Clone() const;
    bool Interrupt;                                          // +0x114
    long LoopNum;                                            // +0x118
};

CCreatureActionBase* CCreatureAction_KrakenLowAttackLoop::Clone() const
{
    return new CCreatureAction_KrakenLowAttackLoop(*this);
}

}   // namespace