// CAIStateGroup_WorkAtSchool::PlayLoop(float, const CCharString&, const CCharString&, const CCharString&) -- play the into animation, repeat the loop animation until `duration` seconds have passed, then the out-of animation; false as soon as one of them does not complete
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009426D0, length 322 bytes (`ret 10h`, bool). Called by the landed CrouchAndTalk
//          @ 0x00942820 / Teach / StandAndTalk wrappers.
// Callees: CWorld::GetFrame @ 0x0049D870 (static; manifest "GFGetPreMainMemoryUsed"), _ftol @ 0x00BFEA70, the CCreatureAction_
//          PlayAnimation ctor @ 0x008424F0 (creature, name, sound pair, five flags; the outer call's `false` is pushed first), CAIStateGroupBase::AIAction_
//          PerformActionAndWait @ 0x0097DB70, ~CCreatureActionBase @ 0x00693EF0; data: the frame rate @ 0x01375550.
// Shape: the frame rate is read into a local before GetWorldFrame (its slot is later reused by the end frame, which VC keeps
//          in ebp and spills); the end frame is `(long)(rate * duration + frame)` (VC7.1 evaluates the right operand of a
//          float `+` first, so the source lists the product first to get retail's `fild frame ; fild rate ; fmul`); each
//          animation is an action temporary built in an `if (... != 2) return false;` test (sound pair zeroed in the
//          dead frame slot; the `!= 2` result lands in bl before the destructor); the middle one runs in a rotated
//          `while (GetWorldFrame() < end)`; the function ends `return true;` after the third test, so VC returns
//          through `test bl,bl ; sete al` and lays the shared false exit right after the first test (a final
//          `return !failed;` puts it after the `sete al` exit).
// Layout: engine/CAIStateGroup_WorkAtSchool.h layout (no members used) over the landed CAIStateGroupBase model;
//          CCreatureAction_PlayAnimation is 0xb4 bytes (base 0xa8 + 0xc), as in the landed Wander/ChickenWander rows.
class CThingCreatureBase;
class CCharString { public: void* PStringData; };

class CSoundPair { public:
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
class CCreatureActionBase { public:
    virtual void v00();
    unsigned char _body[0xa4];
    ~CCreatureActionBase();                     // 0x00693EF0
};
class CCreatureAction_PlayAnimation : public CCreatureActionBase { public:
    unsigned char _anim[0x0c];
    CCreatureAction_PlayAnimation(CThingCreatureBase* creature, const CCharString& animation, const CSoundPair& sound,
        bool a, bool b, bool c, long d, long e);   // 0x008424F0
};

class CWorld { public:
    static long __fastcall GetFrame();                // 0x0049D870
};
extern long g_FrameRate;                        // 0x01375550

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};

class CAIStateGroupBase { public:
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
    long frame = CWorld::GetFrame();
    long endFrame = (long)((float)rate * duration + (float)frame);
    if (AIAction_PerformActionAndWait(&CCreatureAction_PlayAnimation(GetCreature(), into, CSoundPair(),
        false, true, false, 0, 0), false) != 2)
        return false;
    while (CWorld::GetFrame() < endFrame) {
        if (AIAction_PerformActionAndWait(&CCreatureAction_PlayAnimation(GetCreature(), loop, CSoundPair(),
            false, true, false, 0, 0), false) != 2)
            return false;
    }
    if (AIAction_PerformActionAndWait(&CCreatureAction_PlayAnimation(GetCreature(), outOf, CSoundPair(),
        false, true, false, 0, 0), false) != 2)
        return false;
    return true;
}

// ---- test harness ----
#include <stdio.h>
#include <string.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
long g_FrameRate = 10;
static long g_frame = 100, g_step = 5;
long CWorld::GetFrame() { long f = g_frame; g_frame += g_step; return f; }
static const CCharString* g_anims[64]; static int g_nanims = 0, g_live = 0, g_failAt = -1, g_calls = 0;
static bool g_flagsOk = true;
void CCreatureActionBase::v00() { }
CCreatureActionBase::~CCreatureActionBase() { --g_live; }
CCreatureAction_PlayAnimation::CCreatureAction_PlayAnimation(CThingCreatureBase* creature, const CCharString& animation,
    const CSoundPair& sound, bool a, bool b, bool c, long d, long e)
{
    ++g_live; g_anims[g_nanims++ & 63] = &animation;
    if (creature != (CThingCreatureBase*)0x1234 || sound.m_first || sound.m_second || a || !b || c || d || e) g_flagsOk = false;
}
CAIStateGroupBase::~CAIStateGroupBase() { }
long CAIStateGroupBase::AIAction_PerformActionAndWait(CCreatureActionBase*, bool stop)
{ if (stop) g_flagsOk = false; return g_calls++ == g_failAt ? 3 : 2; }

static void reset(long step, int failAt) { g_frame = 100; g_step = step; g_nanims = 0; g_calls = 0; g_failAt = failAt; }

int main()
{
    static CAIBrain brain; brain.PCreature = (CThingCreatureBase*)0x1234;
    static CAIStateGroup_WorkAtSchool sg; sg.PBrain = &brain;
    CCharString into = { 0 }, loop = { 0 }, outOf = { 0 };
    // duration 2s at 10 fps: end frame 120; frames advance by 5 per GetWorldFrame call
    reset(5, -1);
    CHECK(sg.PlayLoop(2.0f, into, loop, outOf));
    CHECK(g_nanims == 5 && g_anims[0] == &into && g_anims[1] == &loop && g_anims[3] == &loop && g_anims[4] == &outOf);   // loops at 105, 110, 115
    CHECK(g_live == 0 && g_flagsOk);
    reset(5, 0);                                   // into fails
    CHECK(!sg.PlayLoop(2.0f, into, loop, outOf) && g_nanims == 1 && g_live == 0);
    reset(5, 2);                                   // second loop fails
    CHECK(!sg.PlayLoop(2.0f, into, loop, outOf) && g_nanims == 3 && g_anims[2] == &loop && g_live == 0);
    reset(5, 4);                                   // outOf fails
    CHECK(!sg.PlayLoop(2.0f, into, loop, outOf) && g_nanims == 5 && g_live == 0);
    reset(50, -1);                                 // no loop iterations
    CHECK(sg.PlayLoop(2.0f, into, loop, outOf) && g_nanims == 2 && g_anims[1] == &outOf);
    CHECK(g_flagsOk);
    if (fails == 0) printf("009426d0_TEST PASS\n");
    return fails;
}