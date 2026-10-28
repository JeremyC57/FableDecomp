// CCreatureAction_PlayIntoLoopOutOfAnimation::CCreatureAction_PlayIntoLoopOutOfAnimation(CThingCreatureBase&, const CCharString&, const CCharString&, const CCharString&, float, bool, long, long) -- the timed-loop form: the base action on the "into" animation (speeds -1 / 1, empty sound pair), stage 0, no loop count, copies of the loop / out-of names, and the finish frame = now + frames-per-second * loop time (the time is then cleared)
// Leaf: ??0CCreatureAction_PlayIntoLoopOutOfAnimation@@QAE@AAVCThingCreatureBase@@ABVCCharString@@11M_NJJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008427E0, 200 bytes, thiscall, `ret 20h`, returns this. Vtable 0x01273B2C.
// Identity (proven): Ego_r.pdb donor order has four ctors of this class, 0x0051CC50 (..11J_NJJ, the landed 0x00842740),
//   0x0051CCF0 (..11M_NJJ), 0x0051CDD0 (private ..1JM_NJJ) and 0x0051CE90 (private .._NJJ, the landed 0x00842960); retail keeps
//   that order (0x00842740, 0x008427E0, 0x008428B0, 0x00842960). This body stores its fifth argument into the float
//   AmountOfTimeToLoop (+0xB0) and scales it by an fild'ed long: the M form. Both rows store vtable 0x01273B2C.
// Callees: CCreatureActionBase::GetRandomAnimFromName 0x006924B0 (protected static __fastcall), CCreatureActionBase::
//   CCreatureActionBase(CThingCreatureBase&, float, float, const CAnimationEntry&, const CSoundPair&, long, long) 0x00693B30,
//   CCharString::CCharString(const CCharString&) 0x0099EC30 (x2), CWorld::GetFrame() 0x0049D870 (static __fastcall:
//   Ego_r/FableWin ?GetFrame@CWorld@@SIJXZ, donor-adjacent to GetFrameAsString as retail 0x0049D870 / 0x0049D880), _ftol2.
// Globals: 0x01375550, the game's frames-per-second long (descriptive name; no PDB data symbol).
// Layout: Ego_r.pdb members Stage / NumberOfTimesToLoop / AmountOfTimeToLoop / FrameToFinish / UseAnimationMovement /
//   LoopAnimName / OutOfAnimName at PDB +0xB4.., 0xC lower in retail (CCreatureActionBase is 0xA8 bytes here).
#define CCreatureActionBase CCreatureActionBasePdb
#include "engine/CCreatureActionBase.h"
#undef CCreatureActionBase
#define CThingCreatureBase CThingCreatureBasePdb
#include "engine/CThingCreatureBase.h"
#undef CThingCreatureBase
#define CWorld CWorldPdb
#include "engine/CWorld.h"
#undef CWorld
class CAnimationEntry;
class CCharString {
public:
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                          // 0x0099EAE0
    void* m_pStringData;
};
class CSoundPair {
public:
    CSoundPair() : FirstSound(0), LastSound(0) { }
    long FirstSound;
    long LastSound;
};
class CThingCreatureBase {
public:
    virtual ~CThingCreatureBase();
};
class CWorld {
public:
    static long __fastcall GetFrame();                       // 0x0049D870
};
extern long g_framesPerSecond;                               // 0x01375550 (descriptive)
class CCreatureActionBase {
public:
    CCreatureActionBase(CThingCreatureBase& creature, float a, float b, const CAnimationEntry& anim, const CSoundPair& sound,
                        long c, long d);                     // 0x00693B30
    virtual ~CCreatureActionBase();
    unsigned char _pad04[0x18 - 4];
    long AnimDelayTime;                                      // +0x18
    unsigned char _pad1c[0xa8 - 0x1c];                       // retail sizeof(CCreatureActionBase) 0xA8
protected:
    static const CAnimationEntry& __fastcall GetRandomAnimFromName(const CThingCreatureBase& creature,
                                                                const CCharString& name);   // 0x006924B0
};
class CCreatureAction_PlayIntoLoopOutOfAnimation : public CCreatureActionBase {
public:
    enum EStage { STAGE_INTO, STAGE_LOOP, STAGE_OUT_OF };
    CCreatureAction_PlayIntoLoopOutOfAnimation(CThingCreatureBase& owner, const CCharString& intoAnimName,
        const CCharString& loopAnimName, const CCharString& outOfAnimName, long numberOfTimesToLoop, bool useAnimationMovement,
        long c, long d);
    CCreatureAction_PlayIntoLoopOutOfAnimation(CThingCreatureBase& owner, const CCharString& intoAnimName,
        const CCharString& loopAnimName, const CCharString& outOfAnimName, float amountOfTimeToLoop, bool useAnimationMovement,
        long c, long d);
private:
    CCreatureAction_PlayIntoLoopOutOfAnimation(CThingCreatureBase& owner, const CCharString& loopAnimName,
        const CCharString& outOfAnimName, long numberOfTimesToLoop, float frameToFinish, bool useAnimationMovement, long c, long d);
    CCreatureAction_PlayIntoLoopOutOfAnimation(CThingCreatureBase& owner, const CCharString& animName, bool useAnimationMovement,
        long c, long d);
public:
    EStage      Stage;                                       // +0xA8
    long        NumberOfTimesToLoop;                         // +0xAC
    float       AmountOfTimeToLoop;                          // +0xB0
    long        FrameToFinish;                               // +0xB4
    bool        UseAnimationMovement;                        // +0xB8
    CCharString LoopAnimName;                                // +0xBC
    CCharString OutOfAnimName;                               // +0xC0
};

CCreatureAction_PlayIntoLoopOutOfAnimation::CCreatureAction_PlayIntoLoopOutOfAnimation(CThingCreatureBase& owner,
        const CCharString& intoAnimName, const CCharString& loopAnimName, const CCharString& outOfAnimName,
        float amountOfTimeToLoop, bool useAnimationMovement, long c, long d)
    : CCreatureActionBase(owner, -1.0f, 1.0f, GetRandomAnimFromName(owner, intoAnimName), CSoundPair(), c, d)
    , Stage(STAGE_INTO)
    , NumberOfTimesToLoop(0)
    , AmountOfTimeToLoop(amountOfTimeToLoop)
    , UseAnimationMovement(useAnimationMovement)
    , LoopAnimName(loopAnimName)
    , OutOfAnimName(outOfAnimName)
{
    long framesPerSecond = g_framesPerSecond;
    FrameToFinish = (long)(framesPerSecond * AmountOfTimeToLoop + CWorld::GetFrame());
    AmountOfTimeToLoop = 0;
}