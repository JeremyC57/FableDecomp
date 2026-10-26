// CTCSoundPlayer::InternalPlaySoundEvent(CASoundBank& bank, const CCharString& name, bool flag, bool loop, bool attached, long overrideID) -- play a named sound event through the attached emitter with fader bits 0x8010 and the given override id
// Leaf: ?InternalPlaySoundEvent@CTCSoundPlayer
// Retail: Fable.exe (TLC, VC7.1) @ 0x0066B120, 163 bytes (+ int3 padding to 176), thiscall `ret 18h`.
//   Name: Ego_r.pdb `?InternalPlaySoundEvent@CTCSoundPlayer@@AAEKAAVCASoundBank@@ABVCCharString@@_N22J@Z` (the landed
//   PlayAttachedCriteria 0x0066BD00 already uses this name). Callers: 0x0066B24A (PlayAttachedSoundEvent 0x0066B1D0),
//   0x0066B866..0x0066B9C5, 0x0066BD78.
// Label correction: the catalog "CTCSoundPlayer_GetSoundState" is descriptive.
// Callees/globals: CThingSoundEmitter::Create(const C3DVector&, const CThing*, bool) @ 0x008369F0 (static __fastcall),
//   CThingSoundEmitter::PlaySample @ 0x00836810 / PlaySoundEvent @ 0x00836840 (landed), CTCSoundPlayer::
//   CreatePAttachedEmitter(bool) @ 0x0066AEF0 (landed), CThing::GetSampleBank() const @ 0x004C7A80, the CASoundBank virtual
//   slot 9 (+0x24, a by-name event test; base 0x00A0BD20 returns false; named here IsSoundEventValid), the game state
//   @ 0x013B8394 (no sound while null), the invalid sound handle @ 0x0129D25C.
// Layout: Ego_r.pdb CSoundInit (Volume, Pitch, Pan, NoPlays, PlaybackType, GainFaderBits, PitchFaderBits, OverrideID; the
//   default ctor of the landed CGameScriptInterface::PlaySoundAtPos 0x00891170: PlaybackType/GainFaderBits in the body),
//   CSoundPair {FirstSound, LastSound}; CTCBase ParentThing +4; CThing PhysicsTC +0x60, Position +0x0c.
// Shape: as InternalPlaySoundEventAtPos 0x0066A920 with CreatePAttachedEmitter(flag) and `init.OverrideID = overrideID`.
// No pragma.
class CCharString { public: void* PStringData; };
class C3DVector { public: float X, Y, Z; };
class CASoundBank {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool IsSoundEventValid(const CCharString& name);   // slot 9 (+0x24); CASoundBank 0x00A0BD20 returns false, CSoundBankLH 0x00A0BA40
};
class CSoundInit {
public:
    float         Volume;                       // +0x00
    float         Pitch;                        // +0x04
    float         Pan;                          // +0x08
    long          NoPlays;                      // +0x0c
    long          PlaybackType;                 // +0x10
    unsigned long GainFaderBits;                // +0x14
    unsigned long PitchFaderBits;               // +0x18
    long          OverrideID;                   // +0x1c
    CSoundInit() : Volume(1.0f), Pitch(1.0f), Pan(0.0f), NoPlays(1), PitchFaderBits(0), OverrideID(-1)
    {
        PlaybackType = 1;
        GainFaderBits = 0;
    }
};
class CSoundPair {
public:
    long FirstSound;                            // +0x00
    long LastSound;                             // +0x04
};
class CThing;
class CThingSoundEmitter {
public:
    static CThingSoundEmitter* __fastcall Create(const C3DVector& position, const CThing* parent, bool flag);   // 0x008369F0
    unsigned long PlaySample(CASoundBank& bank, long sample, bool loop, const CSoundInit& init, bool attached);      // 0x00836810
    unsigned long PlaySoundEvent(CASoundBank& bank, const CCharString& name, bool loop, CSoundInit& init, bool attached);   // 0x00836840
};
class CTCPhysicsBase {
public:
    unsigned char _pad00[0x0c];
    C3DVector     Position;                     // +0x0c
};
class CThing {
public:
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
    const C3DVector& GetPosition() const { return PhysicsTC->Position; }   // CThing::GetPosition 0x004C73D0, inline
    CASoundBank& GetSampleBank() const;         // 0x004C7A80
};
extern void*         g_pGameState;              // 0x013B8394 (no sound while null)
extern unsigned long g_InvalidSoundHandle;      // 0x0129D25C
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890
class CRandomGenerator {                        // the landed 0x0066B300 model
public:
    unsigned long Seed;
    __forceinline unsigned long Next() { Seed = Seed * 0x24a1 + 0x24df; Seed = GFROR13(Seed); return Seed; }
    __forceinline unsigned long Random(unsigned long n) { unsigned long r = Next(); return n ? r % n : 0; }
};
class CGameSubsystem {
public:
    unsigned char    _pad00[0x16140];
    CRandomGenerator Random;                    // +0x16140
};
class CTCBase {
public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    CGameSubsystem* GetGameSubsystem();         // 0x00686D60
};
class CTCSoundPlayer : public CTCBase {
public:
    unsigned char _pad08[0x24 - 0x08];          // PAttachedEmitter +0x0c, VoiceName +0x14, ExtraTag +0x18, ...
    CThingSoundEmitter* CreatePAttachedEmitter(bool flag);   // 0x0066AEF0
    unsigned long InternalPlaySoundEventAtPos(CASoundBank& bank, const C3DVector& pos, const CCharString& name, bool loop, bool attached);
    unsigned long PlayUnattachedSample(long sample, const CSoundInit& init, bool loop, bool attached);
    unsigned long InternalPlayAttachedSample(CASoundBank& bank, long sample, const CSoundInit& init, bool flag, bool loop, bool attached);
    unsigned long InternalPlaySoundEvent(CASoundBank& bank, const CCharString& name, bool flag, bool loop, bool attached, long overrideID);
    unsigned long PlayAttachedSample(const CSoundPair& sound, const CSoundInit& init, bool flag, bool loop, bool attached);
};

unsigned long CTCSoundPlayer::InternalPlaySoundEvent(CASoundBank& bank, const CCharString& name, bool flag, bool loop, bool attached, long overrideID)
{
    if (g_pGameState) {
        CThingSoundEmitter* emitter = CreatePAttachedEmitter(flag);
        if (emitter && bank.IsSoundEventValid(name)) {
            CSoundInit init;
            init.GainFaderBits = 0x8010;
            init.OverrideID = overrideID;
            return emitter->PlaySoundEvent(bank, name, loop, init, attached);
        }
    }
    return g_InvalidSoundHandle;
}