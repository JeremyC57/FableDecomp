// CTCNoise::ResetLifeTimer() -- restarts the noise's life timer: (definition life time in seconds) x (frames per second) + 5 frames
// Leaf: ResetLifeTimer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007497C0, 33 bytes. Manifest label "CTCHeroExperience::AddSuccessfulBlockParry"
// is propagated from an unrelated body: the members are CTCNoise's (PDB: Definition CTCNoiseDef @ +0x0C, its LifeTime @ +0x2C
// -> +0x38; LifeTimer @ +0x40), and CTCNoise's vtable slot 4 (0x007498E0, the definition refresh) inlines exactly this
// body in its ENoiseType 0 case. This is the out-of-line copy of that inline member (no direct callers).
// Callees: CTCBase world accessor @ 0x00686D60 (returns a global's +0x1C; the result is unused here), __ftol2 @ 0x00BFEA70.
// Globals: frames-per-second long @ 0x01375550 (15), float pool 5.0f @ 0x012462A8.
//
// Shape: `call` with ecx still `this` and eax dropped = a __thiscall accessor called as a bare statement;
// `fild [g] ; fmul [this+38h] ; fadd [5.0f]` = `g * LifeTime + 5.0f` (the int global is the left operand),
// truncated by __ftol2 into LifeTimer.

extern long g_FramesPerSecond;               // 0x01375550

struct CTCNoiseDef_Retail {                  // embedded definition, +0x0C in CTCNoise
    void*         __vftable;                 // +0x00
    unsigned char _pad_0x04[0x24];           // +0x04 CDefBase fields
    long          Type;                      // +0x28 ENoiseType
    float         LifeTime;                  // +0x2C seconds
    float         DistanceCanBeHeardFrom;    // +0x30
};

struct CTCNoise_Retail {
    void*              __vftable;            // +0x00
    void*              ParentThing;          // +0x04 CTCBase
    bool               ToBeRemoved;          // +0x08
    bool               IsInGlobalUpdateTCs;  // +0x09
    unsigned char      _pad_0x0a[0x2];
    CTCNoiseDef_Retail Definition;           // +0x0C
    long               LifeTimer;            // +0x40 frames
    float              DistanceCanBeHeardFrom; // +0x44

    void* GetWorld();                        // 0x00686D60
    void ResetLifeTimer();
};

void CTCNoise_Retail::ResetLifeTimer()
{
    GetWorld();
    LifeTimer = (long)(g_FramesPerSecond * Definition.LifeTime + 5.0f);
}
