// CAIStateGroup_Freeze::Update() -- the frozen creature does nothing until the group can no longer run.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009224F0, length 34 bytes.
// Callees: CAIStateGroupBase::AIAction_DoNothing @ 0x0097E200, the virtual IsRunnable (slot 11, +0x2c of
// the CAIStateGroupBase vtable 0x01288C60 -> 0x008FD282 in the base), CAIStateGroupBase::Terminate @
// 0x008FCF40 (`Terminated = true`, manifest label "CXMVPlaybackInfo" is propagated), tail-jumped.
//
// Shape: `while (AIAction_DoNothing() == AI_ACTION_FINISHED (4) && IsRunnable()) ;` then Terminate(): the
// loop head is the DoNothing call, the `!= 4` exit and the virtual test both fall out to the tail call.
// EAIActionReturn 4 is the "action completed" code used by every AIAction_* helper.
typedef long EAIActionReturn;
enum { AI_ACTION_FINISHED = 4 };

struct CAIStateGroupBase {
    virtual void v00();                         // slot 0  (deleting destructor)
    virtual void v01();                         // slot 1  Initialise
    virtual void v02();                         // slot 2  OnActivate
    virtual void v03();                         // slot 3  InitCreaturePosition
    virtual void v04();                         // slot 4  AllowPriorityOverlap
    virtual void v05();                         // slot 5  IsStillValid
    virtual void v06();                         // slot 6  GetDebugText
    virtual void v07();                         // slot 7  IsTimingCritical
    virtual void v08();                         // slot 8  GetStateGroupType
    virtual void v09();                         // slot 9  Cleanup
    virtual void v10();                         // slot 10 OnReplace
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    EAIActionReturn AIAction_DoNothing();       // 0x0097E200
    void Terminate();                           // 0x008FCF40
};

struct CAIStateGroup_Freeze : CAIStateGroupBase {
    void Update();
};

void CAIStateGroup_Freeze::Update()
{
    while (AIAction_DoNothing() == AI_ACTION_FINISHED && IsRunnable())
        ;
    Terminate();
}