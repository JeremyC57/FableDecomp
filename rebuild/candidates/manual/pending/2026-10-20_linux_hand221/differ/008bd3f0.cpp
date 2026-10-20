// CCreatureAction_TeleportOut::FinishAction() -- save the creature's physics state, face it towards the live target, run the base FinishAction and start a CCreatureAction_TeleportIn
// Leaf: FinishAction
// Grade: DIFFER(312v312) -- register swap only: `this` in edi and the target-pointer/position temporaries in ebx (retail ebx / edi); every instruction otherwise matches
// Retail: Fable.exe (TLC, VC7.1) @ 0x008BD3F0, 312 bytes, virtual FinishAction.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PCreature +0x08, PTarget +0xa8), the physics component's virtual slot
//   31 (+0x7c, given this action's +0x114 block) and slot 66 (+0x108, the heading as a 0..1 turn fraction) (names
//   unconfirmed), CThing::GetPosition 0x004C73D0, fmod 0x00BFEB28 (_CIfmod), CCombatActionBase::FinishAction 0x00859A20,
//   CCreatureAction_TeleportIn(CThingCreatureBase*, bool) 0x008BD040, CThingCreatureBase::SetCurrentAction 0x006644F0,
//   ~CCombatActionBase 0x0062E900. Constants: 1/(2*pi) 0x01237F08, 0.0f 0x0122DEDC, 1.0f 0x0122DED8, 1.0 0x01237F00.
// Layout: retail CCombatActionBase is 0x114 bytes: this class's saved block at +0x114 and its flag byte at +0x120; CThing
//   +0x60 the physics component.
// Shape: the heading is atan2(dx, dy) / (2*pi) wrapped into [0, 1) with fmod; dx/dy and the heading are named float
//   locals; the creature is fetched into a named local before the base FinishAction; the TeleportIn action is a
//   by-value temporary argument of SetCurrentAction (0x118 bytes in the frame).
#include <math.h>
struct C3DVector { float X, Y, Z; };
struct C2DVector { float X, Y; };
struct CThingCreatureBase;
#define V10(n) virtual void v##n##0(); virtual void v##n##1(); virtual void v##n##2(); virtual void v##n##3(); virtual void v##n##4(); \
    virtual void v##n##5(); virtual void v##n##6(); virtual void v##n##7(); virtual void v##n##8(); virtual void v##n##9();
struct CPhysicsComponent {
    V10(0) V10(1) V10(2)                              // slots 0..29
    virtual void v30();
    virtual void SaveState(void* state);              // slot 31 (+0x7c)
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37();
    virtual void v38(); virtual void v39();
    V10(4) V10(5)                                     // slots 40..59
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void SetHeading(float turns);             // slot 66 (+0x108)
};
#undef V10
struct CCombatActionBase;
struct CThing {
    unsigned char _pad00[0x60];
    CPhysicsComponent* m_pPhysics;                    // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char m_flags91;                          // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;             // 0x004C73D0
    CPhysicsComponent* GetPhysics() const { return m_pPhysics; }
    bool SetCurrentAction(const CCombatActionBase& action);   // 0x006644F0 (CThingCreatureBase)
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                  // +4
    CThing* GetPItem() const;                         // 0x00A01B50
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CCombatActionBase {                            // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;                // +0x08
    unsigned char _pad10[0xa8 - 0x10];
    CBaseIntelligentPointer PTarget;                  // +0xa8
    unsigned char _padb0[0x114 - 0xb0];
    ~CCombatActionBase();                             // 0x0062E900
    void FinishAction();                              // 0x00859A20
};
struct CCreatureAction_TeleportIn : CCombatActionBase {   // retail sizeof 0x118
    unsigned char _body[4];
    CCreatureAction_TeleportIn(CThing* creature, bool flag);   // 0x008BD040
};
struct CCreatureAction_TeleportOut : CCombatActionBase {
    unsigned char m_savedState[0xc];                  // +0x114
    bool m_flag;                                      // +0x120
    void FinishAction();
};

void CCreatureAction_TeleportOut::FinishAction()
{
    PCreature.GetPItem()->GetPhysics()->SaveState(m_savedState);
    if (IS_THING_ALIVE(PTarget)) {
        CThing* target = PTarget.GetPItem();
        const C3DVector& from = PCreature.GetPItem()->GetPosition();
        const C3DVector& to = target->GetPosition();
        C3DVector delta;
        delta.X = to.X - from.X;
        delta.Y = to.Y - from.Y;
        CPhysicsComponent* physics = PCreature.GetPItem()->m_pPhysics;
        float heading = (float)atan2(delta.X, delta.Y) * 0.15915494f;
        if (heading < 0.0f || heading >= 1.0f) {
            heading = (float)fmod((double)heading, 1.0);
            if (heading < 0.0f)
                heading += 1.0f;
        }
        physics->SetHeading(heading);
    }
    CThing* creature = PCreature.GetPItem();
    CCombatActionBase::FinishAction();
    creature->SetCurrentAction(CCreatureAction_TeleportIn(creature, m_flag));
}
