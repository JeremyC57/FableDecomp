// CCreatureAction_Ambush::CCreatureAction_Ambush(CThingCreatureBase*, CThing*, const C3DVector&) -- base CCombatActionBase(owner, target, 250, zeroed CSoundPair), own vtable (0x0127FF14), JumpDest copied from the argument
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008BFDA0, length 96 bytes.
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, CThing*, long, const CSoundPair&) @ 0x00857F30
// Shape: the vtable store precedes the three dword copies of the vector (ecx/edx/eax): the init-list copy of the member
// through C3DVector's inlined memberwise copy constructor (a plain POD assignment addresses the member with `lea`
// and stores through it instead). JumpDest is the engine-header name (PDB +0x12c; retail
// CCombatActionBase is 0x114 bytes, so +0x114 here). `ret 0Ch`: three arguments.
class CThingCreatureBase;
class CThing;

class CSoundPair { public:                         // two sound ids, zeroed by the inline constructor (the register-zeroed
    long FirstSound;                        //   stack stores before the base call: an embedded object's inline ctor)
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

#include "engine/C3DVector.h"

class CCombatActionBase { public:                  // retail sizeof 0x114 (the PDB build is 0x12c: three containers are 4 bytes wider there)
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound);   // 0x00857F30
};

class CCreatureAction_Ambush : public CCombatActionBase { public:   // vtable 0x0127FF14
    C3DVector JumpDest;                     // +0x114
    virtual void v00();
    CCreatureAction_Ambush(CThingCreatureBase* owner, CThing* target, const C3DVector& jumpDest);
};

CCreatureAction_Ambush::CCreatureAction_Ambush(CThingCreatureBase* owner, CThing* target, const C3DVector& jumpDest)
    : CCombatActionBase(owner, target, 250, CSoundPair())
    , JumpDest(jumpDest)
{
}