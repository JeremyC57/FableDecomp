// CThingSoundEmitter::IsSamplePlaying(unsigned long) const -- asks the emitter's sound object whether the sample is playing; false without a game state or when the thing is dead
// Retail: Fable.exe 0x00836870, 34 bytes (`ret 4`).
// Callee: the sound object at +0x98 through its vtable (a sibling-call `jmp`, the arguments passed through
// in place); globals 0x013B8394 (the game state: no sound while it is null) and 0x0129D25C (the invalid
// sound handle).
// Retail CThingSoundEmitter: the CThing flag byte +0x91 (PDB +0xa1, bit 0 = Dead) and a sound object at
// +0x98 that Ego_r.pdb lacks (the ctor 0x008367B0 zeroes it). Local model.
// Shape: the global test first, then the dead bit, through one inline guard (`g && !Dead`);
// the fallback is `xor al,al`.
class CASoundBank;
class CSoundInit;
class CCharString { public: void* m_pData; };

extern void*         g_pGameState;                       // 0x013B8394
extern unsigned long g_InvalidSoundHandle;               // 0x0129D25C

struct CSoundPlayer {                                    // the emitter's sound object at +0x98
    virtual void v000(); virtual void v001(); virtual void v002();
    virtual unsigned long PlaySample(CASoundBank& bank, long sample, bool loop, const CSoundInit& init, bool attached);           // slot 3 (+0x0c)
    virtual unsigned long PlaySoundEvent(CASoundBank& bank, const CCharString& name, bool loop, CSoundInit& init, bool attached); // slot 4 (+0x10)
    virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014();
    virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020();
    virtual bool IsSampleLoadedAndPlaying(unsigned long sampleID) const;   // slot 21 (+0x54)
    virtual bool IsSamplePlaying(unsigned long sampleID) const;            // slot 22 (+0x58)
};

class CThingSoundEmitter { public:
    unsigned char  _pad00[0x91];
    unsigned char  Dead : 1;                             // +0x91 bit 0 (CThing flags, PDB +0xa1)
    unsigned char  _flags91_1 : 7;
    unsigned char  _pad92[0x6];
    CSoundPlayer*  m_pSoundPlayer;                       // +0x98 (retail-only member, not in Ego_r.pdb)
    bool IsActive() const { return g_pGameState && !Dead; }
    unsigned long PlaySample(CASoundBank& bank, long sample, bool loop, const CSoundInit& init, bool attached);
    unsigned long PlaySoundEvent(CASoundBank& bank, const CCharString& name, bool loop, CSoundInit& init, bool attached);
    bool IsSamplePlaying(unsigned long sampleID) const;
    bool IsSampleLoadedAndPlaying(unsigned long sampleID) const;
};

bool CThingSoundEmitter::IsSamplePlaying(unsigned long sampleID) const
{
    if (IsActive())
        return m_pSoundPlayer->IsSamplePlaying(sampleID);
    return false;
}