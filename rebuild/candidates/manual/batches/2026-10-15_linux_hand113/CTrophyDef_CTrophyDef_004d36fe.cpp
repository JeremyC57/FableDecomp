// CTrophyDef::CTrophyDef() -- base CSubDefClassBase(), own vtable, the ShowTrophyAnimName def string = -1 and the seven sound-name CCharStrings default-constructed
// Leaf: ??0CTrophyDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D36FE, length 78 bytes.
// Callees: CSubDefClassBase::CSubDefClassBase @ 0x0044C0C0 (the manifest's "CShoveHaPennyRegionDef" label is
// propagated), CCharString::CCharString() @ 0x0099E4B0 (seven times). Vtable 0x0123AC1C = CTrophyDef.
// Layout: engine/CTrophyDef.h (0x64 bytes): MinigameInitialTimerSecs +0x28 .. SecsDelayPerGainedWitnessSound
// +0x44, ShowTrophyAnimName (CDefString TablePos) +0x3c, the seven sound names (CCharString) +0x48..+0x60.
// The header is a flat struct (vptr as a void* word, the strings as their data words), so the class is
// modelled locally over the same offsets with real member types.
// Shape: as the landed CTavernGameDef constructor 0x004E1049: the def string's inline constructor stores
// -1 after the vtable (`or dword ptr [esi+3Ch],-1` = a size-optimised TU, `#pragma optimize("s", on)`),
// then the string members in declaration order; the scalar members are left uninitialised.
#pragma optimize("s", on)
struct CSubDefClassBase {
    virtual void v00();
    unsigned char _body[0x24];                     // +0x04 .. +0x28
    CSubDefClassBase();                            // 0x0044C0C0
};

struct CDefString {
    long TablePos;
    CDefString() : TablePos(-1) { }
};

struct CCharString {
    void* m_pStringData;
    CCharString();                                 // 0x0099E4B0
};

struct CTrophyDef : CSubDefClassBase {             // vtable 0x0123AC1C
    float         MinigameInitialTimerSecs;        // +0x28
    float         TimeBonusPerWitness;             // +0x2c
    long          RenownGainedPerWitnessAhead;     // +0x30
    bool          EvilTrophy;                      // +0x34
    bool          FakeTrophy;                      // +0x35
    long          EmoteIconForAvailableWitness;    // +0x38
    CDefString    ShowTrophyAnimName;              // +0x3c
    float         GameDebugTextScale;              // +0x40
    float         SecsDelayPerGainedWitnessSound;  // +0x44
    CCharString   SoundStartGame;                  // +0x48
    CCharString   SoundShowTrophy;                 // +0x4c
    CCharString   SoundGainedWitness;              // +0x50
    CCharString   SoundOutOfTime;                  // +0x54
    CCharString   SoundGotAllWitnesses;            // +0x58
    CCharString   SoundEndNewHighScore;            // +0x5c
    CCharString   SoundEndNoNewScore;              // +0x60
    virtual void v00();
    CTrophyDef();
};

CTrophyDef::CTrophyDef()
{
}
