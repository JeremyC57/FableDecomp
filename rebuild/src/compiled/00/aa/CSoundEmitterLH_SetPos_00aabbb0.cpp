// CSoundEmitterLH::SetPos(const C3DVector& position) -- when the sound system is up and the emitter is positional, store the new position and pass a copy of it to the event manager for every active sound that has started and is not stopping
// Leaf: ?SetPos@CSoundEmitterLH@@UAEXABVC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AABBB0, 146 bytes (+ int3 padding), thiscall `ret 4`.
// Name (agentSND2, 2026-11-10): vtable 0x0129E6EC (RTTI CSoundEmitterLH : CASoundEmitter : CBaseClass) slot 15; the slot name
//   is engine/CASoundEmitter.h's (real, Fable Anniversary 360 PDB CSoundEmitterUN publics + the TLC CThingSoundEmitter forwarders); was CSoundEmitterLH::vfunc_15.
// Model: CSoundEmitterLH derives from the shared engine/CASoundEmitter.h and overrides the slot with its header signature.
//   No PDB covers the audio module, so its members are descriptive (one spelling in every self row): PManager +0x04 (the
//   owning CSoundManagerLH, ctor 0x00AAC580), Position +0x08, ActiveSounds +0x14 (STLport list of
//   CCountedPointer<CActiveSound>, push_back 0x00AACCC0), Positional +0x18 / Playing +0x19 / Active +0x1A (the getters
//   Is3D 0x00AAB9D0 / IsPlaying 0x00AAB9E0 / IsActive 0x00AAB9F0), EmitterID +0x1C (CSoundManagerLH::GetNextEmitterID).
//   CSoundManagerLH derives from the shared engine/CASoundManager.h (opt-in FABLE_RETAIL_CASOUNDMANAGER_SIZE40: the base is
//   0x40 bytes, the LH members start with the libpk sound system at +0x40, as the landed LH ctor / dtor rows model it).
//   The libpk classes have no PDB or RTTI: CSoundSystem (vtable 0x012B1E24, the landed spelling) slot 6 0x00C0A590 returns
//   its +0x38 CSoundEventManager (vtable 0x012B2748, ctor 0x00C11D00); the event-manager slot names are descriptive, from
//   what the emitter does with them.
// Shape: the two guards (system, positional flag) share the early exit; the 1-aligned C3DVector (retail packs it, EXTRA_RULES)
//   assigned through a pointer kept in ebp; per sound a named C3DVector built component-wise from the member before
//   GetEventManager(), then the slot-27 call with the sound's handle and the copy's address.
#define FABLE_RETAIL_CASOUNDMANAGER_SIZE40
#pragma pack(push, 1)
#include "engine/C3DVector.h"
#pragma pack(pop)
#include "engine/CASoundManager.h"
#include "engine/CASoundEmitter.h"
#include "engine/CASoundBank.h"
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CCountedPointer.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"

class CSoundEventManager {                                // libpk sound-event manager, vtable 0x012B2748 (descriptive slot names)
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21();
    virtual void StopEvent(long sampleID);                                   // slot 22 (+0x58) 0x00C11330
    virtual void ReleaseEventLoop(long sampleID);                            // slot 23 (+0x5C) 0x00C114B0
    virtual void v24();
    virtual void SetEventVolume(long sampleID, float volume);                // slot 25 (+0x64) 0x00C113B0
    virtual void SetEventPitch(long sampleID, float pitch);                  // slot 26 (+0x68) 0x00C11430
    virtual void SetEventPosition(long sampleID, const C3DVector& position); // slot 27 (+0x6C) 0x00C11520
    virtual float GetEventValue(long sampleID, float* value);                // slot 28 (+0x70) 0x00C11650
    virtual long GetEventState(long sampleID);                               // slot 29 (+0x74) 0x00C11C00 (1 / 2 = playing)
};

class CSoundSystem {                                      // libpk sound system, vtable 0x012B1E24 (landed spelling)
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual CSoundEventManager* GetEventManager();                           // slot 6 (+0x18) 0x00C0A590 (descriptive)
};

class CSoundBankLH;

class CSoundManagerLH : public CASoundManager {           // vtable 0x0129D2C4
public:
    CSoundSystem* m_pSoundSystem;                         // +0x40 (the landed CSoundManagerLH rows' spelling)
};

class CSoundEmitterLH : public CASoundEmitter {           // vtable 0x0129E6EC
public:
    class CActiveSound : public CBaseClassNonCopyable {   // vtable 0x0129E67C, 0x24 bytes (descriptive members)
    public:
        long          State;                              // +0x04 (0 once started by PlaySample)
        long          SampleID;                           // +0x08 the libpk event handle (-1 once stopped)
        CASoundBank*  PBank;                              // +0x0C the bank PlaySample / PlaySoundEvent played from
        float         StartTime;                          // +0x10
        bool          Started;                            // +0x14
        bool          Stopping;                           // +0x15
        long          Sample;                             // +0x18 the sample index in that bank
        CCharString   Name;                               // +0x1C
        CSoundBankLH* PAtmos;                             // +0x20
    };
    CSoundManagerLH*                           PManager;      // +0x04
    C3DVector                                  Position;      // +0x08
    std::list<CCountedPointer<CActiveSound> >  ActiveSounds;  // +0x14
    bool                                       Positional;    // +0x18
    bool                                       Playing;       // +0x19
    bool                                       Active;        // +0x1A
    long                                       EmitterID;     // +0x1C
    virtual void SetPos(const C3DVector& position);
};

void CSoundEmitterLH::SetPos(const C3DVector& position)
{
    if (PManager->m_pSoundSystem && Positional) {
        Position = position;
        for (std::list<CCountedPointer<CActiveSound> >::iterator it = ActiveSounds.begin(); it != ActiveSounds.end(); ++it) {
            CActiveSound* sound = it->Data;
            if (sound->Started && !sound->Stopping) {
                C3DVector soundPosition(Position.X, Position.Y, Position.Z);
                PManager->m_pSoundSystem->GetEventManager()->SetEventPosition(sound->SampleID, soundPosition);
            }
        }
    }
}
