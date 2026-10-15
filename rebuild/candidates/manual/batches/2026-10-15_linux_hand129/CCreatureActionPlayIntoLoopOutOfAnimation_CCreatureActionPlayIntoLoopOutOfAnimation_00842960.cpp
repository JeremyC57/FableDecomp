// CCreatureAction_PlayIntoLoopOutOfAnimation::CCreatureAction_PlayIntoLoopOutOfAnimation(CThingCreatureBase*, const CCharString&, bool, long, long) -- base on the named animation, stage 2 (straight to the out-of stage), the movement flag, empty loop / out-of names, no anim delay
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00842960, length 137 bytes. Vtable 0x01273B2C.
// Callees: the creature's animation lookup by name @ 0x006924B0 (`__fastcall`), CCreatureActionBase::CCreatureActionBase(CThingCreatureBase*, float,
// float, CAnimationEntry*, const CSoundPair&, long, long) @ 0x00693B30, CCharString::CCharString() @ 0x0099E4B0 (twice).
// Shape: `ret 14h`; the zeroed sound pair is written with immediates (no spare zero register); Stage and UseAnimationMovement are
// initialiser-list entries, the two strings default-constructed members, and the base's AnimDelayTime (+0x18) is cleared in the body
// (its store follows the member constructors).
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
    unsigned char _pad04[0x18 - 4];
    long AnimDelayTime;                     // +0x18
    unsigned char _pad1c[0xa8 - 0x1c];
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
    CCreatureAction_PlayIntoLoopOutOfAnimation(CThingCreatureBase* owner, const CCharString& animName, bool useAnimationMovement, long c, long d);
};

CCreatureAction_PlayIntoLoopOutOfAnimation::CCreatureAction_PlayIntoLoopOutOfAnimation(CThingCreatureBase* owner, const CCharString& animName,
        bool useAnimationMovement, long c, long d)
    : CCreatureActionBase(owner, -1.0f, 1.0f, GetAnimationEntry(owner, animName), CSoundPair(), c, d)
    , Stage(2)
    , UseAnimationMovement(useAnimationMovement)
{
    AnimDelayTime = 0;
}
