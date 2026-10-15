// CTCAmbientBird::OnInitialActivate() -- unless the game subsystem at main-component +0x1c reports so through its slot 9, the bird's thing is killed with probability one half (a world-seeded random in [0,1) above 0.5)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E1F70, length 70 bytes.
// Leaf: OnInitialActivate
// Callees: the CTCBase accessor @ 0x00686D60 (`[g_pMainGameComponent]+0x1c`; its virtual slot 9 returns a bool -- the object
// is not identified, the manifest's "CViewBase::GetDisplayEngine" is propagated), CTCBase::GetWorld @ 0x00686A70
// (`+0x24`, as the landed 0x00943AE0 family), CWorld::GetPRandomSeed @ 0x0049D810, GFFloatRandom(float, unsigned long*)
// @ 0x004978A0 (`__fastcall`: the seed in ecx, the float on the stack), CThing::Kill(bool) @ 0x004C9B80. Float pool
// 0x0122F59C = 0.5f. Ego_r.pdb: `?OnInitialActivate@CTCAmbientBird@@UAEXXZ`.
// Shape: `fcomp [0.5] ; fnstsw ax ; test ah,41h ; jne` is the `> 0.5f` test; the two conditions are nested ifs.
struct CGameSubsystem {                        // the main game component's member at +0x1c (unidentified)
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool IsSuppressed();               // slot 9 (+0x24)
};

struct CWorld {
    unsigned long* GetPRandomSeed();           // 0x0049D810
};

float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0

struct CThing {
    void Kill(bool immediately);               // 0x004C9B80
};

struct CTCBase {                               // engine/CTCBase.h layout, modelled polymorphic
    virtual void v00();
    CThing* ParentThing;                       // +0x04
    bool    ToBeRemoved;                       // +0x08
    bool    IsInGlobalUpdateTCs;               // +0x09
    CWorld* GetWorld();                        // 0x00686A70 (main game component +0x24)
    CGameSubsystem* GetSubsystem();            // 0x00686D60 (main game component +0x1c)
};

struct CTCAmbientBird : CTCBase {
    bool  Flying;                              // +0x0c
    float InitialHeight;                       // +0x10
    virtual void OnInitialActivate();
};

void CTCAmbientBird::OnInitialActivate()
{
    if (!GetSubsystem()->IsSuppressed()) {
        if (GFFloatRandom(1.0f, GetWorld()->GetPRandomSeed()) > 0.5f)
            ParentThing->Kill(false);
    }
}
