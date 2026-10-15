// CCreatureAction_PlayIntoLoopOutOfAnimation::CCreatureAction_PlayIntoLoopOutOfAnimation(CThingCreatureBase*, const CCharString&, const CCharString&, const CCharString&, long, bool, long, long) -- base on the "into" animation, then stage 0, the loop count, the movement flag and copies of the loop / out-of animation names
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00842740, length 148 bytes (+ int3 padding). Vtable 0x01273B2C.
// Callees: the creature's animation lookup by name @ 0x006924B0 (`__fastcall`), CCreatureActionBase::CCreatureActionBase(CThingCreatureBase*, float,
// float, CAnimationEntry*, const CSoundPair&, long, long) @ 0x00693B30, CCharString::CCharString(const CCharString&) @ 0x0099EC30 (twice).
// Shape: `ret 20h`; the zeroed sound pair shares ebx with the later zero stores; Stage / NumberOfTimesToLoop / FrameToFinish /
// UseAnimationMovement and the two string members are initialiser-list entries (AmountOfTimeToLoop is left alone); the CCharString
// class is modelled locally so its copy constructor stays out of line.
struct CThingCreatureBase;
struct CThing;
struct CAnimationEntry;
struct CSoundPair {                         // two sound ids, zeroed by the inline constructor (register-zeroed stack stores)
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
struct CCreatureActionBase {                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    CCreatureActionBase(CThingCreatureBase* owner, float a, float b, CAnimationEntry* animation, const CSoundPair& sound, long c, long d);   // 0x00693B30
};
struct CCharString {                        // modelled locally: retail calls the out-of-line copy constructor
    void* m_pData;
    CCharString();                          // 0x0099E4B0
    CCharString(const CCharString& other);  // 0x0099EC30
    ~CCharString();                         // 0x0099EAE0
};
CAnimationEntry* __fastcall GetAnimationEntry(CThingCreatureBase* owner, const CCharString& name);   // 0x006924B0

struct CCreatureAction_PlayIntoLoopOutOfAnimation : CCreatureActionBase {   // vtable 0x01273B2C; members = PDB names (PDB +0xb4.. sit 0xc high)
    long        Stage;                      // +0xa8 EStage
    long        NumberOfTimesToLoop;        // +0xac
    float       AmountOfTimeToLoop;         // +0xb0
    long        FrameToFinish;              // +0xb4
    bool        UseAnimationMovement;       // +0xb8
    CCharString LoopAnimName;               // +0xbc
    CCharString OutOfAnimName;              // +0xc0
    virtual void v00();
    CCreatureAction_PlayIntoLoopOutOfAnimation(CThingCreatureBase* owner, const CCharString& intoAnimName, const CCharString& loopAnimName,
                                               const CCharString& outOfAnimName, long numberOfTimesToLoop, bool useAnimationMovement, long c, long d);
};

CCreatureAction_PlayIntoLoopOutOfAnimation::CCreatureAction_PlayIntoLoopOutOfAnimation(CThingCreatureBase* owner, const CCharString& intoAnimName,
        const CCharString& loopAnimName, const CCharString& outOfAnimName, long numberOfTimesToLoop, bool useAnimationMovement, long c, long d)
    : CCreatureActionBase(owner, -1.0f, 1.0f, GetAnimationEntry(owner, intoAnimName), CSoundPair(), c, d)
    , Stage(0)
    , NumberOfTimesToLoop(numberOfTimesToLoop)
    , FrameToFinish(0)
    , UseAnimationMovement(useAnimationMovement)
    , LoopAnimName(loopAnimName)
    , OutOfAnimName(outOfAnimName)
{
}
