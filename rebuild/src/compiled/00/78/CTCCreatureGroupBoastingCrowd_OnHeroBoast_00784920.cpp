// CTCCreatureGroupBoastingCrowd::OnHeroBoast(const CBoast* boast) -- the hero boasted: advance the encouragement state (2 -> 3 -> 4), schedule leaving the post-boast state 5..10 seconds ahead, and enter crowd state 6 for a flagged boast or 5 otherwise (recording the latter)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00784920, length 160 bytes.
// Callees: CTCBase::GetPRandomSeed @ 0x00686DA0, GFFloatRandom(float, ulong*) @ 0x004978A0 (__fastcall: float on the stack,
// seed in ecx), GFGetCurrentFrame @ 0x0049D870 (__fastcall), __ftol2 @ 0x00BFEA70, EnterCrowdState @ 0x00784260.
// Globals/constants: frames per second @ 0x01375550, 0.5f @ 0x0122F59C, 10.0f @ 0x0126B45C.
// Caller: CTCBoastingCrowdControl::OnHeroBoast (landed 0x00783C50) forwards its 4-byte argument; here it is a pointer to a
// record whose byte at +0x18 selects state 6 (its class is not established, so the member is named descriptively).
// Layout: retail = engine/CTCCreatureGroupBoastingCrowd.h - 4 (12-byte retail member map): CurrentEncouragementState +0x30,
// HeroBoasted +0x34, HeroMadeGoodBoast +0x35, FrameToLeavePostBoastState +0x44.
// Shape: the frame rate is read into a local before the seed call (it feeds `fimul` from the frame); the delay is a named
// float local spilled across the frame call; the state advance is a switch (`sub eax,2 ; je ; dec eax ; jne`); the bool result of the boast test feeds `setne ; add ecx,5`.
long __fastcall GFGetCurrentFrame();                                // 0x0049D870
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0
extern long g_framesPerSecond;                                      // 0x01375550

struct CBoast {
    unsigned char _pad00[0x18];
    bool m_flag18;                              // +0x18
};
struct CTCCreatureGroupBoastingCrowd {
    virtual void v00();
    unsigned char _pad04[0x2c - 0x04];          // CTCBase, CTCCreatureGroup, PHero
    long CrowdState;                            // +0x2c
    long CurrentEncouragementState;             // +0x30
    bool HeroBoasted;                           // +0x34
    bool HeroMadeGoodBoast;                     // +0x35
    long CurrentOpinionDeedHandle;              // +0x38
    long HeroOpinionSourcesPushed;              // +0x3c
    long FrameToClearRemainingOpinionSources;   // +0x40
    long FrameToLeavePostBoastState;            // +0x44
    unsigned long* GetPRandomSeed();            // 0x00686DA0 (CTCBase)
    void EnterCrowdState(long state);           // 0x00784260
    void OnHeroBoast(const CBoast* boast);
};

void CTCCreatureGroupBoastingCrowd::OnHeroBoast(const CBoast* boast)
{
    HeroBoasted = true;
    switch (CurrentEncouragementState) {
    case 2:
        CurrentEncouragementState = 3;
        break;
    case 3:
        CurrentEncouragementState = 4;
        break;
    }
    long framesPerSecond = g_framesPerSecond;
    float delay = (GFFloatRandom(0.5f, GetPRandomSeed()) + 0.5f) * framesPerSecond * 10.0f;
    FrameToLeavePostBoastState = (long)(GFGetCurrentFrame() + delay);
    bool flagged;
    if (boast && boast->m_flag18)
        flagged = true;
    else {
        flagged = false;
        HeroMadeGoodBoast = true;
    }
    EnterCrowdState(flagged ? 6 : 5);
}