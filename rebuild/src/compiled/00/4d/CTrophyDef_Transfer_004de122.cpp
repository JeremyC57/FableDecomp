// Grade: RELOCATION_MATCH (Fable landing 2026-10-19 after the Codex review hold: test exercises a hand-written stand-in, not the candidate)
#pragma optimize("s",on)
// CTrophyDef::Transfer(CPersistContext&) -- serialize each trophy-definition field in the retail order
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DE122, 189 bytes; slot 18 of vtable 0x0123AC1C = CTrophyDef (rtti-vtables.tsv).
// Callees (CPersistContext): Transfer(float&) 0x00431061, Transfer(long&) 0x00431102, Transfer(bool&) 0x0043314A,
//   Transfer(CDefString&) 0x0045228F (dword write / CDefString read 0x0044FC00), Transfer(CCharString&) 0x00431143
//   (labelled CPersistContext::TransferString).
// Layout: PDB (Ego_r) names/types; for this def the retail offsets equal the PDB offsets (+0x28..+0x60).
//   Local model: no engine/ header exists for CTrophyDef or CPersistContext.
struct CDefString { long TablePos; };
struct CCharString { void* PStringData; };
struct CPersistContext {
    void Transfer(float&);                              // 0x00431061
    void Transfer(long&);                               // 0x00431102
    void Transfer(bool&);                               // 0x0043314A
    void Transfer(CDefString&);                         // 0x0045228F
    void Transfer(CCharString&);                        // 0x00431143
};
struct CTrophyDef {
    virtual void Transfer(CPersistContext&);            // vtable slot 18 in retail
    unsigned char _base[0x24];                          // +0x04..+0x27 def base
    float MinigameInitialTimerSecs;                     // +0x28
    float TimeBonusPerWitness;                          // +0x2c
    long  RenownGainedPerWitnessAhead;                  // +0x30
    bool  EvilTrophy;                                   // +0x34
    bool  FakeTrophy;                                   // +0x35
    long  EmoteIconForAvailableWitness;                 // +0x38
    CDefString ShowTrophyAnimName;                      // +0x3c
    float GameDebugTextScale;                           // +0x40
    float SecsDelayPerGainedWitnessSound;               // +0x44
    CCharString SoundStartGame;                         // +0x48
    CCharString SoundShowTrophy;                        // +0x4c
    CCharString SoundGainedWitness;                     // +0x50
    CCharString SoundOutOfTime;                         // +0x54
    CCharString SoundGotAllWitnesses;                   // +0x58
    CCharString SoundEndNewHighScore;                   // +0x5c
    CCharString SoundEndNoNewScore;                     // +0x60
};
void CTrophyDef::Transfer(CPersistContext& c)
{
    c.Transfer(MinigameInitialTimerSecs);
    c.Transfer(TimeBonusPerWitness);
    c.Transfer(RenownGainedPerWitnessAhead);
    c.Transfer(EvilTrophy);
    c.Transfer(FakeTrophy);
    c.Transfer(EmoteIconForAvailableWitness);
    c.Transfer(ShowTrophyAnimName);
    c.Transfer(GameDebugTextScale);
    c.Transfer(SecsDelayPerGainedWitnessSound);
    c.Transfer(SoundStartGame);
    c.Transfer(SoundShowTrophy);
    c.Transfer(SoundGainedWitness);
    c.Transfer(SoundOutOfTime);
    c.Transfer(SoundGotAllWitnesses);
    c.Transfer(SoundEndNewHighScore);
    c.Transfer(SoundEndNoNewScore);
}