// CCreatureAction_PlayIntoLoopOutOfAnimation::FinishAction() -- base FinishAction; unless the creature is busy (0x661E70),
// advance the into/loop/out-of state machine by queueing the next stage as a new action
// Leaf: ?FinishAction@CCreatureAction_PlayIntoLoopOutOfAnimation
// Layout: retail CCreatureActionBase is 0xa8 (PDB 0xb4): Stage +0xa8, NumberOfTimesToLoop +0xac, AmountOfTimeToLoop
//   +0xb0, FrameToFinish +0xb4, UseAnimationMovement +0xb8, LoopAnimName +0xbc, OutOfAnimName +0xc0 (landed 0x00842960);
//   base InterruptionGroupID +0x20, InterruptionPriority +0x24.
// Shape: a switch on Stage; the next-loop count is a named local computed before the argument list (its dec runs before
//   both GetPItem calls); the first temporary's implicit dtor is called out of line (ICF'd at 0x0057A545), the later two
//   are inlined (two ~CCharString + ~CCreatureActionBase).
class CCharString { public:
    void* m_pData;
    ~CCharString();                                   // 0x0099EAE0
};
class CCreatureActionBase;
class CThingCreatureBase { public:
    bool IsBusy() const;                              // 0x00661E70 (manifest: CanCollideWithLandscape)
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};
long GetWorldFrame();                                 // 0x0049D870 (manifest: GFGetPreMainMemoryUsed)
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;
    CThingCreatureBase* GetPItem() const;             // 0x00A01B50
};
class CCreatureActionBase { public:                          // retail sizeof 0xa8
    virtual ~CCreatureActionBase();                   // 0x00693EF0
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;                // +0x08
    unsigned char _pad10[0x20 - 0x10];
    long InterruptionGroupID;                         // +0x20
    long InterruptionPriority;                        // +0x24
    unsigned char _pad28[0xa8 - 0x28];
    void FinishAction();                              // 0x00694430
    CThingCreatureBase* GetCreature() const { return PCreature.GetPItem(); }
};
class CCreatureAction_PlayIntoLoopOutOfAnimation : public CCreatureActionBase { public:
    long        Stage;                                // +0xa8
    long        NumberOfTimesToLoop;                  // +0xac
    float       AmountOfTimeToLoop;                   // +0xb0
    long        FrameToFinish;                        // +0xb4
    bool        UseAnimationMovement;                 // +0xb8
    CCharString LoopAnimName;                         // +0xbc
    CCharString OutOfAnimName;                        // +0xc0
    CCreatureAction_PlayIntoLoopOutOfAnimation(CThingCreatureBase* owner, const CCharString& loopAnimName,
        const CCharString& outOfAnimName, long loops, float time, bool useAnimationMovement, long group, long priority);   // 0x008428B0
    CCreatureAction_PlayIntoLoopOutOfAnimation(CThingCreatureBase* owner, const CCharString& animName,
        bool useAnimationMovement, long group, long priority);   // 0x00842960
    void FinishAction();
};

void CCreatureAction_PlayIntoLoopOutOfAnimation::FinishAction()
{
    CCreatureActionBase::FinishAction();
    if (GetCreature()->IsBusy())
        return;
    switch (Stage) {
    case 0:
        GetCreature()->SetCurrentAction(CCreatureAction_PlayIntoLoopOutOfAnimation(GetCreature(), LoopAnimName, OutOfAnimName,
            NumberOfTimesToLoop, (float)FrameToFinish, UseAnimationMovement, InterruptionGroupID, InterruptionPriority));
        break;
    case 1:
        if (NumberOfTimesToLoop <= 0 && GetWorldFrame() >= FrameToFinish)
            GetCreature()->SetCurrentAction(CCreatureAction_PlayIntoLoopOutOfAnimation(GetCreature(), OutOfAnimName,
                UseAnimationMovement, InterruptionGroupID, InterruptionPriority));
        else {
            long loops = NumberOfTimesToLoop - 1;
            GetCreature()->SetCurrentAction(CCreatureAction_PlayIntoLoopOutOfAnimation(GetCreature(), LoopAnimName, OutOfAnimName,
                loops, (float)FrameToFinish, UseAnimationMovement, InterruptionGroupID, InterruptionPriority));
        }
        break;
    }
}