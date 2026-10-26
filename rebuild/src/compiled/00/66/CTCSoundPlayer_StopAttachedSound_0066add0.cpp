// CTCSoundPlayer::StopAttachedSound(unsigned long sampleID) -- stop one attached sample: while the sound system exists and the attached emitter (PAttachedEmitter, +0xc) is set and not dead, forward to the emitter's StopSpecificSample
// Leaf: ?StopAttachedSound@CTCSoundPlayer@@QAEXK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0066ADD0, 63 bytes, thiscall `ret 4`.
// Identity (sweep3; was CTCSoundPlayer::StopSample (retry 11a label; no PDB StopSample), source CGameScriptThing::SetToKillOnLevelUnload_0066add0): Ego_r.pdb / FableWin.pdb `?StopAttachedSound@CTCSoundPlayer@@QAEXK@Z`.
// Order: Ego_r.pdb CTCSoundPlayer ... PlayUnattachedSample, (InternalPlaySoundEventAtPos, InternalPlaySoundEvent inlined in
//   retail), ReleaseAttachedLoopingSound, StopAttachedSound, IsAttachedSoundPlaying, IsAttachedSoundLoadedAndPlaying,
//   StopAllAttachedSounds = retail PlayUnattachedSample 0x0066ACE0, 0x0066AD90 (-> emitter ReleaseLoop 0x008368F0), 0x0066ADD0
//   (-> StopSpecificSample 0x008368D0), IsAttachedSoundPlaying 0x0066AE10 (landed), 0x0066AE60 (-> IsSampleLoadedAndPlaying
//   0x008368A0), StopAllAttachedSounds 0x0066AEB0 (landed); each forwards its unsigned long sample id to the like-named
//   Ego_r CThingSoundEmitter member (ReleaseLoop / StopSpecificSample / IsSampleLoadedAndPlaying, all `K`).
// Callees: CBaseIntelligentPointer::GetPItem() const @ 0x00A01B50 three times (re-fetched for every access),
//   CThingSoundEmitter::StopSpecificSample(unsigned long) @ 0x008368D0.
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
    void StopSpecificSample(unsigned long sampleID);   // 0x008368D0
};

class CTCSoundPlayer {
public:
    unsigned char _tcbase[0xc];                     // CTCBase
    CBaseIntelligentPointer PAttachedEmitter;        // +0x0c, CIntelligentPointer<CThingSoundEmitter>
    void StopAttachedSound(unsigned long sampleID);
};

void CTCSoundPlayer::StopAttachedSound(unsigned long sampleID)
{
    if (g_pSoundSystem) {
        if (PAttachedEmitter.GetPItem()) {
            if (!((const CThingSoundEmitter*)PAttachedEmitter.GetPItem())->Dead)
                ((CThingSoundEmitter*)PAttachedEmitter.GetPItem())->StopSpecificSample(sampleID);
        }
    }
}
