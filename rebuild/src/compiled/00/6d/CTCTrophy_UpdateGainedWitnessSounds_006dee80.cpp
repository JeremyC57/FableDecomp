// CTCTrophy::UpdateGainedWitnessSounds() -- when witness sounds are pending and their frame has come, plays the "gained witness" sound, counts it off and schedules the next one SecsDelayPerGainedWitnessSound seconds later
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DEE80, length 94 bytes.
// Callees: the static world-frame getter @ 0x0049D870 (twice; manifest "GFGetPreMainMemoryUsed" is propagated),
// CTCTrophy::PlaySoundW(const CCharString&) @ 0x006DDCD0 (result dropped), __ftol2 @ 0x00BFEA70. The frames-per-second
// global @ 0x01375550 is a long (`fild`).
// Shape: both conditions folded into one `jle` chain to the epilogue; the def pointer is re-read after the call
// (edi); the frame-rate global is copied to a local before the second frame read, which stays a call inside the
// expression `(long)(fps * delay + GetWorldFrame())` -- with a named `frame` local VC folds it into `fiadd`, retail
// `fild`s the stored call result first and adds with `faddp`; the long-to-float conversions go through the 8-byte frame.
// Layout: Ego_r.pdb CTCTrophy (no list shift before +0x34): PDef +0x0C, PendingGainedWitnessSounds +0x2C,
// FrameForNextGainedWitnessSound +0x30 (no engine/CTCTrophy.h; modelled locally). CTrophyDef from engine/CTrophyDef.h
// (SecsDelayPerGainedWitnessSound +0x44, SoundGainedWitness +0x50 flattened to its PStringData word).
#include "engine/CTrophyDef.h"

class CCharString { public:
    void* m_pData;
};


struct CDefPointerTrophyDef {
    const CTrophyDef* Object;
};

class CWorld { public:
    static long GetWorldFrame();            // 0x0049D870
};

extern long g_FramesPerSecond;              // 0x01375550

class CTCTrophy { public:
    void*                __vftable;         // +0x00
    unsigned char        _pad_0x04[0x8];
    CDefPointerTrophyDef PDef;              // +0x0C
    unsigned char        _pad_0x10[0x1C];
    long                 PendingGainedWitnessSounds;        // +0x2C
    long                 FrameForNextGainedWitnessSound;    // +0x30
    unsigned long PlaySoundW(const CCharString& name);      // 0x006DDCD0
    void UpdateGainedWitnessSounds();
};

void CTCTrophy::UpdateGainedWitnessSounds()
{
    if (PendingGainedWitnessSounds > 0 && CWorld::GetWorldFrame() > FrameForNextGainedWitnessSound) {
        PlaySoundW(*(const CCharString*)&PDef.Object->SoundGainedWitness_PStringData);
        const CTrophyDef* def = PDef.Object;
        --PendingGainedWitnessSounds;
        long framesPerSecond = g_FramesPerSecond;
        FrameForNextGainedWitnessSound = (long)(framesPerSecond * def->SecsDelayPerGainedWitnessSound + CWorld::GetWorldFrame());
    }
}