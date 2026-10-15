// CTCTrophy::UpdateGainedWitnessSounds() -- when witness sounds are pending and their frame has come, plays the "gained witness" sound, counts it off and schedules the next one SecsDelayPerGainedWitnessSound seconds later
// Grade: DIFFER(ours v retail) -- the frame/fps conversion: ours `mov [esp+8],fps ; call ; mov [esp+0Ch],frame ; fild fps ; fmul ; fiadd frame`, retail stores fps at +0Ch, frame at +8 and does `fild frame ; fild fps ; fmul ; faddp` (92v94)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DEE80, length 94 bytes.
// Callees: the static world-frame getter @ 0x0049D870 (twice; manifest "GFGetPreMainMemoryUsed" is propagated),
// CTCTrophy::PlaySoundW(const CCharString&) @ 0x006DDCD0 (result dropped), __ftol2 @ 0x00BFEA70. The frames-per-second
// global @ 0x01375550 is a long (`fild`).
// Shape: both conditions folded into one `jle` chain to the epilogue; the def pointer is re-read after the call
// (edi); the long-to-float conversions go through the 8-byte frame (`sub esp,8`).
// Layout: Ego_r.pdb CTCTrophy (no list shift before +0x34): PDef +0x0C, PendingGainedWitnessSounds +0x2C,
// FrameForNextGainedWitnessSound +0x30. CTrophyDef (PDB): SecsDelayPerGainedWitnessSound +0x44, SoundGainedWitness +0x50.
struct CCharString {
    void* m_pData;
};

struct CTrophyDef {
    unsigned char _pad[0x44];
    float         SecsDelayPerGainedWitnessSound;   // +0x44
    unsigned char _pad48[0x8];
    CCharString   SoundGainedWitness;               // +0x50
};

struct CDefPointerTrophyDef {
    const CTrophyDef* Object;
};

struct CWorld {
    static long GetWorldFrame();            // 0x0049D870
};

extern long g_FramesPerSecond;              // 0x01375550

struct CTCTrophy {
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
        PlaySoundW(PDef.Object->SoundGainedWitness);
        const CTrophyDef* def = PDef.Object;
        --PendingGainedWitnessSounds;
        long framesPerSecond = g_FramesPerSecond;
        long frame = CWorld::GetWorldFrame();
        FrameForNextGainedWitnessSound = (long)((float)framesPerSecond * def->SecsDelayPerGainedWitnessSound + (float)frame);
    }
}
