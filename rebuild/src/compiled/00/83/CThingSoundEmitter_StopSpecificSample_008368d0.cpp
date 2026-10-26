// CThingSoundEmitter::StopSpecificSample(unsigned long) -- stops one sample on the emitter's sound object unless the game state is gone or the thing is dead
// Leaf: StopSpecificSample
// Retail: Fable.exe (TLC, VC7.1) @ 0x008368D0, 32 bytes, `ret 4`.
// Identity: FableWin.pdb orders the CThingSoundEmitter TU as PlaySample, PlaySoundEvent, IsSamplePlaying,
//   IsSampleLoadedAndPlaying, StopSpecificSample, ReleaseLoop -- retail 0x00836810 / 0x00836840 / 0x00836870 /
//   0x008368A0 / 0x008368D0 / 0x008368F0. The only caller (0x0066ADD0, CTCSoundPlayer::StopAttachedSound) forwards its
//   sample id here. The manifest label "CActiveEntity_KillOnLevelUnload" is propagated and wrong.
// Callee: the sound object at +0x98 through its vtable, slot 9 (+0x24), as a sibling-call `jmp` with the argument
//   passed through in place; global 0x013B8394 (the game state: no sound while it is null).
// Layout: retail CThingSoundEmitter (the landed PlaySample 0x00836810 model): the CThing flag byte +0x91 (bit 0 = Dead)
//   and the retail-only sound object at +0x98. Local model (Ego_r.pdb lacks the +0x98 member).
// Shape: the global test first, then the dead bit, through one inline guard (`g && !Dead`), as the landed siblings.
extern void* g_pGameState;                               // 0x013B8394

struct CSoundPlayer {                                    // the emitter's sound object at +0x98
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004();
    virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008();
    virtual void StopSpecificSample(unsigned long sampleID);   // slot 9 (+0x24)
};

class CThingSoundEmitter { public:
    unsigned char  _pad00[0x91];
    unsigned char  Dead : 1;                             // +0x91 bit 0 (CThing flags, PDB +0xa1)
    unsigned char  _flags91_1 : 7;
    unsigned char  _pad92[0x6];
    CSoundPlayer*  m_pSoundPlayer;                       // +0x98 (retail-only member, not in Ego_r.pdb)
    bool IsActive() const { return g_pGameState && !Dead; }
    void StopSpecificSample(unsigned long sampleID);
};

void CThingSoundEmitter::StopSpecificSample(unsigned long sampleID)
{
    if (IsActive())
        m_pSoundPlayer->StopSpecificSample(sampleID);
}