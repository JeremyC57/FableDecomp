// CCreatureAction_BalverineCorneredJumpIntoSky::CCreatureAction_BalverineCorneredJumpIntoSky(CThingCreatureBase*, CThing*, C3DVector*) -- base CCombatActionBase(owner, target, 150, zeroed CSoundPair), EndPos pointer stored, own vtable (0x012837C4)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CCB80, length 80 bytes.
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, CThing*, long, const CSoundPair&) @ 0x00857F30
// Shape: the pointer argument is stored BEFORE the vtable store (an init-list member: its plain store sinks the
// vtable store below it). EndPos is the engine-header name (PDB +0x12c -> retail +0x114). `ret 0Ch`: three arguments.
class CThingCreatureBase;
class CThing;
class C3DVector;

class CSoundPair { public:                         // two sound ids, zeroed by the inline constructor (the register-zeroed
    long FirstSound;                        //   stack stores before the base call: an embedded object's inline ctor)
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

class CCombatActionBase { public:                  // retail sizeof 0x114 (the PDB build is 0x12c: three containers are 4 bytes wider there)
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound);   // 0x00857F30
};

class CCreatureAction_BalverineCorneredJumpIntoSky : public CCombatActionBase { public:   // vtable 0x012837C4
    C3DVector* EndPos;                      // +0x114
    virtual void v00();
    CCreatureAction_BalverineCorneredJumpIntoSky(CThingCreatureBase* owner, CThing* target, C3DVector* endPos);
};

CCreatureAction_BalverineCorneredJumpIntoSky::CCreatureAction_BalverineCorneredJumpIntoSky(CThingCreatureBase* owner, CThing* target, C3DVector* endPos)
    : CCombatActionBase(owner, target, 150, CSoundPair())
    , EndPos(endPos)
{
}