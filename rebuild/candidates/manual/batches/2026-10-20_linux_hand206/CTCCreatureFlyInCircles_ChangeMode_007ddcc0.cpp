// CTCCreatureFlyInCircles::ChangeMode() -- start the hover<->fly transition animation for the current fly mode; when the creature takes the action, flag the transition, switch the mode and spread the speed change over the action's duration
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007DDCC0, 212 bytes (plain `ret`, 0x128-byte frame: the animation name, the
// target speed, the duration temporary and the 0x11c-byte action).
// Callees: CCharString() @ 0x0099E4B0, CCharString::operator=(const char*) @ 0x0099EFE0, ~CCharString @ 0x0099EAE0,
// CCreatureAction_DragonFlight(CThingCreatureBase*, const CCharString&) @ 0x008F4CB0 (vtable 0x01288544; manifest
// "CCreatureAction_CastTimeSpell" is propagated), CThingCreatureBase::SetCurrentAction @ 0x006644F0, the action's TotalTime
// getter CCreatureActionBase::GetTotalTime @ 0x006925B0 (`mov eax,[ecx+10h]`; manifest "CWorld::DrawGetEnvironment" is propagated), ~CCombatActionBase
// @ 0x0062E900. Literals "HOVER_INTO_FLY" @ 0x0126FAA8, "FLY_INTO_HOVER" @ 0x0126FAB8.
// Layout: CTCCreatureFlyInCircles (Ego_r.pdb, no containers before these): Creature +0x0c, Speed +0x1c, DeltaSpeed +0x20,
// SpeedChangeTicks +0x24, Mode +0x34, Transitioning +0x38, ModeChangeQueued +0x39.
// Shape: the mode switch's case bodies are laid out in source order (2, 1, 0; `sub eax,edi ; je / dec ; je / dec ; jne`, the
// zero register shared with the new-mode and target-speed initialisers); the action is a named local destroyed through the
// out-of-line ~CCombatActionBase (the derived class is `__declspec(novtable)` with no dtor of its own); the duration is a
// long local so the division is a `fidiv` of its frame copy.
struct CCharString {
    void* PStringData;
    CCharString();                                      // 0x0099E4B0
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const char* text);           // 0x0099EFE0
};
struct CThingCreatureBase;
struct CCreatureActionBase {                            // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body04[0x10 - 0x04];
    long TotalTime;                                     // +0x10
    unsigned char _body14[0xa8 - 0x14];
    long GetTotalTime() const;                          // 0x006925B0
};
struct CCombatActionBase : CCreatureActionBase {        // retail sizeof 0x114
    unsigned char _bodyA8[0x114 - 0xa8];
    ~CCombatActionBase();                               // 0x0062E900
};
struct __declspec(novtable) CCreatureAction_DragonFlight : CCombatActionBase {
    unsigned char FaceTarget;                           // +0x114
    float         TurnRate;                             // +0x118
    CCreatureAction_DragonFlight(CThingCreatureBase* owner, const CCharString& animName);   // 0x008F4CB0
};
struct CThingCreatureBase {
    bool SetCurrentAction(CCreatureActionBase* action); // 0x006644F0
};
struct CTCCreatureFlyInCircles {
    virtual void v00();
    unsigned char _pad04[0x0c - 0x04];
    CThingCreatureBase* Creature;                       // +0x0c
    float Centre[3];                                    // +0x10 C3DVector
    float Speed;                                        // +0x1c
    float DeltaSpeed;                                   // +0x20
    long  SpeedChangeTicks;                             // +0x24
    float CurrentRadius;                                // +0x28
    float DeltaRadius;                                  // +0x2c
    long  RadiusChangeTicks;                            // +0x30
    long  Mode;                                         // +0x34 EFlyMode
    bool  Transitioning;                                // +0x38
    bool  ModeChangeQueued;                             // +0x39
    void ChangeMode();
};

void CTCCreatureFlyInCircles::ChangeMode()
{
    CCharString animation;
    long newMode = 0;
    float targetSpeed = 0.0f;
    switch (Mode) {
    case 2:
        return;
    case 1:
        animation = "FLY_INTO_HOVER";
        newMode = 0;
        targetSpeed = 1.0f;
        break;
    case 0:
        animation = "HOVER_INTO_FLY";
        newMode = 1;
        targetSpeed = 3.0f;
        break;
    }
    CCreatureAction_DragonFlight action(Creature, animation);
    if (Creature->SetCurrentAction(&action)) {
        Transitioning = true;
        ModeChangeQueued = false;
        Mode = newMode;
        long time = action.GetTotalTime();
        DeltaSpeed = (targetSpeed - Speed) / time;
        SpeedChangeTicks = time;
    }
}
