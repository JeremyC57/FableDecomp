// CCreatureAction_PlayIntoLoopOutOfAnimation::CCreatureAction_PlayIntoLoopOutOfAnimation(const CCreatureAction_PlayIntoLoopOutOfAnimation&) -- copy constructor: base copy, the stage / loop settings, then copies of the two animation names
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084C3E0, length 123 bytes (+ int3 padding). Vtable 0x01273B2C.
// Callees: CCreatureActionBase::CCreatureActionBase(const CCreatureActionBase&) @ 0x0062EBC0, CCharString::CCharString(const CCharString&) @ 0x0099EC30 (twice).
// Shape: `ret 4`; the explicit memberwise copy constructor in declaration order (four dwords, the flag byte, the two strings).
struct CCharString {                        // modelled locally: retail calls the out-of-line copy constructor
    void* m_pData;
    CCharString();                          // 0x0099E4B0
    CCharString(const CCharString& other);  // 0x0099EC30
    ~CCharString();                         // 0x0099EAE0
};
struct CCreatureActionBase {                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    CCreatureActionBase(const CCreatureActionBase& other);   // 0x0062EBC0
};

struct CCreatureAction_PlayIntoLoopOutOfAnimation : CCreatureActionBase {   // vtable 0x01273B2C; members = PDB names (PDB +0xb4.. sit 0xc high)
    long        Stage;                      // +0xa8 EStage
    long        NumberOfTimesToLoop;        // +0xac
    float       AmountOfTimeToLoop;         // +0xb0
    long        FrameToFinish;              // +0xb4
    bool        UseAnimationMovement;       // +0xb8
    CCharString LoopAnimName;               // +0xbc
    CCharString OutOfAnimName;              // +0xc0
    virtual void v00();
    CCreatureAction_PlayIntoLoopOutOfAnimation(const CCreatureAction_PlayIntoLoopOutOfAnimation& other);
};

CCreatureAction_PlayIntoLoopOutOfAnimation::CCreatureAction_PlayIntoLoopOutOfAnimation(const CCreatureAction_PlayIntoLoopOutOfAnimation& other)
    : CCreatureActionBase(other)
    , Stage(other.Stage)
    , NumberOfTimesToLoop(other.NumberOfTimesToLoop)
    , AmountOfTimeToLoop(other.AmountOfTimeToLoop)
    , FrameToFinish(other.FrameToFinish)
    , UseAnimationMovement(other.UseAnimationMovement)
    , LoopAnimName(other.LoopAnimName)
    , OutOfAnimName(other.OutOfAnimName)
{
}
