// NCreatureMode::CModeSitBase::OnActivate() -- base activation, then seed the idle counter with a random frame count derived from the mode's combination-idle delay
// Leaf: ?OnActivate@CModeSitBase
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D6580, 144 bytes, two `ret` tails; CModeSitChair vtable slot 5 (inherited).
// Callees: CModeBase::OnActivate @ 0x0073AAC0 (qualified), virtual GetType (slot 1),
//   CModeBase::GetNoFramesBeforePlayingCombinationIdleAnims(EMode) @ 0x0073A6F0 (as the landed CModeStandBase::AnimUpdate
//   0x00862EC0), _ftol @ 0x00BFEA70 (twice), CModeBase::GetWorld @ 0x0073A680, the world's random seed accessor @ 0x0049D810,
//   GFROR13 @ 0x00497890 (the inlined generator `seed = GFROR13(seed * 0x24A1 + 0x24DF)`).
// Name: FableWin.pdb `?OnActivate@CModeSitBase@NCreatureMode@@UAEXXZ` (the catalog's CModeSitChair::vfunc_5 is inherited).
// Layout: Ego_r.pdb CModeSitBase (0x2c): IdleCounter +0x1c, PlayingIdle +0x20, PRemoveAction +0x24.
// Constants: 0x01238174 = 0.75f, 0x01285670 = -1.5f.
struct CRandomSeed {
    unsigned long Seed;
};
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

struct CWorld {
    CRandomSeed* GetRandomSeed();          // 0x0049D810
};

struct CModeBase {                         // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual void v00();
    virtual unsigned long GetType() const; // slot 1
    void*         ParentThing;             // +0x04
    unsigned char _body[0x14];
    CWorld* GetWorld() const;              // 0x0073A680
    long GetNoFramesBeforePlayingCombinationIdleAnims(unsigned long mode);   // 0x0073A6F0
    void OnActivate();                     // 0x0073AAC0
};

namespace NCreatureMode {
struct CModeSitBase : CModeBase {
    long          IdleCounter;             // +0x1c
    bool          PlayingIdle;             // +0x20
    unsigned char PRemoveAction[8];        // +0x24 CCountedPointer<CCreatureActionBase>
    void OnActivate();
};
}

inline unsigned long GFRandom(CRandomSeed* random)
{
    random->Seed = random->Seed * 0x24a1 + 0x24df;
    random->Seed = GFROR13(random->Seed);
    return random->Seed;
}

void NCreatureMode::CModeSitBase::OnActivate()
{
    CModeBase::OnActivate();
    float frames = (float)GetNoFramesBeforePlayingCombinationIdleAnims(GetType());
    long minFrames = (long)(frames * 0.75f);
    CRandomSeed* random = GetWorld()->GetRandomSeed();
    unsigned long range = -minFrames - (long)(frames * -1.5f);
    unsigned long r = GFRandom(random);
    IdleCounter = (range ? r % range : 0) + minFrames;
}