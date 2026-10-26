// CTCSoundPlayer::IsAttachedSoundLoadedAndPlaying(unsigned long sampleID) const -- ask whether an attached sample is loaded and playing: while the sound system exists and the attached emitter (PAttachedEmitter, +0xc) is set and not dead, forward to the emitter's IsSampleLoadedAndPlaying; else false
// Leaf: ?IsAttachedSoundLoadedAndPlaying@CTCSoundPlayer@@QBE_NK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0066AE60, 69 bytes, thiscall `ret 4`.
// Identity (sweep3; was CGameScriptThing::IsSampleLoadedAndPlaying (no PDB CGameScriptThing member of that name)): Ego_r.pdb / FableWin.pdb `?IsAttachedSoundLoadedAndPlaying@CTCSoundPlayer@@QBE_NK@Z`.
// Order: Ego_r.pdb CTCSoundPlayer ... PlayUnattachedSample, (InternalPlaySoundEventAtPos, InternalPlaySoundEvent inlined in
//   retail), ReleaseAttachedLoopingSound, StopAttachedSound, IsAttachedSoundPlaying, IsAttachedSoundLoadedAndPlaying,
//   StopAllAttachedSounds = retail PlayUnattachedSample 0x0066ACE0, 0x0066AD90 (-> emitter ReleaseLoop 0x008368F0), 0x0066ADD0
//   (-> StopSpecificSample 0x008368D0), IsAttachedSoundPlaying 0x0066AE10 (landed), 0x0066AE60 (-> IsSampleLoadedAndPlaying
//   0x008368A0), StopAllAttachedSounds 0x0066AEB0 (landed); each forwards its unsigned long sample id to the like-named
//   Ego_r CThingSoundEmitter member (ReleaseLoop / StopSpecificSample / IsSampleLoadedAndPlaying, all `K`).
// Callees: CBaseIntelligentPointer::GetPItem() const @ 0x00A01B50 three times (re-fetched for every access),
//   CThingSoundEmitter::IsSampleLoadedAndPlaying(unsigned long) const @ 0x008368A0.
// Shape: as the landed IsAttachedSoundPlaying 0x0066AE10 (the global test first, the pointer's address kept in esi).
// No engine header exists for CTCSoundPlayer or CThingSoundEmitter: local models (CTCBase part as padding).
extern void* g_pSoundSystem;                        // 0x013B8394 (descriptive name, as 0x0066AE10)

class CBaseObject;
class CBaseIntelligentPointer {                     // 8 bytes: vptr + the pointee link
public:
    virtual ~CBaseIntelligentPointer();
    void* PItem;
    CBaseObject* GetPItem() const;                  // 0x00A01B50
};

class CThingSoundEmitter {
public:
    unsigned char _pad00[0x91];
    unsigned char Dead : 1;                         // +0x91 bit 0 (CThing flag)
    unsigned char _flags91 : 7;
    bool IsSampleLoadedAndPlaying(unsigned long sampleID) const;   // 0x008368A0
};

class CTCSoundPlayer {
public:
    unsigned char _tcbase[0xc];                     // CTCBase
    CBaseIntelligentPointer PAttachedEmitter;        // +0x0c, CIntelligentPointer<CThingSoundEmitter>
    bool IsAttachedSoundLoadedAndPlaying(unsigned long sampleID) const;
};

bool CTCSoundPlayer::IsAttachedSoundLoadedAndPlaying(unsigned long sampleID) const
{
    if (g_pSoundSystem) {
        if (PAttachedEmitter.GetPItem()) {
            if (!((const CThingSoundEmitter*)PAttachedEmitter.GetPItem())->Dead)
                return ((const CThingSoundEmitter*)PAttachedEmitter.GetPItem())->IsSampleLoadedAndPlaying(sampleID);
        }
    }
    return false;
}
