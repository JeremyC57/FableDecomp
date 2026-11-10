// CSoundEmitterLH::StopAllSamples() -- when the sound system is up: stop the event of every active sound of this emitter and mark it -1, prune the stopped sounds, then empty the list
// Leaf: ?StopAllSamples@CSoundEmitterLH@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AAC2E0, length 90 bytes (+ int3 padding).
// Name (agentNF3, 2026-11-10): vtable 0x0129E6EC (RTTI CSoundEmitterLH : CASoundEmitter : CBaseClass) slot 10 = 0x00AAC2E0;
//   engine/CASoundEmitter.h slot 10 StopAllSamples (Fable Anniversary 360 PDB ?StopAllSamples@CSoundEmitterUN@@UAAXXZ). The
//   destructor ~CSoundEmitterLH 0x00AAC4F0 / ??_G 0x00AACEA0 call it directly. Was landed as the descriptive
//   CAnimQueueOwner00aac2e0::StopAllQueuedAnims (catalog CSoundEmitterLH::Shutdown) over a local "animation" model: the +0x40 read
//   is CSoundManagerLH's libpk sound system, slot 6 its event manager and slot 22 (+0x58) StopEvent.
// Model: the landed CSoundEmitterLH rows' (StopSpecificSample 0x00AAC400 et al.): shared engine/CASoundEmitter.h base, descriptive
//   members (PManager +0x04, ActiveSounds +0x14, the STLport list of CCountedPointer<CActiveSound>).
// Callees: the event manager's slot 22 (StopEvent), CSoundEmitterLH::RemoveInactiveSounds 0x00AABCA0 (landed spelling),
//   _List_base<CCountedPointer<CActiveSound> >::clear 0x00AACC60 (STLport, out of line: tail jump with the list in ecx; the landed
//   destructor 0x00AAC4F0 binds the same spelling).
// Shape: a speed body with ebp as a general register (`mov ebp,ecx`), a 7-byte `lea esp,[esp]` loop alignment, the sound system
//   re-read from the manager every iteration (after the virtual call), the list pointer in ebx for the end compare and the final
//   tail jump.
#define FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE
#define FABLE_RETAIL_CASOUNDMANAGER_SIZE40
#include "engine/C3DVector.h"
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
    void RemoveInactiveSounds();                                             // 0x00AABCA0
    virtual void StopAllSamples();
};
void CSoundEmitterLH::StopAllSamples()
{
    if (PManager->m_pSoundSystem) {
        for (std::list<CCountedPointer<CActiveSound> >::iterator it = ActiveSounds.begin(); it != ActiveSounds.end(); ++it) {
            CActiveSound* sound = it->Data;
            PManager->m_pSoundSystem->GetEventManager()->StopEvent(sound->SampleID);
            sound->SampleID = -1;
        }
        RemoveInactiveSounds();
        ActiveSounds.clear();
    }
}
