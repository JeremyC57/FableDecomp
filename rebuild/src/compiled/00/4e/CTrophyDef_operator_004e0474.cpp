// CTrophyDef::operator=(const CTrophyDef&) -- def copy: CDefClassBase part, then the CTrophyDef fields memberwise in declaration order
// Leaf: ??4CTrophyDef
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E0474, 159 bytes, size-optimised def TU.
// Identity: reached through the 5-byte `jmp` thunk at 0x004E046F in slot 19 of vtable 0x0123AC1C = CTrophyDef
//   (rtti-vtables.tsv); the manifest label "CThingBuildingDef::operator=" is propagated over every def assignment.
// Callees: CDefClassBase::operator= @ 0x00431F10, CCharString::operator= @ 0x0099EFB0.
// Layout (engine/CTrophyDef.h, matches retail): MinigameInitialTimerSecs +0x28, TimeBonusPerWitness +0x2c, RenownGainedPerWitnessAhead +0x30, EvilTrophy +0x34, FakeTrophy +0x35, EmoteIconForAvailableWitness +0x38, ShowTrophyAnimName +0x3c, GameDebugTextScale +0x40, SecsDelayPerGainedWitnessSound +0x44, SoundStartGame +0x48, SoundShowTrophy +0x4c, SoundGainedWitness +0x50, SoundOutOfTime +0x54, SoundGotAllWitnesses +0x58, SoundEndNewHighScore +0x5c, SoundEndNoNewScore +0x60.
// Shape: the implicit memberwise assignment written out (the def module out-lines it); the generated engine header
//   flattens the base (no base operator= to call), so the classes are modelled locally like the landed 0x00463BA2.
#pragma optimize("s", on)

#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)
class CCharString { public:
    void* m_pStringData;
    const CCharString& operator=(const CCharString& other);       // 0x0099EFB0
};
class CDefString { public:
    long TablePos;                      // index into the def string table (trivially copied)
};

// def vtable 0x0123AC1C: slot 0 0x004D7F91 (implicit inline dtor); real overrides from the def vtable scan
class CTrophyDef : public CDefClassBase { public:
    float                    MinigameInitialTimerSecs;    // +0x28
    float                    TimeBonusPerWitness;         // +0x2c
    long                     RenownGainedPerWitnessAhead; // +0x30
    bool                     EvilTrophy;                  // +0x34
    bool                     FakeTrophy;                  // +0x35
    long                     EmoteIconForAvailableWitness; // +0x38
    CDefString               ShowTrophyAnimName;          // +0x3c
    float                    GameDebugTextScale;          // +0x40
    float                    SecsDelayPerGainedWitnessSound; // +0x44
    CCharString              SoundStartGame;              // +0x48
    CCharString              SoundShowTrophy;             // +0x4c
    CCharString              SoundGainedWitness;          // +0x50
    CCharString              SoundOutOfTime;              // +0x54
    CCharString              SoundGotAllWitnesses;        // +0x58
    CCharString              SoundEndNewHighScore;        // +0x5c
    CCharString              SoundEndNoNewScore;          // +0x60
    CTrophyDef& operator=(const CTrophyDef& other);
    virtual void Transfer(CPersistContext& context);         // slot 18 0x004DE122
    virtual void Copy(const CDefClassBase* other);           // slot 19 0x004E046F
    virtual unsigned long GetSizeofClass();                  // slot 20 0x004D374C
};

CTrophyDef& CTrophyDef::operator=(const CTrophyDef& other)
{
    CDefClassBase::operator=(other);
    MinigameInitialTimerSecs = other.MinigameInitialTimerSecs;
    TimeBonusPerWitness = other.TimeBonusPerWitness;
    RenownGainedPerWitnessAhead = other.RenownGainedPerWitnessAhead;
    EvilTrophy = other.EvilTrophy;
    FakeTrophy = other.FakeTrophy;
    EmoteIconForAvailableWitness = other.EmoteIconForAvailableWitness;
    ShowTrophyAnimName = other.ShowTrophyAnimName;
    GameDebugTextScale = other.GameDebugTextScale;
    SecsDelayPerGainedWitnessSound = other.SecsDelayPerGainedWitnessSound;
    SoundStartGame = other.SoundStartGame;
    SoundShowTrophy = other.SoundShowTrophy;
    SoundGainedWitness = other.SoundGainedWitness;
    SoundOutOfTime = other.SoundOutOfTime;
    SoundGotAllWitnesses = other.SoundGotAllWitnesses;
    SoundEndNewHighScore = other.SoundEndNewHighScore;
    SoundEndNoNewScore = other.SoundEndNoNewScore;
    return *this;
}