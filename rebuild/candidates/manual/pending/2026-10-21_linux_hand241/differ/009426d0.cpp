// CAIStateGroup_WorkAtSchool::PlayLoop(float, const CCharString&, const CCharString&, const CCharString&) -- play the into animation, repeat the loop animation until `duration` seconds have passed, then the out-of animation; false as soon as one of them does not complete
// Grade: DIFFER(ours v retail) -- 328v322: retail places the shared `xor al,al` false exit (its own epilogue) right after the first action test (`je` over it; the loop failure jumps back to it); the lane puts that block after the final `sete al` exit (`jne` forward). Code otherwise identical.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009426D0, length 322 bytes (`ret 10h`, bool). Called by the landed CrouchAndTalk
//          @ 0x00942820 / Teach / StandAndTalk wrappers.
// Callees: GetWorldFrame @ 0x0049D870 (static; manifest "GFGetPreMainMemoryUsed"), _ftol @ 0x00BFEA70, the CCreatureAction_
//          PlayAnimation ctor @ 0x008424F0 (creature, name, sound pair, five flags; the outer call's `false` is pushed first), CAIStateGroupBase::AIAction_
//          PerformActionAndWait @ 0x0097DB70, ~CCreatureActionBase @ 0x00693EF0; data: the frame rate @ 0x01375550.
// Shape: the frame rate is read into a local before GetWorldFrame (its slot is later reused by the end frame, which VC keeps
//          in ebp and spills); the end frame is `(long)(rate * duration + frame)` (VC7.1 evaluates the right operand of a
//          float `+` first, so the source lists the product first to get retail's `fild frame ; fild rate ; fmul`); each
//          animation is an action temporary built in the call (sound pair zeroed in the dead frame slot) whose `!= 2`
//          result lands in bl before the destructor; the middle one runs in a rotated `while (GetWorldFrame() < end)`;
//          the last result is returned through `sete al`.
// Layout: engine/CAIStateGroup_WorkAtSchool.h layout (no members used) over the landed CAIStateGroupBase model;
//          CCreatureAction_PlayAnimation is 0xb4 bytes (base 0xa8 + 0xc), as in the landed Wander/ChickenWander rows.
struct CThingCreatureBase;
struct CCharString { void* PStringData; };

struct CSoundPair {
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0xa4];
    ~CCreatureActionBase();                     // 0x00693EF0
};
struct CCreatureAction_PlayAnimation : CCreatureActionBase {
    unsigned char _anim[0x0c];
    CCreatureAction_PlayAnimation(CThingCreatureBase* creature, const CCharString& animation, const CSoundPair& sound,
        bool a, bool b, bool c, long d, long e);   // 0x008424F0
};

struct CWorld {
    static long GetWorldFrame();                // 0x0049D870
};
extern long g_FrameRate;                        // 0x01375550

struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool stopWhenReplaced);   // 0x0097DB70
};

struct CAIStateGroup_WorkAtSchool : CAIStateGroupBase {
    bool PlayLoop(float duration, const CCharString& into, const CCharString& loop, const CCharString& outOf);
};

bool CAIStateGroup_WorkAtSchool::PlayLoop(float duration, const CCharString& into, const CCharString& loop, const CCharString& outOf)
{
    long rate = g_FrameRate;
    long frame = CWorld::GetWorldFrame();
    long endFrame = (long)((float)rate * duration + (float)frame);
    bool failed = AIAction_PerformActionAndWait(&CCreatureAction_PlayAnimation(GetCreature(), into, CSoundPair(),
        false, true, false, 0, 0), false) != 2;
    if (failed)
        return false;
    while (CWorld::GetWorldFrame() < endFrame) {
        failed = AIAction_PerformActionAndWait(&CCreatureAction_PlayAnimation(GetCreature(), loop, CSoundPair(),
            false, true, false, 0, 0), false) != 2;
        if (failed)
            return false;
    }
    failed = AIAction_PerformActionAndWait(&CCreatureAction_PlayAnimation(GetCreature(), outOf, CSoundPair(),
        false, true, false, 0, 0), false) != 2;
    return !failed;
}
