// CCreatureAction_PlayIntoLoopOutOfAnimation::CCreatureAction_PlayIntoLoopOutOfAnimation(CThingCreatureBase&, const CCharString&, const CCharString&, long, float, bool, long, long) -- the private straight-into-the-loop form: the base action on the loop animation (speeds -1 / 1, empty sound pair), stage 1, the loop count, the finish frame from the float argument, the movement flag, copies of the loop / out-of names, and the base's AnimDelayTime cleared
// Leaf: ??0CCreatureAction_PlayIntoLoopOutOfAnimation@@AAE@AAVCThingCreatureBase@@ABVCCharString@@1JM_NJJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008428B0, 172 bytes, thiscall, `ret 20h`, returns this. Vtable 0x01273B2C.
// Identity (proven): Ego_r.pdb donor order of this class's four ctors (0x0051CC50 ..11J_NJJ, 0x0051CCF0 ..11M_NJJ,
//   0x0051CDD0 private ..1JM_NJJ, 0x0051CE90 private .._NJJ) is retail's order (0x00842740 landed, 0x008427E0, 0x008428B0,
//   0x00842960 landed); this body takes two names, a long (NumberOfTimesToLoop +0xAC) and a float (converted with _ftol2
//   into FrameToFinish +0xB4): the private 1JM_NJJ form. The first name feeds both the base animation and LoopAnimName.
// Callees: CCreatureActionBase::GetRandomAnimFromName 0x006924B0 (protected static __fastcall), CCreatureActionBase::
//   CCreatureActionBase(CThingCreatureBase&, float, float, const CAnimationEntry&, const CSoundPair&, long, long) 0x00693B30,
//   CCharString::CCharString(const CCharString&) 0x0099EC30 (x2), _ftol2.
// Layout: Ego_r.pdb member names, 0xC lower in retail (CCreatureActionBase is 0xA8 bytes here); AnimDelayTime is the base
//   member at +0x18 (the landed 0x00842960 clears it the same way, in the body).
#define CCreatureActionBase CCreatureActionBasePdb
#include "engine/CCreatureActionBase.h"
#undef CCreatureActionBase
#define CThingCreatureBase CThingCreatureBasePdb
#include "engine/CThingCreatureBase.h"
#undef CThingCreatureBase
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
        const CCharString& loopAnimName, const CCharString& outOfAnimName, long numberOfTimesToLoop, float frameToFinish,
        bool useAnimationMovement, long c, long d)
    : CCreatureActionBase(owner, -1.0f, 1.0f, GetRandomAnimFromName(owner, loopAnimName), CSoundPair(), c, d)
    , Stage(STAGE_LOOP)
    , NumberOfTimesToLoop(numberOfTimesToLoop)
    , FrameToFinish((long)frameToFinish)
    , UseAnimationMovement(useAnimationMovement)
    , LoopAnimName(loopAnimName)
    , OutOfAnimName(outOfAnimName)
{
    AnimDelayTime = 0;
}