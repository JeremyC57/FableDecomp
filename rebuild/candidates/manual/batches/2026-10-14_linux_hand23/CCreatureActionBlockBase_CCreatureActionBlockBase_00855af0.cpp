// CCreatureAction_BlockBase::CCreatureAction_BlockBase(CThingCreatureBase*, CThing*, long, long) -- base CCombatActionBase(owner, target, actionCode, zeroed CSoundPair), own vtable (0x01275F3C), BlowStrength from the argument, MovementDirection zeroed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00855AF0, length 96 bytes.
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, CThing*, long, const CSoundPair&) @ 0x00857F30
// Shape: init-list members after the vtable store: BlowStrength (+0x114) from the fourth argument, then the three
// zero stores of MovementDirection (+0x120, +0x11c, +0x118 from one zeroed edi: the inline default constructor's
// chained `X = Y = Z = 0.0f`, which evaluates right to left -- an init list X(0),Y(0),Z(0) stores forward and differs).
// Member names from engine/CCreatureAction_BlockBase.h (PDB +0x12c/+0x130; retail CCombatActionBase is 0x114 bytes).
// `ret 10h`: four arguments.
struct CThingCreatureBase;
struct CThing;

struct CSoundPair {                         // two sound ids, zeroed by the inline constructor (the register-zeroed
    long FirstSound;                        //   stack stores before the base call: an embedded object's inline ctor)
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

struct C3DVector {
    float X, Y, Z;
    C3DVector() { X = Y = Z = 0.0f; }     // chained assignment: Z, Y, X store order (retail +0x120, +0x11c, +0x118)
};

struct CCombatActionBase {                  // retail sizeof 0x114 (the PDB build is 0x12c: three containers are 4 bytes wider there)
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound);   // 0x00857F30
};

struct CCreatureAction_BlockBase : CCombatActionBase {   // vtable 0x01275F3C
    long      BlowStrength;                 // +0x114
    C3DVector MovementDirection;            // +0x118
    virtual void v00();
    CCreatureAction_BlockBase(CThingCreatureBase* owner, CThing* target, long actionCode, long blowStrength);
};

CCreatureAction_BlockBase::CCreatureAction_BlockBase(CThingCreatureBase* owner, CThing* target, long actionCode, long blowStrength)
    : CCombatActionBase(owner, target, actionCode, CSoundPair())
    , BlowStrength(blowStrength)
    , MovementDirection()
{
}
