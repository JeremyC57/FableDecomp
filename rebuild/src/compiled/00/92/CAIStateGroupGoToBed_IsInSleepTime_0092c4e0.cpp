// CAIStateGroup_GoToBed::IsInSleepTime() -- is the world clock inside this creature's sleep window?
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0092C4E0, length 62 bytes.
// Callees: CAIStateGroupBase::GetPWorld @ 0x008FCFC0 (manifest "CPlayer::GetCurrentMode" is a propagated label:
// it returns PBrain->+0x1c->+0x24, the brain's world), CThingAICreature::GetSleepTimes(long*, long*) @ 0x00833390,
// CGameTime::IsTimeOfDayInRange(long, long) @ 0x006BB390 (manifest "Game_IsTimeOfDayInRange", __fastcall on the
// world's clock at CWorld+0x1c).
//
// Shape: the clock pointer is fetched first (kept in edi across the call), the creature's sleep start/end
// come back through two stack out-parameters, and the range test is [start + bedtime delay, end]; the callee's result is returned
// untouched. The sleep-time family (0x0092C4E0 / 0x0092C520 / 0x0092C560) differs only in which delay member
// is added to which bound. No engine header exists for CAIStateGroup_GoToBed; the layout is local.
struct CGameTime {
    bool IsTimeOfDayInRange(long start, long end);   // 0x006BB390
};

class CWorld { public:
    unsigned char _pad[0x1c];
    CGameTime* m_pTime;                         // +0x1c
};

class CThingAICreature { public:
    void GetSleepTimes(long* pStart, long* pEnd);    // 0x00833390
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingAICreature* PCreature;                // +0x18
};

class CAIStateGroupBase { public:
    virtual void v00();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CWorld* GetPWorld();                        // 0x008FCFC0
};

struct CAIStateGroup_GoToBed : CAIStateGroupBase {
    unsigned char PBed[8];                      // +0x14 CIntelligentPointer<CThing>
    long m_sleepTimeOffset;                     // +0x1c
    long m_bedTimeDelay;                        // +0x20 minutes past sleep start (InitStateGroup: 0..9)
    long m_wakeTimeDelay;                       // +0x24 minutes past sleep end
    long m_pad28;                               // +0x28
    long m_state;                               // +0x2c
    bool IsInSleepTime();
};

bool CAIStateGroup_GoToBed::IsInSleepTime()
{
    CGameTime* pTime = GetPWorld()->m_pTime;
    long sleepEnd;
    long sleepStart;
    PBrain->PCreature->GetSleepTimes(&sleepStart, &sleepEnd);
    return pTime->IsTimeOfDayInRange(sleepStart + m_bedTimeDelay, sleepEnd);
}