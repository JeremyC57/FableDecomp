// CCreatureAction_DragonSwoop::CCreatureAction_DragonSwoop(CThingCreatureBase*, const CCharString&, const C3DVector&, const C3DVector&, const C3DVector&) -- combat action (action code 150, silent sound pair) with the given animation and three swoop points
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F4F40, length 143 bytes, `ret 14h` (five arguments). Vtable 0x012887D4.
// Callees: CCombatActionBase::CCombatActionBase(owner, actionCode, const CSoundPair&) @ 0x00858030,
//          CCombatActionBase::SetAnimationName(const CCharString&) @ 0x008574D0 (manifest label
//          "CTCInGameMenu::SetPDef" is propagated).
// Shape: CSoundPair temporary zeroed from eax by its inline constructor; after the vtable store the animation name
//   argument is forwarded by reference, then the body copies each point into SwoopPoints[i]: a C3DVector POD
//   assignment addresses the element once with `lea` and copies three dwords through it.
// Layout: engine/CCreatureAction_DragonSwoop.h is the PDB build (SwoopPoints at +0x12c); the retail CCombatActionBase
//   is 0x114 bytes, so the array sits at +0x114 and the header is not used.
class CThingCreatureBase;
class CCharString;

class CSoundPair { public:                         // two sound ids, zeroed by the inline constructor
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

class C3DVector { public: float X, Y, Z; };

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
    void SetAnimationName(const CCharString& name);                                           // 0x008574D0
};

class CCreatureAction_DragonSwoop : public CCombatActionBase { public:   // vtable 0x012887D4
    C3DVector SwoopPoints[3];               // +0x114 (PDB +0x12c)
    virtual void v00();
    CCreatureAction_DragonSwoop(CThingCreatureBase* owner, const CCharString& animation,
                                const C3DVector& point0, const C3DVector& point1, const C3DVector& point2);
};

CCreatureAction_DragonSwoop::CCreatureAction_DragonSwoop(CThingCreatureBase* owner, const CCharString& animation,
                                                         const C3DVector& point0, const C3DVector& point1, const C3DVector& point2)
    : CCombatActionBase(owner, 150, CSoundPair())
{
    SetAnimationName(animation);
    SwoopPoints[0] = point0;
    SwoopPoints[1] = point1;
    SwoopPoints[2] = point2;
}