// CCreatureActionHero180RunningTurn::CCreatureActionHero180RunningTurn(CThingCreatureBase*, const C3DVector&) -- base CCombatActionBase(owner, 1, zeroed CSoundPair), own vtable (0x0127655C), RequiredFacing copied from the argument
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0085BB10, length 80 bytes.
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&) @ 0x00858030
// Shape: the vtable store precedes the three dword copies of the vector (edx/ecx/edx): the init-list copy of the member
// through C3DVector's inlined memberwise copy constructor (a plain POD assignment addresses the member with `lea` and
// stores through it; a body assignment of a class with a user copy constructor has no default constructor to build on).
// RequiredFacing is the engine-header name (PDB +0x12c; retail CCombatActionBase is 0x114 bytes, so +0x114 here).
class CThingCreatureBase;
class CCharString;

class CSoundPair { public:                         // two sound ids, zeroed by the inline constructor (the register-zeroed
    long FirstSound;                        //   stack stores before the base call: an embedded object's inline ctor)
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

class C3DVector { public:                         // three floats with a memberwise copy constructor (one register per dword)
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};

class CCombatActionBase { public:                  // retail sizeof 0x114 (the PDB build is 0x12c: three containers are 4 bytes wider there)
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
    void SetCombatAnimation(const CCharString& name);                                           // 0x008574D0
};

class CCreatureActionHero180RunningTurn : public CCombatActionBase { public:   // vtable 0x0127655C
    C3DVector RequiredFacing;               // +0x114
    virtual void v00();
    CCreatureActionHero180RunningTurn(CThingCreatureBase* owner, const C3DVector& requiredFacing);
};

CCreatureActionHero180RunningTurn::CCreatureActionHero180RunningTurn(CThingCreatureBase* owner, const C3DVector& requiredFacing)
    : CCombatActionBase(owner, 1, CSoundPair())
    , RequiredFacing(requiredFacing)
{
}