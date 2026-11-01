// CCreatureAction_DragonFlight::Clone() const -- new CCreatureAction_DragonFlight(*this): the implicit copy constructor inline (CCombatActionBase copy, vtable, FaceTarget, TurnRate)
// Leaf: ?Clone@CCreatureAction_DragonFlight@@UBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008F5740, 71 bytes, thiscall, plain `ret`.
// Identity (proven): vtable 0x01288544 slot 11 (the Clone slot; 0x01288570 is this function's only reference), RTTI
//   .?AVCCreatureAction_DragonFlight@@ (rtti-vtables.tsv), and the body stores that same vtable into the copy. Ego_r.pdb /
//   FableWin.pdb ?Clone@CCreatureAction_DragonFlight@@UBEPAVCCreatureActionBase@@XZ. RTTI base: CCreatureAction_DragonFlight :
//   CCombatActionBase : CCreatureActionBase, so the base copy is ??0CCombatActionBase@@QAE@ABV0@@Z 0x0062F3B0 (bound so).
//   It allocates 0x11C bytes = the retail sizeof (engine/CCreatureAction_DragonFlight.h: Ego_r 0x134 less the
//   CCombatActionBase shrink 0x12C -> 0x114). The manifest label is right.
// Callees: operator new 0x00BFEA1A, CCombatActionBase::CCombatActionBase(const CCombatActionBase&) 0x0062F3B0.
// Layout: Ego_r.pdb members FaceTarget (bool, retail +0x114: `mov al` byte copy) and TurnRate (float, retail +0x118: copied
//   through ecx by the implicit copy constructor). engine/CCreatureAction_DragonFlight.h and engine/CCreatureActionBase.h
//   flatten the vptr into a data member, so the
//   polymorphic base chain with its out-of-line copy constructor is modelled locally over the same offsets (the landed
//   CCreatureAction_TentacleStrikeLoop::Clone model, 0x008CFB70).
// Shape (family K037): `return new X(*this);` with X's implicit copy constructor inlined; null-tested allocation.
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

class CCreatureAction_DragonFlight : public CCombatActionBase {   // vtable 0x01288544
public:
    virtual CCreatureActionBase* Clone() const;
    bool  FaceTarget;                                        // +0x114
    float TurnRate;                                          // +0x118
};

CCreatureActionBase* CCreatureAction_DragonFlight::Clone() const
{
    return new CCreatureAction_DragonFlight(*this);
}