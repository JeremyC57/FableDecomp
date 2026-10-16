// CTCOracleMinigame::FireOutput() -- pick a random symbol 0..3, record it as the next output, mark the symbol firing since this frame, count the output and play the coloured-flare sound
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E61B0, length 125 bytes.
// Callees: CTCBase::GetWorld @ 0x00686A70, the world's random-seed accessor @ 0x0049D810 (catalog "CVertexBufferWin32::DoSizeof" is a
// byte twin), GFROR13(unsigned long) @ 0x00497890 (__fastcall), GetWorldFrame @ 0x0049D870, CCharString::CCharString(const char*, long)
// @ 0x0099EBF0 ("SND_MM_COLOURED_FLARE" @ 0x01270824), CTCOracleMinigame::PlaySound(const CCharString&) @ 0x007E60F0 (catalog
// "PlaySoundW"), CCharString::~CCharString @ 0x0099EAE0.
// Layout: the offsets agree with rebuild/include/engine/CTCOracleMinigame.h (NumOutputsFired +0x24, Output[32] +0x28, SymbolsFiring[4]
// +0xC0, FramesFired[4] +0xD0); that header is a data-only pack(1) struct with no member functions, so `this` is modelled locally
// with the same members.
// Shape: the landed CRandomGenerator::Next inline (seed * 0x24A1 + 0x24DF, then GFROR13) & 3 into a named local; the sound name is
// a named CCharString local (an unnamed temporary passes the ctor's eax instead of `lea ecx ; push ecx`).
long __fastcall GFROR13(unsigned long value);       // 0x00497890
long GetWorldFrame();                               // 0x0049D870
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};
struct CRandomGenerator {
    unsigned long Seed;
    __forceinline unsigned long Next() { Seed = Seed * 0x24a1 + 0x24df; Seed = GFROR13(Seed); return Seed; }
};
struct CWorld {
    CRandomGenerator* GetRandomGenerator();         // 0x0049D810
};
struct CThing;
struct CTCOracleMinigame {
    virtual void v00();
    CThing* ParentThing;                            // +0x04 (CTCBase)
    unsigned char _base08[4];
    unsigned long CurrentText;                      // +0x0c
    long  Level;                                    // +0x10
    long  Stage;                                    // +0x14 EOracleMinigameState
    long  PostWaitStage;                            // +0x18
    long  WaitTime;                                 // +0x1c
    float TimeBetweenOutputs;                       // +0x20
    long  NumOutputsFired;                          // +0x24
    long  Output[32];                               // +0x28
    bool  Success;                                  // +0xa8
    long  NextOutputFrame;                          // +0xac
    CThing* Symbols[4];                             // +0xb0
    long  SymbolsFiring[4];                         // +0xc0
    long  FramesFired[4];                           // +0xd0
    CWorld* GetWorld();                             // 0x00686A70 (CTCBase)
    unsigned long PlaySound(const CCharString& name);   // 0x007E60F0
    void FireOutput();
};

void CTCOracleMinigame::FireOutput()
{
    long symbol = GetWorld()->GetRandomGenerator()->Next() & 3;
    Output[NumOutputsFired] = symbol;
    SymbolsFiring[symbol] = 1;
    FramesFired[symbol] = GetWorldFrame();
    ++NumOutputsFired;
    CCharString sound("SND_MM_COLOURED_FLARE");
    PlaySound(sound);
}