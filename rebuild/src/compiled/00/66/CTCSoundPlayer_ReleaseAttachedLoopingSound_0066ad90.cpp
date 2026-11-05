// CTCSoundPlayer::ReleaseAttachedLoopingSound(unsigned long sampleID) -- release a looping attached sample: while the sound system exists and the attached emitter (PAttachedEmitter, +0xc) is set and not dead, forward to the emitter's ReleaseLoop
// Leaf: ?ReleaseAttachedLoopingSound@CTCSoundPlayer@@QAEXK@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0066AD90, 63 bytes, thiscall `ret 4`.
// Identity (sweep3; was CGameScriptThing::SetToKillOnLevelUnload_0066ad90 (manifest label propagated from another module)): Ego_r.pdb / FableWin.pdb `?ReleaseAttachedLoopingSound@CTCSoundPlayer@@QAEXK@Z`.
// Order: Ego_r.pdb CTCSoundPlayer ... PlayUnattachedSample, (InternalPlaySoundEventAtPos, InternalPlaySoundEvent inlined in
//   retail), ReleaseAttachedLoopingSound, StopAttachedSound, IsAttachedSoundPlaying, IsAttachedSoundLoadedAndPlaying,
//   StopAllAttachedSounds = retail PlayUnattachedSample 0x0066ACE0, 0x0066AD90 (-> emitter ReleaseLoop 0x008368F0), 0x0066ADD0
//   (-> StopSpecificSample 0x008368D0), IsAttachedSoundPlaying 0x0066AE10 (landed), 0x0066AE60 (-> IsSampleLoadedAndPlaying
//   0x008368A0), StopAllAttachedSounds 0x0066AEB0 (landed); each forwards its unsigned long sample id to the like-named
//   Ego_r CThingSoundEmitter member (ReleaseLoop / StopSpecificSample / IsSampleLoadedAndPlaying, all `K`).
// Callees: CBaseIntelligentPointer::GetPItem() const @ 0x00A01B50 three times (re-fetched for every access),
//   CThingSoundEmitter::ReleaseLoop(unsigned long) @ 0x008368F0.
// Shape: as the landed IsAttachedSoundPlaying 0x0066AE10 (the global test first, the pointer's address kept in esi).
// No engine header exists for CTCSoundPlayer or CThingSoundEmitter: local models (CTCBase part as padding).
class CASoundManager;
extern CASoundManager* PSoundManager;                        // 0x013B8394 (descriptive name, as 0x0066AE10)

class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CThingSoundEmitter {
public:
    unsigned char _pad00[0x91];
    unsigned char Dead : 1;                         // +0x91 bit 0 (CThing flag)
    unsigned char _flags91 : 7;
    void ReleaseLoop(unsigned long sampleID);   // 0x008368F0
};

class CTCSoundPlayer {
public:
    unsigned char _tcbase[0xc];                     // CTCBase
    CBaseIntelligentPointer PAttachedEmitter;        // +0x0c, CIntelligentPointer<CThingSoundEmitter>
    void ReleaseAttachedLoopingSound(unsigned long sampleID);
};

void CTCSoundPlayer::ReleaseAttachedLoopingSound(unsigned long sampleID)
{
    if (((void*)PSoundManager)) {
        if (PAttachedEmitter.GetPItem()) {
            if (!((const CThingSoundEmitter*)PAttachedEmitter.GetPItem())->Dead)
                ((CThingSoundEmitter*)PAttachedEmitter.GetPItem())->ReleaseLoop(sampleID);
        }
    }
}
